#include "BaseClient.hpp"
#include "../Exception.hpp"
#include <cppjson/StringWriteStream.hpp>
#include <cppjson/Writer.hpp>
#include <cppjson/Document.hpp>
#include <cppweb/Logger.hpp>
#include <cppweb/Singleton.hpp>

namespace CPPRPC {

static const size_t kMaxMessageLen = 65536;
static auto& logger = CPPWEB::Singleton<CPPWEB::Logger>::GetInstance();

static cppjson::Value& findValue(cppjson::Value &value, const char *key, cppjson::ValueType type)
{
    auto it = value.findMember(key);
    if (it == value.getObject().end()) {
        throw ResponseException("missing at least one field");
    }
    if (it->m_value.getType() != type) {
        throw ResponseException("bad type of at least one field");
    }
    return it->m_value;
}

static cppjson::Value& findValue(cppjson::Value& value, const char* key, cppjson::ValueType type, int32_t id) {
    try {
        return findValue(value, key, type);
    }
    catch (ResponseException& e) {
        throw ResponseException(e.what(), id);
    }
}

void BaseClient::sendCall(const CPPWEB::TCPConnectionPtr& conn, cppjson::Value& call, const ResponseCallback& cb) {
    call.addMember(cppjson::Value("id"), cppjson::Value(m_id));
    m_callbacks[m_id] = cb;
    sendRequest(conn, call);
}

void BaseClient::sendNotify(const CPPWEB::TCPConnectionPtr& conn, cppjson::Value& notify) {
    sendRequest(conn, notify);
}

void BaseClient::sendRequest(const CPPWEB::TCPConnectionPtr& conn, cppjson::Value& request) {
    cppjson::StringWriteStream os;
    cppjson::Writer<cppjson::StringWriteStream> writer(os);

    writer.fromValue(request);
    auto message = std::to_string(os.get().length() + 2).append("\r\n").append(os.get()).append("\r\n");
    conn->send(message);
}

void BaseClient::onMessage(const CPPWEB::TCPConnectionPtr& conn, CPPWEB::Buffer& buffer) {
    try {
        handleMessage(buffer);
    } catch (ResponseException& e) {
        ERROR(logger, "response error: %s", e.what());
        if (e.hasId()) {
            m_callbacks.erase(e.getId());
        }
    }
}

void BaseClient::handleMessage(CPPWEB::Buffer& buffer) {
    while(true) {
        const char* crlf = buffer.findCRLF();
        if (crlf == nullptr) {
            break;
        }
        size_t headerLen = crlf - buffer.peek() + 2;

        cppjson::Document header;
        auto err = header.parse(buffer.peek(), headerLen);
        if (err != cppjson::PARSE_OK || !header.isInt32() || header.getInt32() <= 0) {
            // retrieve trash line for debugging
            buffer.retrieve(headerLen);
            throw ResponseException("invalid message length");
        }
        auto bodyLen = static_cast<uint32_t>(header.getInt32());
        if (bodyLen >= kMaxMessageLen) {
            throw ResponseException("message is too long");
        }
        
        buffer.retrieve(headerLen);
        auto json = buffer.retrieveAsString(bodyLen);
        handleResponse(json);
    }
}

void BaseClient::handleResponse(std::string& json) {
    cppjson::Document response;
    cppjson::ParseError err = response.parse(json);
    if (err != cppjson::PARSE_OK) {
        throw ResponseException(cppjson::ParseErrorStr(err));
    }

    switch (response.getType()) {
    case cppjson::TYPE_OBJECT:
        handleSingleResponse(response);
        break;
    case cppjson::TYPE_ARRAY:
        size_t n = response.getSize();
        if (n == 0) {
            throw ResponseException("batch response is empty");
        }
        for (size_t i = 0; i < n; ++i) {
            handleSingleResponse(response[i]);
        }
    default:
        break;
    }
}

void BaseClient::handleSingleResponse(cppjson::Value& response) {
    validateResponse(response);
    auto id = response["id"].getInt32();
    auto it = m_callbacks.find(id);
    if (it == m_callbacks.end()) {
        WARN(logger, "response %d not found in stub", id);
        return;
    }
    auto result = response.findMember("result");
    if (result != response.getObject().end()) {
        it->second(result->m_value, false, false);
    } else {
        auto error = response.findMember("error");
        assert(error != response.getObject().end());
        it->second(error->m_value, true, false);
    }
    m_callbacks.erase(it);
}

void BaseClient::validateResponse(cppjson::Value& response) {
    if (response.getSize() != 3) {
        throw ResponseException("response should have exactly 3 field (jsonrpc, error/result, id)");
    }

    auto id = findValue(response, "id", cppjson::TYPE_INT32).getInt32();

    auto version = findValue(response, "jsonrpc", cppjson::TYPE_STRING, id).getString();
    if (version != "2.0") {
        throw ResponseException("unknown json rpc version", id);
    }

    if (response.findMember("result") != response.getObject().end()) {
        return;
    }
    
    findValue(response, "error", cppjson::TYPE_OBJECT, id);
}

}