#ifndef CPPRPC_RPCERROR_HPP
#define CPPRPC_RPCERROR_HPP

#include <assert.h>
#include <stdint.h>
namespace CPPRPC {

#define ERROR_MAP(XX) \
  XX(PARSE_ERROR, -32700, "Parse error") \
  XX(INVALID_REQUEST, -32600, "Invalid request") \
  XX(METHOD_NOT_FOUND, -32601,"Method not found") \
  XX(INVALID_PARAMS, -32602, "Invalid params") \
  XX(INTERNAL_ERROR, -32603, "Internal error") \

enum Error {
#define GEN_ERRNO(e, c, s) RPC_##e,
    ERROR_MAP(GEN_ERRNO)
#undef GEN_ERRNO
};

class RpcError {
public:
    RpcError(Error err): m_err(err) {}
    explicit RpcError(int32_t errorCode): m_err(fromErrorCode(errorCode)) {}
    const char* asString() const { return errorString[m_err]; }
    int32_t asCode() const { return errorCode[m_err]; }
private:
    static Error fromErrorCode(int32_t code);
private:
    Error m_err;
    static const int32_t errorCode[];
    static const char* errorString[];
};




}

#endif