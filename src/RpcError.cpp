#include "RpcError.hpp"

namespace CPPRPC {

Error RpcError::fromErrorCode(int32_t code) {
    switch (code) {
        case -32700: return RPC_PARSE_ERROR;
        case -32600: return RPC_INVALID_REQUEST;
        case -32601: return RPC_METHOD_NOT_FOUND;
        case -32602: return RPC_INVALID_PARAMS;
        case -32603: return RPC_INTERNAL_ERROR;
        default: assert(false && "bad error code");
    }
}

const int32_t RpcError::errorCode[] = {
#define GEN_ERROR_CODE(e, c, n) c,
    ERROR_MAP(GEN_ERROR_CODE)
#undef GEN_ERROR_CODE
};

const char* RpcError::errorString[] = {
#define GEN_STRERR(e, c, n) n,
    ERROR_MAP(GEN_STRERR)
#undef GEN_STRERR
};


}