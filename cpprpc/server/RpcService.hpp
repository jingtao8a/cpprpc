#ifndef CPPRPC_RPCSERVICE_HPP
#define CPPRPC_RPCSERVICE_HPP

#include <cpprpc/server/NotifyProcedure.hpp>
#include <cpprpc/server/ReturnProcedure.hpp>
#include <cppweb/nocopyable.hpp>
#include <memory>
#include <unordered_map>

namespace CPPRPC {
class RpcService: public CPPWEB::nocopyable {
public:
    void addNotifyProcedure(std::string methodName, NotifyProcedure* p) {
        assert(m_notifyProcedureList.find(methodName) == m_notifyProcedureList.end());
        m_notifyProcedureList.emplace(methodName, p);
    }

    void addReturnProcedure(std::string methodName, ReturnProcedure* p) {
        assert(m_returnProcedureList.find(methodName) == m_returnProcedureList.end());
        m_returnProcedureList.emplace(methodName, p);
    }

    void callReturnProcedure(std::string methodName, cppjson::Value& request, const RpcDoneCallback& done);
    void callNotifyProcedure(std::string methodName, cppjson::Value& request);
private:
    typedef std::unique_ptr<ReturnProcedure> ReturnProcedurePtr;
    typedef std::unique_ptr<NotifyProcedure> NotifyProcedurePtr;

    std::unordered_map<std::string, ReturnProcedurePtr> m_returnProcedureList;
    std::unordered_map<std::string, NotifyProcedurePtr> m_notifyProcedureList;
};

}

#endif