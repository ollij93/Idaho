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

    Matrix3x3 xOrientation = GetOrientation();

    // Create the rotation matrix from the objects orientation
    DirectX::XMMATRIX xRotationMatrix(xOrientation.xx, xOrientation.xy, xOrientation.xz, 0,
        xOrientation.yx, xOrientation.yy, xOrientation.yz, 0,
        xOrientation.zx, xOrientation.zy, xOrientation.zz, 0,
        0, 0, 0, 1);

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    xLookAt = DirectX::XMVector3TransformCoord(xLookAt, xRotationMatrix);
    xUpVec = DirectX::XMVector3TransformCoord(xUpVec, xRotationMatrix);

    // Translate the rotated camera position to the location of the viewer.
    xLookAt = DirectX::XMVectorAdd(xLookAt, xPos);

    // Finally create the view matrix from the three updated vectors.
    m_xViewMatrix = DirectX::XMMatrixLookAtLH(xPos, xLookAt, xUpVec);

    // Create the projection matrix
    m_xProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fFieldOfView, (float)guSCREENWIDTH/(float)guSCREENHEIGHT, gfSCREENNEAR, gfSCREENDEPTH);
}
