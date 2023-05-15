#include <cpprpc/server/NotifyProcedure.hpp>
#include <cpprpc/Exception.hpp>

namespace CPPRPC{

void NotifyProcedure::invoke(cppjson::Value& request) {
    if (!validateGeneric(request)) {
        throw NotifyException(RPC_INVALID_PARAMS, "params name or type mismatch");       
    }
    m_callback(request);
}

}