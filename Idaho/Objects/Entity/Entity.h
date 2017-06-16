#pragma once

// Includes...
#include "Objects/Object.h"
#include "Graphics/Renderable.h"

class Entity : public Object {
public:
    Entity(rp3d::CollisionWorld &xWorld, u_int uGUID);
    ~Entity();

    virtual void Update(float fTimestep) = 0 {}

    // Statics...
    static void ProcessUpdates(float fTimestep);

private:
    static std::list<Entity*> s_lpxEntityList;
    typedef Object PARENT;
};

// Abstract class to handle the multiple inheritance of Entity and Renderable
class RenderableEntity : public Entity, public Renderable {
public:
    RenderableEntity(rp3d::CollisionWorld &xWorld, u_int uGUID)
        : Entity(xWorld, uGUID)
        , Renderable()
    {
    }
    ~RenderableEntity() {}

    // Overrides...
    virtual DirectX::XMMATRIX GetWorldMatrix() const override
    {
        DirectX::XMMATRIX xOri = GetOrientation();
        const Vector3<float> xPos = GetPosition();
        DirectX::XMMATRIX xWorldMatrix = DirectX::XMMatrixTranslation(xPos.x, xPos.y, xPos.z);
        return DirectX::XMMatrixMultiply(xOri, xWorldMatrix);
    }
};
