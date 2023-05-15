#include <test/echo/EchoClientStub.hpp>
#include <iostream>
#include <cppweb/EventLoop.hpp>

void run(CPPRPC::EchoClientStub& client) {
    static int counter = 0;
    counter++;

    std::string str = "苟利国家生死以+" + std::to_string(counter) + "s";
    client.Echo(str, [](cppjson::Value response, bool isError, bool timeout) {
        if (!isError) {
            std::cout << "response: " << response.getString() << "\n";
        }
        else if (timeout) {
            std::cout << "timeout\n";
        } else {
            std::cout << "response: "
                      << response["message"].getString() << ": "
                      << response["data"].getString() << "\n";
        }
    });
}

int main() {
    CPPWEB::EventLoop loop;
    CPPWEB::InetAddress serverAddr(9877);
    CPPRPC::EchoClientStub client(&loop, serverAddr);

    client.setConnectionCallback([&](const CPPWEB::TCPConnectionPtr& conn) {
        if (conn->isDisconnected()) {
            loop.quit();
        }
        else {
            run(client);
        }
    });

    client.start();
    loop.loop();
}

