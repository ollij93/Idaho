#pragma once

// Includes...
#include "Object.h"
#include "Core/Types.h"

class Light : public Object {
public:
    Light(rp3d::CollisionWorld &xWorld, u_int uGUID)
        : PARENT(xWorld, uGUID)
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

    // Static Getters & Setters...
    static Light* GetActive() { return s_pxActive; }
    static void SetActive(Light* pxActive) { s_pxActive = pxActive; }

private:
    Color m_xDiffuseColor;
    Color m_xAmbientColor;

    // Statics...
    static Light* s_pxActive;

    typedef Object PARENT;
};

