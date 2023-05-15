#include <cpprpc/server/RpcService.hpp>
#include <cpprpc/Exception.hpp>

namespace CPPRPC {

void RpcService::callReturnProcedure(std::string methodName, cppjson::Value& request, const RpcDoneCallback& done) {
    auto it = m_returnProcedureList.find(methodName);
    if (it == m_returnProcedureList.end()) {
        throw RequestException(RPC_METHOD_NOT_FOUND, request["id"], "method not found");
    }
    it->second->invoke(request, done);
}

void RpcService::callNotifyProcedure(std::string methodName, cppjson::Value& request) {
    auto it = m_notifyProcedureList.find(methodName);
    if (it == m_notifyProcedureList.end()) {
        throw NotifyException(RPC_METHOD_NOT_FOUND, "method not found");
    }
    it->second->invoke(request);
}

}