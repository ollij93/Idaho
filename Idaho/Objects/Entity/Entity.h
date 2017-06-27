#pragma once

// Includes...
#include "Objects/Object.h"
#include "Graphics/Renderable.h"

// Forward Declarations...
class Scene;

class Entity : public Object {
public:
    Entity(Scene &xScene, u_int uGUID);
    ~Entity();

    virtual void Update(float fTimestep) = 0 {}
    virtual void RegisterInputCallbacks() {}
    virtual void UnRegisterInputCallbacks() {}

private:
    Scene* m_pxScene;
    typedef Object PARENT;
};

// Abstract class to handle the multiple inheritance of Entity and Renderable
class RenderableEntity : public Entity, public Renderable {
public:
    RenderableEntity(Scene &xScene, u_int uGUID)
        : Entity(xScene, uGUID)
        , Renderable(xScene)
    {}
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
