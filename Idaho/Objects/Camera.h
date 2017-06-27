#pragma once

// External Includes...
#include <DirectXMath.h>

// Includes...
#include "Object.h"

__declspec(align(16))
class Camera : public Object {
public:
    Camera(Scene &xScene, u_int uGUID)
        : PARENT(xScene, uGUID)
        , m_xViewMatrix()
        , m_xProjectionMatrix()
        , m_fFieldOfView(Math::PI / 3.f)
    {
    }
    virtual ~Camera() {}

    void Render();

    // Getters & Setters...
    void GetViewMatrix(DirectX::XMMATRIX& xViewMatrix) const { xViewMatrix = m_xViewMatrix; }
    void GetProjectionMatrix(DirectX::XMMATRIX& xProjectionMatrix) const { xProjectionMatrix = m_xProjectionMatrix; }

private:
    DirectX::XMMATRIX m_xViewMatrix;
    DirectX::XMMATRIX m_xProjectionMatrix;
    float m_fFieldOfView;

    typedef Object PARENT;
};
