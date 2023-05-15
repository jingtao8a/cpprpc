#include <cpprpc/server/RpcServer.hpp>
#include <cppweb/Logger.hpp>
#include <cppweb/Singleton.hpp>
#include <cppjson/Document.hpp>
#include <cpprpc/Exception.hpp>
#include <initializer_list>

namespace CPPRPC {

static auto& logger = CPPWEB::Singleton<CPPWEB::Logger>::GetInstance();

static void checkValueType(cppjson::ValueType type, std::initializer_list<cppjson::ValueType> types) {
    if (types.size() == 0) {
        throw NotifyException(RPC_INVALID_REQUEST, "bad type of at least one field");
    }
    for (auto dst : types) {
        if (type == dst) {
            return;
        }
    }
    throw NotifyException(RPC_INVALID_REQUEST, "bad type of at least one field");
}

template<cppjson::ValueType... types>
static cppjson::Value& findValue(cppjson::Value& request, const char* key) {
    auto it = request.findMember(key);
    if (it == request.getObject().end()) {
        throw NotifyException(RPC_INVALID_REQUEST, "missing at least one field");
    }
    checkValueType(it->m_value.getType(), {types...});
    return it->m_value;
}

template<cppjson::ValueType... types>
static cppjson::Value& findValue(cppjson::Value& request, cppjson::Value& id, const char* key) {
    try {
        return findValue<types...>(request, key);
    } catch (NotifyException& e) {
        throw RequestException(e.getErr(), id, e.getDetail());//warp Exception
    }
}

static bool isNotify(const cppjson::Value& request) {
    return request.findMember("id") == request.getObject().end();//无id成员返回true
}

static bool hasParams(const cppjson::Value& request) {
    return request.findMember("params") != request.getObject().end();//有函数参数返回true
}

void RpcServer::addService(std::string serviceName, RpcService* service) {
    assert(m_serviceList.find(serviceName) == m_serviceList.end());
    m_serviceList.emplace(serviceName, service);
}

void RpcServer::handleRequest(const std::string& json, const RpcDoneCallback& done) {
    cppjson::Document request;
    cppjson::ParseError err = request.parse(json);
    if (err != cppjson::PARSE_OK) {
        throw NotifyException(RPC_PARSE_ERROR, cppjson::ParseErrorStr(err));
    }
    switch (request.getType()) {
        case cppjson::TYPE_OBJECT:
            if (isNotify(request)) {
                handleSingleNotify(request);
            } else {
                handleSingleRequest(request, done);
            }
            break;
        default:
            throw NotifyException(RPC_INVALID_REQUEST, "request should be json object");
    }
}

void RpcServer::handleSingleNotify(cppjson::Value& request) {
    validateNotify(request);
    auto methodName = request["method"].getString();
    auto pos = methodName.find('.');
    if (pos == std::string::npos) {
        throw NotifyException(RPC_INVALID_REQUEST, "missing service name in method");
    }
    auto serviceName = methodName.substr(0, pos);
    auto it = m_serviceList.find(serviceName);
    if (it == m_serviceList.end()) {
        throw NotifyException(RPC_METHOD_NOT_FOUND, "service not found");
    }
    methodName = methodName.substr(pos + 1);
    if (methodName.length() == 0) {
        throw NotifyException(RPC_INVALID_REQUEST, "missing method name in method");
    }
    it->second->callNotifyProcedure(methodName, request);
}

void RpcServer::handleSingleRequest(cppjson::Value& request, const RpcDoneCallback& done) {
    validateRequest(request);
    auto& id = request["id"];
    auto methodName = request["method"].getString();
    auto pos = methodName.find('.');
    if (pos == std::string::npos) {
        throw RequestException(RPC_INVALID_REQUEST, id, "missing service name in method");
    }

    auto serviceName = methodName.substr(0, pos);
    auto it = m_serviceList.find(serviceName);
    if (it == m_serviceList.end()) {
        throw RequestException(RPC_METHOD_NOT_FOUND, id, "service not found");
    }
    methodName = methodName.substr(pos + 1);
    if (methodName.length() == 0) {
        throw RequestException(RPC_INVALID_REQUEST, id, "missing method name in method");
    }
    it->second->callReturnProcedure(methodName, request, done);
}

void RpcServer::validateRequest(cppjson::Value& request) {
    auto& id = findValue<
            cppjson::TYPE_STRING,
            cppjson::TYPE_NULL,
            cppjson::TYPE_INT32,
            cppjson::TYPE_INT64>(request, "id");
    auto& version = findValue<cppjson::TYPE_STRING>(request, id, "jsonrpc");
    if (version.getString() != "2.0") {
        throw RequestException(RPC_INVALID_REQUEST, id, "jsonrpc version is unknown/unsupported");
    }
    auto& method = findValue<cppjson::TYPE_STRING>(request, id, "method");
    if (method.getString() == "rpc.") {//internal use
        throw RequestException(RPC_METHOD_NOT_FOUND, id, "method name is internal use");
    }
    size_t nMembers = 3 + hasParams(request);//jsonrpc, method, params, id
    if (request.getSize() != nMembers) {
        throw RequestException(RPC_INVALID_REQUEST, id, "unexpected field");
    }
}

void RpcServer::validateNotify(cppjson::Value& request) {
    auto& version = findValue<cppjson::TYPE_STRING>(request, "jsonrpc");
    if (version.getString() != "2.0") {
        throw NotifyException(RPC_INVALID_REQUEST, "jsonrpc version is unknown/unsupported");
    }
    auto& method = findValue<cppjson::TYPE_STRING>(request, "method");
    if (method.getString() == "rpc.") {//internal use
        throw NotifyException(RPC_METHOD_NOT_FOUND, "method name is internal use");
    }

    size_t nMembers = 2 + hasParams(request);//jsonrpc, method, params, no id
    if (request.getSize() != nMembers) {
        throw NotifyException(RPC_INVALID_REQUEST, "unexpected field");
    }
}

}
