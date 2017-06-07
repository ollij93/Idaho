#pragma once

// External Includes...
#include "TinyXML/tinyxml2.h"

// Includes...
#include "SpecificationSystem.h"
#include "Core/Types.h"

enum SpecificationParameterType {
    PARAM_TYPE_HASH = 0,
    PARAM_TYPE_INT = 1,
    PARAM_TYPE_UINT = 2,
    PARAM_TYPE_FLOAT = 3,
    PARAM_TYPE_BOOL = 4,
};

struct SpecificationParameter {
    Hash m_uHash;
    SpecificationParameterType m_eType;
    union {
        int iInt;
        operator int() const { return iInt; }
        u_int uInt;
        operator u_int() const { return uInt; }
        float fFloat;
        operator float() const { return fFloat; }
        bool bBool;
        operator bool() const { return bBool; }
    } m_UValue ;
};

class Specification {
public:
    Specification()
        : m_vxParameters()
        , m_uHash(uHASH_UNSET)
    {
    }
    ~Specification() {}

    void Shutdown();

    template<typename T> T GetValue(Hash uParameterHash, T tDefaultValue) const;

private:
    bool LoadParamsFromElement(const tinyxml2::XMLElement* pxElement);

    const SpecificationParameter* GetParameterFromHash(Hash uHash) const;

    std::vector<SpecificationParameter> m_vxParameters;
    Hash m_uHash;

    friend SpecificationSystem;
};

