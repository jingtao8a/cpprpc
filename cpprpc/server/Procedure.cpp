#include "Procedure.hpp"

namespace CPPRPC {

bool Procedure::validateGeneric(cppjson::Value& reqeust) const{
    auto mIter = reqeust.findMember("params");
    if (mIter == reqeust.getObject().end()) {
        return m_params.empty();
    }
    auto& params = mIter->m_value;
    if (params.getSize() == 0 || params.getSize() != m_params.size()) {
        return false;
    }

    switch(params.getType()) {
        case cppjson::TYPE_ARRAY:
            for (size_t i = 0; i < m_params.size(); ++i) {
                if (params[i].getType() != m_params[i].m_paramType) {
                    return false;
                }
            }
            break;
        case cppjson::TYPE_OBJECT:
            for (auto& p : m_params) {
                auto it = params.findMember(p.m_paramName);
                if (it == params.getObject().end()) {
                    return false;
                }
                if (it->m_value.getType() != p.m_paramType) {
                    return false;
                }
            }
            break;
        default:
            return false;
    }
    return true;
}

}