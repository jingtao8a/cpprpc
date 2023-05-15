#ifndef CPPRPC_RETURNPROCEDURE_HPP
#define CPPRPC_RETURNPROCEDURE_HPP

#include <cpprpc/utils.hpp>
#include <cpprpc/server/Procedure.hpp>

namespace CPPRPC {

class ReturnProcedure: public Procedure {
public:
    template<typename... ParamNameAndTypes>
    ReturnProcedure(const ProcedureReturnCallback& cb, ParamNameAndTypes&&... nameAndTypes):
        Procedure(nameAndTypes...), m_callback(cb) {}

    void invoke(cppjson::Value& request, const RpcDoneCallback& done);

private:
    ProcedureReturnCallback m_callback;
};

}


#endif