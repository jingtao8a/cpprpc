#include "cppweb/src/ThreadPool.hpp"
#include "cppweb/src/Logger.hpp"
#include "cppweb/src/Singleton.hpp"

using namespace CPPWEB;

static auto& logger = Singleton<Logger>::GetInstance();

int main() {
    INFO(logger, "hello jsonrpc");
    return 0;
}