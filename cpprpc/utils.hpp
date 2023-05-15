#ifndef CPPRPC_UTILS_HPP
#define CPPRPC_UTILS_HPP

#include <functional>
#include <cppjson/Value.hpp>

namespace CPPRPC {

typedef std::function<void(cppjson::Value)> RpcDoneCallback;
typedef std::function<void(cppjson::Value&, const RpcDoneCallback&)> ProcedureReturnCallback;
typedef std::function<void(cppjson::Value&)> ProcedureNotifyCallback;
typedef std::function<void(cppjson::Value&, bool isError, bool isTimeout)> ResponseCallback;
 
class UserDoneCallback {
public:
    UserDoneCallback(cppjson::Value& request, const RpcDoneCallback& callback):m_request(request), m_callback(callback) {}

    void operator()(cppjson::Value&& result) const {
        cppjson::Value response(cppjson::TYPE_OBJECT);
        response.addMember(cppjson::Value("jsonrpc"), cppjson::Value("2.0"));
        response.addMember(cppjson::Value("id"), m_request["id"]);
        response.addMember(cppjson::Value("result"), result);
        m_callback(response);
    }
private:
    mutable cppjson::Value m_request;
    RpcDoneCallback m_callback;
};

}

#endif