#include "notifyClientStub.hpp"
#include <iostream>
#include <cppweb/EventLoop.hpp>


int main() {
    CPPWEB::EventLoop loop;
    CPPWEB::InetAddress serverAddr(9099);
    CPPRPC::notifyClientStub client(&loop, serverAddr);

    client.setConnectionCallback([&](const CPPWEB::TCPConnectionPtr& conn) {
        if (conn->isDisconnected()) {
            loop.quit();
        }
        else {
            client.hello("yuxintao");
            client.goodbye("yuxintao");
        }
    });

    client.start();
    loop.loop();
}
