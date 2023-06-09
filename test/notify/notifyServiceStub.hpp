
/*
 * This stub is generated by cpprpc, DO NOT modify it!
 */
#ifndef CPPRPC_NOTIFYSERVICESTUB_HPP
#define CPPRPC_NOTIFYSERVICESTUB_HPP

#include <cppjson/Value.hpp>

#include <cpprpc/utils.hpp>
#include <cpprpc/server/RpcServer.hpp>
#include <cpprpc/server/RpcService.hpp>
#include <cppweb/nocopyable.hpp>

class notifyService;

namespace CPPRPC {

template <typename S>
class notifyServiceStub: public CPPWEB::nocopyable {
protected:
    explicit notifyServiceStub(RpcServer& server) {
        static_assert(std::is_same<S, notifyService>::value, "derived class name should be 'notifyService'");

        auto service = new RpcService;

        service->addNotifyProcedure("hello", new NotifyProcedure(
        std::bind(&notifyServiceStub::helloStub, this, CPPWEB::_1) , 
"xingming", cppjson::TYPE_STRING));
service->addNotifyProcedure("goodbye", new NotifyProcedure(
        std::bind(&notifyServiceStub::goodbyeStub, this, CPPWEB::_1) , 
"xingming", cppjson::TYPE_STRING));


        server.addService("notify", service);
    }

private:
    
void helloStub(cppjson::Value& request) {
    auto& params = request["params"];

    if (params.isArray()) {
        auto xingming = params[0].getString();

        convert().hello(xingming);
    } else {
        auto xingming = params["xingming"].getString();

        convert().hello(xingming);
    }
}


void goodbyeStub(cppjson::Value& request) {
    auto& params = request["params"];

    if (params.isArray()) {
        auto xingming = params[0].getString();

        convert().goodbye(xingming);
    } else {
        auto xingming = params["xingming"].getString();

        convert().goodbye(xingming);
    }
}



private:
    S& convert() {
        return static_cast<S&>(*this);
    }
};

}

#endif //CPPRPC_NOTIFYSERVICESTUB_HPP
