#pragma once

// Includes...
#include "Object.h"
#include "Core/Types.h"

class Light : public Object {
public:
    Light(Scene &xScene, u_int uGUID)
        : PARENT(xScene, uGUID)
        , m_xDiffuseColor()
        , m_xAmbientColor()
    {
    }
    ~Light() {}

    // Overrides...
    virtual void InitFromSpecification(const Specification* pxSpecification) override {}
    
    // Getters & Setters...
    Color GetDiffuseColor() const { return m_xDiffuseColor; }
    void SetDiffuseColor(Color xColor) { m_xDiffuseColor = xColor; }
    Color GetAmbientColor() const { return m_xAmbientColor; }
    void SetAmbientColor(Color xColor) { m_xAmbientColor = xColor; }

private:
    Color m_xDiffuseColor;
    Color m_xAmbientColor;

    typedef Object PARENT;
};

