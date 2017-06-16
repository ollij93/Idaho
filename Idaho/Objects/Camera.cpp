// Includes...
#include "Camera.h"
#include "Core/Types.h"

// Global Variables...
extern u_int guSCREENWIDTH;
extern u_int guSCREENHEIGHT;
extern const float gfSCREENNEAR;
extern const float gfSCREENDEPTH;

// Statics...
Camera* Camera::s_pxActive = nullptr;

void
Camera::Render()
{
    DirectX::XMVECTOR xPos = GetPosition();
    DirectX::XMVECTOR xUpVec = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
    DirectX::XMVECTOR xLookAt = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);

    DirectX::XMMATRIX xOrientation = GetOrientation();

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    xLookAt = DirectX::XMVector3TransformCoord(xLookAt, xOrientation);
    xUpVec = DirectX::XMVector3TransformCoord(xUpVec, xOrientation);

    // Translate the rotated camera position to the location of the viewer.
    xLookAt = DirectX::XMVectorAdd(xLookAt, xPos);

    // Finally create the view matrix from the three updated vectors.
    m_xViewMatrix = DirectX::XMMatrixLookAtRH(xPos, xLookAt, xUpVec);

    // Create the projection matrix
    m_xProjectionMatrix = DirectX::XMMatrixPerspectiveFovRH(m_fFieldOfView, (float)guSCREENWIDTH/(float)guSCREENHEIGHT, gfSCREENNEAR, gfSCREENDEPTH);
}
