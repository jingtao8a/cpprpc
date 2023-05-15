#ifndef CPPRPC_PROCEDURE_HPP
#define CPPRPC_PROCEDURE_HPP

#include <cppjson/Value.hpp>
#include <cppweb/nocopyable.hpp>
#include <string>
#include <vector>

namespace CPPRPC {

class Procedure: CPPWEB::nocopyable {
public:
    template<typename... ParamNameAndTypes>
    explicit Procedure(ParamNameAndTypes&&... nameAndTypes) {
        constexpr int n = sizeof...(nameAndTypes);
        static_assert(n % 2 == 0, "procedure must have param name and type pairs");
        if (n > 0) {
            initProcedure(nameAndTypes...);
        }
    }

private:
    template<typename Name, typename... ParamNameAndTypes>
    void initProcedure(Name paramName, cppjson::ValueType paramType, ParamNameAndTypes&& ...nameAndTypes) {
        static_assert(std::is_same<Name, const char*>::value || std::is_same<Name, std::string>::value, "param name must be \'const char*\' or \'std::string_view\'");
        m_params.emplace_back(paramName, paramType);
        if (sizeof...(nameAndTypes) > 0) {
            initProcedure(nameAndTypes...);
        }
    }
    
    void initProcedure() {}
protected:
    bool validateGeneric(cppjson::Value& reqeust) const;

protected:
    struct Param {
        Param(std::string paramName, cppjson::ValueType paramType): m_paramName(paramName), m_paramType(paramType) {}
        std::string m_paramName;
        cppjson::ValueType m_paramType;
    };

protected:
    std::vector<Param> m_params;
};

}

#endif