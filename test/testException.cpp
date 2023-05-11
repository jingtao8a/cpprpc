#include "../src/Exception.hpp"
#include <iostream>
#include "../src/RpcError.hpp"

using namespace CPPRPC;


int main() {
    try {
        throw NotifyException(RpcError(RPC_PARSE_ERROR), "HELLO");
    } catch(NotifyException& e) {
        std::cout << e.what();
    }
    return 0;
}