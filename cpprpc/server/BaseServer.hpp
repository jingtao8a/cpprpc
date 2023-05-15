#ifndef CPPRPC_BASESERVER_HPP
#define CPPRPC_BASESERVER_HPP    

#include <cppweb/TCPServer.hpp>
#include "../utils.hpp"

namespace CPPRPC {

class RequestException;

class BaseServer: CPPWEB::nocopyable {
public:
    BaseServer(CPPWEB::EventLoop* eventLoop, const CPPWEB::InetAddress& local);

    void setNumThread(size_t n) { m_server.setThreadNum(n); }
    void start() { m_server.start(); }
    virtual void handleRequest(const std::string& json, const RpcDoneCallback& done) = 0;

protected:
    cppjson::Value wrapException(RequestException& e);

private:
    void onConnection(const CPPWEB::TCPConnectionPtr& conn);
    void onMessage(const CPPWEB::TCPConnectionPtr& conn, CPPWEB::Buffer& buffer);
    void onHighWatermark(const CPPWEB::TCPConnectionPtr& conn, size_t mark);
    void onWriteComplete(const CPPWEB::TCPConnectionPtr& conn);
    
    void handleMessage(const CPPWEB::TCPConnectionPtr& conn, CPPWEB::Buffer& buffer);
    void sendResponse(const CPPWEB::TCPConnectionPtr& conn, const cppjson::Value& response);

private:
    CPPWEB::TCPServer m_server;
};

};

#endif