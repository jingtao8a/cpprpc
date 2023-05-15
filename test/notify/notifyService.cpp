#include "notifyServiceStub.hpp"
#include <cppweb/EventLoop.hpp>
#include <iostream>
class notifyService: public CPPRPC::notifyServiceStub<notifyService> {
public:
    notifyService(CPPRPC::RpcServer& rpcServer):notifyServiceStub(rpcServer) {}

    void hello(std::string str) {
        std::cout << "hello " << str<<std::endl;
    }

    void goodbye(std::string str) {
        std::cout << "goodbye " << str<<std::endl;
    }
};

int main() {
    CPPWEB::EventLoop loop;
    CPPWEB::InetAddress listen(9099);
    CPPRPC::RpcServer rpcServer(&loop, listen);
    notifyService server(rpcServer);
    rpcServer.start();
    loop.loop();
    return 0;
}