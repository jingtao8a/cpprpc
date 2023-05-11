#include "../src/RpcError.hpp"
#include <iostream>

using namespace CPPRPC;
using namespace std;

int main() {
    RpcError error(RPC_METHOD_NOT_FOUND);
    cout << error.asCode() << error.asString() << endl;
    return 0;
}