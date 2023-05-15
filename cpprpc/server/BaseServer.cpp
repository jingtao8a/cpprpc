#include <cpprpc/server/BaseServer.hpp>
#include <cppweb/TCPClient.hpp>
#include <cppweb/Logger.hpp>
#include <cppweb/Singleton.hpp>
#include <cpprpc/Exception.hpp>
#include <cppjson/Document.hpp>
#include <cppjson/StringWriteStream.hpp>
#include <cppjson/Writer.hpp>

namespace CPPRPC {

static auto& logger = CPPWEB::Singleton<CPPWEB::Logger>::GetInstance();
static const size_t kMaxMessageLen = 100 * 1024 * 1024;

BaseServer::BaseServer(CPPWEB::EventLoop* eventLoop, const CPPWEB::InetAddress& local):
    m_server(eventLoop, local) {
    m_server.setConnectionCallback(std::bind(&BaseServer::onConnection, this, CPPWEB::_1));
    m_server.setMessageCallback(std::bind(&BaseServer::onMessage, this, CPPWEB::_1, CPPWEB::_2));
}

cppjson::Value BaseServer::wrapException(RequestException& e) {
    cppjson::Value response(cppjson::TYPE_OBJECT);
    response.addMember(cppjson::Value("jsonrpc"), cppjson::Value("2.0"));
    
    cppjson::Value internalValue(cppjson::TYPE_OBJECT);
    internalValue.addMember(cppjson::Value("code"), cppjson::Value(e.getErr().asCode()));
    internalValue.addMember(cppjson::Value("message"), cppjson::Value(e.getErr().asString()));
    internalValue.addMember(cppjson::Value("data"), cppjson::Value(e.getDetail()));

    response.addMember(cppjson::Value("error"), internalValue);
    response.addMember(cppjson::Value("id"), e.getId());
    return response;
}

void BaseServer::onConnection(const CPPWEB::TCPConnectionPtr& conn) {
    if (conn->isConnected()) {
        DEBUG(logger, "connection %s is [up]", conn->peer().toIpPort().c_str());
    } else {
        DEBUG(logger, "connection %s is [down]", conn->peer().toIpPort().c_str());
    }
}

void BaseServer::onMessage(const CPPWEB::TCPConnectionPtr& conn, CPPWEB::Buffer& buffer) {
    try {
        handleMessage(conn, buffer);
    } catch (RequestException& e) {//发回客户端
        cppjson::Value response = wrapException(e);
        sendResponse(conn, response);
        conn->shutdown();
        WARN(logger, "BaseServer::onMessage() %s request error: %s", conn->peer().toIpPort().c_str(), e.what());
    } catch (NotifyException& e) {//直接在服务端notify
        WARN(logger, "BaseServer::onMessage() %s notify error: %s", conn->peer().toIpPort().c_str(), e.what());
    }
}

void BaseServer::onHighWatermark(const CPPWEB::TCPConnectionPtr& conn, size_t mark) {}
void BaseServer::onWriteComplete(const CPPWEB::TCPConnectionPtr& conn) {}

void BaseServer::handleMessage(const CPPWEB::TCPConnectionPtr& conn, CPPWEB::Buffer& buffer) {
    while (true) {

        const char *crlf = buffer.findCRLF();
        if (crlf == nullptr)
            break;
        if (crlf == buffer.peek()) {
            buffer.retrieve(2);
            break;
        }

        size_t headerLen = crlf - buffer.peek() + 2;
        cppjson::Document header;
        auto err = header.parse(buffer.peek(), headerLen);
        if (err != cppjson::PARSE_OK || !header.isInt32() || header.getInt32() <= 0) {
            throw NotifyException(RPC_INVALID_REQUEST, "invalid message length");
        }

        auto jsonLen = static_cast<uint32_t>(header.getInt32());

        if (jsonLen >= kMaxMessageLen) {
            throw NotifyException(RPC_INVALID_REQUEST, "message is too long");
        }
        if (buffer.readableBytes() < headerLen + jsonLen) {
            break;
        }

        buffer.retrieve(headerLen);
        auto json = buffer.retrieveAsString(jsonLen);

        //处理request的json
        handleRequest(json, [conn, this](cppjson::Value response) { 
            if (!response.isNull()) {
                this->sendResponse(conn, response);
                TRACE(logger, "BaseServer::handleMessage() %s request success", conn->peer().toIpPort().c_str());
            } else {
                TRACE(logger, "BaseServer::handleMessage() %s notify success", conn->peer().toIpPort().c_str());
            }
        });
    }
}

void BaseServer::sendResponse(const CPPWEB::TCPConnectionPtr& conn, const cppjson::Value& response) {
    cppjson::StringWriteStream os;
    cppjson::Writer<cppjson::StringWriteStream> writer(os);
    writer.fromValue(const_cast<cppjson::Value&>(response));

    auto message = std::to_string(os.get().length() + 2).append("\r\n").append(os.get()).append("\r\n");
    conn->send(message);
}

}