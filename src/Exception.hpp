#ifndef CPPRPC_EXCEPTION_HPP
#define CPPRPC_EXCEPTION_HPP

#include <exception>
#include <memory>
#include "RpcError.hpp"
#include "cppjson/src/Value.hpp"

namespace CPPRPC {

class NotifyException: public std::exception {
public:
    explicit NotifyException(RpcError err, const char* detail):
        m_err(err),
        m_detail(detail) {}

    const char* what() const noexcept { return m_err.asString(); }
    RpcError getErr() const { return m_err; }
    const char* getDetail() const { return m_detail; }
private:
    RpcError m_err;
    const char* m_detail;
};

class RequestException: public std::exception {
public:
    RequestException(RpcError err, cppjson::Value id, const char* detail):
        m_err(err), m_id(new cppjson::Value(id)), m_detail(detail) {}
    
    explicit RequestException(RpcError err, const char* detail):
        m_err(err), m_id(new cppjson::Value(cppjson::TYPE_NULL)), m_detail(detail) {}

    const char* what() const noexcept { return m_err.asString(); }
    RpcError getErr() const { return m_err; }
    const char* getDetail() const { return m_detail; }
    cppjson::Value& getId() { return *m_id; }

private:
    RpcError m_err;
    std::unique_ptr<cppjson::Value> m_id;
    const char* m_detail;
};

class ResponseException: public std::exception {
public:
    explicit ResponseException(const char* msg):
        m_hasId(false), m_id(-1), m_msg(msg) {}
    
    ResponseException(const char* msg, int32_t id):
        m_hasId(false), m_id(id), m_msg(msg) {}

    const char* what() const noexcept { return m_msg; }
    bool hasId() const { return m_hasId; }
    int32_t getId() const { return m_id; }

private:
    const bool m_hasId;
    const int32_t m_id;
    const char* m_msg;
};

class StubException: std::exception {
public:
    explicit StubException(const char* msg): m_msg(msg) {}

    const char* what() const noexcept { return m_msg; }
private:
    const char* m_msg;
};


}


#endif