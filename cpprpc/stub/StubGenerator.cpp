#include "StubGenerator.hpp"
#include "../Exception.hpp"
#include <unordered_set>

namespace CPPRPC {

void expect(bool result, const char* errMsg) {
    if (!result) { throw StubException(errMsg); }
}

void StubGenerator::parseProto(cppjson::Value& proto) {
    expect(proto.isObject(), "expect object");
    expect(proto.getSize() == 2, "expect 'name' and 'rpc' fields in object");
    auto name = proto.findMember("name");
    expect(name != proto.getObject().end(), "missing service name");
    expect(name->m_value.isString(), "service name must be string");
    m_serviceInfo.m_name = name->m_value.getString();

    auto rpc = proto.findMember("rpc");
    expect(rpc != proto.getObject().end(), "missing service rpc definition");
    expect(rpc->m_value.isArray(), "rpc field must be array");

    size_t n = rpc->m_value.getSize();
    for (size_t i = 0; i < n; ++i) {
        parseRpc(rpc->m_value[i]);
    }
}

void StubGenerator::parseRpc(cppjson::Value& rpc) {
    expect(rpc.isObject(), "rpc definition must be object");
    auto name = rpc.findMember("name");
    expect(name != rpc.getObject().end(), "missing name in rpc definition");
    expect(name->m_value.isString(), "rpc name must be string");
    auto params = rpc.findMember("params");
    bool hasParams = (params != rpc.getObject().end());
    if (hasParams) {
        validateParams(params->m_value);
    }
    auto returns = rpc.findMember("returns");
    bool hasReturns = (returns != rpc.getObject().end());
    if (hasReturns) {
        validateReturns(returns->m_value);
    }

    auto paramsValue = hasParams ? params->m_value : cppjson::Value(cppjson::TYPE_OBJECT);
    if (hasReturns) {
        RpcReturn r(name->m_value.getString(), paramsValue, returns->m_value);
        m_serviceInfo.m_rpcReturn.push_back(r);
    } else {
        RpcNotify r(name->m_value.getString(), paramsValue);
        m_serviceInfo.m_rpcNotify.push_back(r);
    }
}

void StubGenerator::validateParams(cppjson::Value& params) {
    std::unordered_set<std::string> set;
    for (auto& p : params.getObject()) {
        auto key = p.m_key.getString();
        auto unique = set.insert(key).second;
        expect(unique, "duplicate param name");

        switch(p.m_value.getType()) {
            case cppjson::TYPE_NULL:
                expect(false, "bad param type");
                break;
            default:
                break;
        }
    }
}

void StubGenerator::validateReturns(cppjson::Value& returns) {
    switch (returns.getType()) {
        case cppjson::TYPE_NULL:
        case cppjson::TYPE_ARRAY:
            expect(false, "bad returns type");
            break;
        case cppjson::TYPE_OBJECT:
            validateParams(returns);
            break;
        default:
            break;
    }
}


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    while (true) {
        size_t i = str.find(from);
        if (i != std::string::npos) {
            str.replace(i, from.size(), to);
        }
        else return;
    }
}
}