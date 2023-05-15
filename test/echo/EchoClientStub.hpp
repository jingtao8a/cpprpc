
/*
 * This stub is generated by cpprpc, DO NOT modify it!
 */

#ifndef CPPRPC_ECHOCLIENTSTUB_HPP
#define CPPRPC_ECHOCLIENTSTUB_HPP

#include <cppjson/Value.hpp>

#include <cpprpc/utils.hpp>
#include <cpprpc/client/BaseClient.hpp>
#include <cppweb/Logger.hpp>
#include <cppweb/Singleton.hpp>

namespace CPPRPC {

static auto& logger = CPPWEB::Singleton<CPPWEB::Logger>::GetInstance();

class EchoClientStub: public CPPWEB::nocopyable{
public:
    EchoClientStub(CPPWEB::EventLoop* loop, const CPPWEB::InetAddress& serverAddress):
            m_client(loop, serverAddress)
    {
        m_client.setConnectionCallback([this](const CPPWEB::TCPConnectionPtr& conn){
            if (conn->isConnected()) {
                INFO(logger, "connected");
                m_conn = conn;
                m_cb(m_conn);
            } else {
                INFO(logger, "disconnected");
                assert(m_conn != nullptr);
                m_cb(m_conn);
            }
        });
    }

    void start() { m_client.start(); }

    void setConnectionCallback(const CPPWEB::ConnectionCallback& cb) {
        m_cb = cb;
    }

    
void Echo(std::string message, const ResponseCallback& cb)
{
    cppjson::Value params(cppjson::TYPE_OBJECT);
    
params.addMember(cppjson::Value("message"), cppjson::Value(message));


    cppjson::Value call(cppjson::TYPE_OBJECT);
    call.addMember(cppjson::Value("jsonrpc"), cppjson::Value("2.0"));
    call.addMember(cppjson::Value("method"), cppjson::Value("Echo.Echo"));
    call.addMember(cppjson::Value("params"), cppjson::Value(params));

    assert(m_conn != nullptr);
    m_client.sendCall(m_conn, call, cb);
}

    

private:
    CPPWEB::TCPConnectionPtr m_conn;
    CPPWEB::ConnectionCallback m_cb;
    BaseClient m_client;
};

}
#endif //CPPRPC_ECHOCLIENTSTUB_HPP
