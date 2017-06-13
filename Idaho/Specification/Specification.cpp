// Includes...
#include "Specification.h"
#include "Core/Assert.h"

void
Specification::Shutdown()
{
    m_vxParameters.clear();
}

template<typename T>
T
Specification::GetValue(Hash uParamHash, T tDefaultValue) const
{
    const SpecificationParameter* pxParam = GetParameterFromHash(uParamHash);
    if (pxParam) {
        return (T)pxParam->m_UValue;
    } else {
        return tDefaultValue;
    }
}

template int Specification::GetValue<int>(Hash uParamHash, int tDefaultValue) const;
template u_int Specification::GetValue<u_int>(Hash uParamHash, u_int tDefaultValue) const;
template float Specification::GetValue<float>(Hash uParamHash, float tDefaultValue) const;
template bool Specification::GetValue<bool>(Hash uParamHash, bool tDefaultValue) const;

bool
Specification::LoadParamsFromElement(const tinyxml2::XMLElement* pxElement)
{
    ASSERT(pxElement, "Null element passed to specification for loading.");
    if (!pxElement) { return false; }

    const char* pszName = pxElement->Attribute("name");
    ASSERT(pszName, "Failed to get the name of an element for loading specification");
    if (!pszName) { return false; }

    m_uHash = GetHash(pszName);
    ASSERT(m_uHash != uHASH_UNSET, "Failed to GetHash of specification name.");
    if (m_uHash == uHASH_UNSET) { return false; }

    const tinyxml2::XMLElement* pxParamElement = pxElement->FirstChildElement("Parameter");
    while (pxParamElement) {
        const char* pszParamName = pxParamElement->Attribute("name");
        if (pszParamName) {
            SpecificationParameter xParam;
            xParam.m_uHash = GetHash(pszParamName);

            xParam.m_eType = static_cast<SpecificationParameterType>(pxParamElement->IntAttribute("type"));

            switch (xParam.m_eType) {
                case PARAM_TYPE_HASH:
                {
                    xParam.m_UValue.uInt = GetHash(pxParamElement->Attribute("value"));
                    break;
                }
                case PARAM_TYPE_INT:
                {
                    xParam.m_UValue.iInt = pxParamElement->IntAttribute("value");
                    break;
                }
                case PARAM_TYPE_UINT:
                {
                    xParam.m_UValue.uInt = pxParamElement->UnsignedAttribute("value");
                    break;
                }
                case PARAM_TYPE_FLOAT:
                {
                    xParam.m_UValue.fFloat = pxParamElement->FloatAttribute("value");
                    break;
                }
                case PARAM_TYPE_BOOL:
                {
                    xParam.m_UValue.bBool = pxParamElement->BoolAttribute("value");
                    break;
                }
                default:
                    break;
            }

            m_vxParameters.push_back(xParam);
        }
        pxParamElement = pxParamElement->NextSiblingElement("Parameter");
    }

    return true;
}

const SpecificationParameter*
Specification::GetParameterFromHash(Hash uParamHash) const
{
    for (u_int u = 0; u < m_vxParameters.size(); u++) {
        if (m_vxParameters[u].m_uHash == uParamHash) {
            return &m_vxParameters[u];
        }
    }
    return nullptr;
}
