#ifndef CPPRPC_NOTIFYPROCEDURE_HPP
#define CPPRPC_NOTIFYPROCEDURE_HPP

#include "../utils.hpp"
#include "Procedure.hpp"

namespace CPPRPC {
class NotifyProcedure: public Procedure {
public:
    template<typename... ParamNameAndTypes>
    explicit NotifyProcedure(const ProcedureNotifyCallback& cb, ParamNameAndTypes&&... nameAndTypes):
            Procedure(nameAndTypes),
            m_callback(cb) {}

    void invoke(cppjson::Value& request);

private:
    ProcedureNotifyCallback m_callback;
};

}


#endif