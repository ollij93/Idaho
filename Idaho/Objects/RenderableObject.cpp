// Includes...
#include "RenderableObject.h"

RenderableObject::RenderableObject(rp3d::CollisionWorld &xWorld, u_int uGUID)
    : Object(xWorld, uGUID)
    , Renderable()
{
}

RenderableObject::~RenderableObject()
{
}

DirectX::XMMATRIX
RenderableObject::GetWorldMatrix() const
{
    const Matrix3x3 xOri = GetOrientation();
    const Vector3<float> xPos = GetPosition();
    return DirectX::XMMATRIX(xOri.xx, xOri.yx, xOri.zx, 0.f,
        xOri.xy, xOri.yy, xOri.zy, 0.f,
        xOri.xz, xOri.yz, xOri.zz, 0.f,
        xPos.x, xPos.y, xPos.z, 1.f);
}
