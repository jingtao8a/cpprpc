#ifndef CPPRPC_STUBGENERATOR_HPP
#define CPPRPC_STUBGENERATOR_HPP
#include "cppjson/Value.hpp"

namespace CPPRPC {
class StubGenerator {
public:
    explicit StubGenerator(cppjson::Value& proto) { parseProto(proto); }

    virtual std::string genStub() = 0;
    virtual std::string genStubClassName() = 0;
private:
    void parseProto(cppjson::Value& proto);
    void parseRpc(cppjson::Value& rpc);
    void validateParams(cppjson::Value& params);
    void validateReturns(cppjson::Value& returns);
protected:
    struct RpcReturn {
        RpcReturn(const std::string& name, cppjson::Value& params, cppjson::Value& returns):
            m_name(name), m_params(params), m_returns(returns) {}

        std::string m_name;
        mutable cppjson::Value m_params;
        mutable cppjson::Value m_returns;
    };

    struct RpcNotify {
        RpcNotify(const std::string& name, cppjson::Value& params):
            m_name(name), m_params(params) {}
        
        std::string m_name;
        mutable cppjson::Value m_params;
    };

    struct ServiceInfo {
        std::string m_name;
        std::vector<RpcReturn> m_rpcReturn;
        std::vector<RpcNotify> m_rpcNotify;
    };

protected:
    ServiceInfo m_serviceInfo;
};

void replaceAll(std::string& str, const std::string& from, const std::string& to);

}

#endif