#ifndef CPPRPC_SERVICESTUBGENERATOR_HPP
#define CPPRPC_SERVICESTUBGENERATOR_HPP

#include <cpprpc/stub/StubGenerator.hpp>

namespace CPPRPC {

class ServiceStubGenerator:public StubGenerator {
public:
    explicit ServiceStubGenerator(cppjson::Value& proto): StubGenerator(proto) {}

    std::string genStub() override;
    std::string genStubClassName() override;

private:
    std::string genMacroName();
    std::string genUserClassName();
    std::string genStubProcedureBindings();
    std::string genStubProcedureDefinitions();
    std::string genStubNotifyBindings();
    std::string genStubNotifyDefinitions();

    template <typename Rpc>
    std::string genStubGenericName(const Rpc& r);
    template <typename Rpc>
    std::string genGenericParams(const Rpc& r);
    template <typename Rpc>
    std::string genGenericArgs(const Rpc& r);

    template <typename Rpc>
    std::string genParamsFromJsonArray(const Rpc& r);
    template <typename Rpc>
    std::string genParamsFromJsonObject(const Rpc& r);
};

}

#endif