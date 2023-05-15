#include <cpprpc/server/ReturnProcedure.hpp>
#include <cpprpc/Exception.hpp>

namespace CPPRPC {
void ReturnProcedure::invoke(cppjson::Value& request, const RpcDoneCallback& done) {
    if (!validateGeneric(request)) {
        throw RequestException(RPC_INVALID_PARAMS, request["id"], "params name or type mismatch");
    }
    m_callback(request, done);
}

}