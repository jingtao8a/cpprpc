#include "ArithmeticServiceStub.hpp"
#include <cppweb/EventLoop.hpp>

class ArithmeticService: public CPPRPC::ArithmeticServiceStub<ArithmeticService> {
public:
    ArithmeticService(CPPRPC::RpcServer& rpcServer):ArithmeticServiceStub(rpcServer) {}

    void add(int32_t left_value, int32_t right_value, const CPPRPC::UserDoneCallback& done) {
        auto result = left_value + right_value;
        done(cppjson::Value(result));
    }
};

int main() {
    CPPWEB::EventLoop loop;
    CPPWEB::InetAddress listen(9099);
    CPPRPC::RpcServer rpcServer(&loop, listen);
    ArithmeticService server(rpcServer);
    rpcServer.start();
    loop.loop();
    return 0;
}