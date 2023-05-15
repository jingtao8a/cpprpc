#ifndef CPPRPC_CLIENTSTUBGENERATOR_HPP
#define CPPRPC_CLIENTSTUBGENERATOR_HPP

#include <cpprpc/stub/StubGenerator.hpp>

namespace CPPRPC {
class ClientStubGenerator: public StubGenerator {
public:
    explicit ClientStubGenerator(cppjson::Value& proto): StubGenerator(proto) {}

    std::string genStub() override;
    std::string genStubClassName() override;

private:
    std::string genMacroName();

    std::string genProcedureDefinitions();
    std::string genNotifyDefinitions();

    template<typename Rpc>
    std::string genGenericArgs(const Rpc& r, bool appendComma);
    template<typename Rpc>
    std::string genGenericParamMembers(const Rpc& r);
};

}


#endif