#ifndef CPPRPC_BASECLIENT_HPP
#define CPPRPC_BASECLIENT_HPP

#include "../utils.hpp"
#include <cppweb/TCPClient.hpp>
#include <unordered_map>

namespace CPPRPC {
class BaseClient: public CPPWEB::nocopyable {
public:
    BaseClient(CPPWEB::EventLoop* loop, const CPPWEB::InetAddress& serverAddress):
        m_id(0),
        m_client(loop, serverAddress) {
        
        m_client.setMessageCallback(std::bind(&BaseClient::onMessage, this, CPPWEB::_1, CPPWEB::_2));
    }

    void start() { m_client.start(); }
    void setConnectionCallback(const CPPWEB::ConnectionCallback& cb) {
        m_client.setConnectionCallback(cb);
    }

    void sendCall(const CPPWEB::TCPConnectionPtr& conn, cppjson::Value& call, const ResponseCallback& cb);
    void sendNotify(const CPPWEB::TCPConnectionPtr& conn, cppjson::Value& notify);
private:
    void onMessage(const CPPWEB::TCPConnectionPtr& conn, CPPWEB::Buffer& buffer);
    void handleMessage(CPPWEB::Buffer& buffer);
    void handleResponse(std::string& json);
    void handleSingleResponse(cppjson::Value& response);
    void validateResponse(cppjson::Value& response);
    void sendRequest(const CPPWEB::TCPConnectionPtr& conn, cppjson::Value& request);
private:
    int64_t m_id;
    CPPWEB::TCPClient m_client;
    std::unordered_map<int64_t, ResponseCallback> m_callbacks;
};

}

#endif