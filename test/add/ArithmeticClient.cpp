#include "ArithmeticClientStub.hpp"
#include <cppweb/EventLoop.hpp>
#include <iostream>
void responseCallback(cppjson::Value& response, bool isError, bool isTimeout){
    if (isError) {
        std::cout << response["message"].getString() << std::endl;
    } else {
        std::cout << response.getInt32() << std::endl;
    }
}

int main() {
    CPPWEB::EventLoop loop;
    CPPWEB::InetAddress addr(9099);
    CPPRPC::ArithmeticClientStub client(&loop, addr);
    client.setConnectionCallback([&](CPPWEB::TCPConnectionPtr conn) {
        if (conn->isDisconnected()) {
            loop.quit();
        } else {
            client.add(193, 7, responseCallback);
            client.add(200, 9, responseCallback);
        }
    });
    client.start();
    loop.loop();
    return 0;
}