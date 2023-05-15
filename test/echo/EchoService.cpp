#include <test/echo/EchoServiceStub.hpp>
#include <cppweb/EventLoop.hpp>

class EchoService: public CPPRPC::EchoServiceStub<EchoService> {
public:
    explicit EchoService(CPPRPC::RpcServer& server): EchoServiceStub(server) {}

    void Echo(std::string message, const CPPRPC::UserDoneCallback& done) {
        done(cppjson::Value(message));
    }
};


int main() {
    CPPWEB::EventLoop loop;
    CPPWEB::InetAddress listen(9877);
    CPPRPC::RpcServer rpcServer(&loop, listen);

    EchoService server(rpcServer);

    rpcServer.start();
    loop.loop();
}