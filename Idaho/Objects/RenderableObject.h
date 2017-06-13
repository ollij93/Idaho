#pragma once

// Includes...
#include "Object.h"
#include "Graphics/Renderable.h"

// Abstract class to handle the multiple inheritance of Object and Renderable
class RenderableObject : virtual public Object, public Renderable {
public:
    RenderableObject(rp3d::CollisionWorld &xWorld, u_int uGUID);
    ~RenderableObject();

    // Overrides...
    virtual DirectX::XMMATRIX GetWorldMatrix() const;
};

