#ifndef CPPRPC_RPCSERVER_HPP
#define CPPRPC_RPCSERVER_HPP

#include "BaseServer.hpp"
#include "RpcService.hpp"

namespace CPPRPC {

class RpcServer: public BaseServer {
public:
    RpcServer(CPPWEB::EventLoop* loop, const CPPWEB::InetAddress& listen): BaseServer(loop, listen) {}

    void addService(std::string serviceName, RpcService* service);
    void handleRequest(const std::string& json, const RpcDoneCallback& done) override;

private:
    void handleSingleRequest(cppjson::Value& request, const RpcDoneCallback& done);
    void handleBatchRequests(cppjson::Value& requests, const RpcDoneCallback& done);
    void handleSingleNotify(cppjson::Value& request);

    void validateRequest(cppjson::Value& request);
    void validateNotify(cppjson::Value& request);

private:
    typedef std::unique_ptr<RpcService> RpcServicePtr;
    std::unordered_map<std::string, RpcServicePtr> m_serviceList;
};

}

#endif