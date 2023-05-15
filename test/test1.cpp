#include <cppweb/ThreadPool.hpp>
#include <cppweb/Logger.hpp>
#include <cppweb/Singleton.hpp>

using namespace CPPWEB;

static auto& logger = Singleton<Logger>::GetInstance();

int main() {
    INFO(logger, "hello jsonrpc");
    return 0;
}