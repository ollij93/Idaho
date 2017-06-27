#pragma once

// Includes...
#include "Core/Math.h"
#include "Core/Types.h"
#include "Graphics/Renderable.h"
#include "Scene.h"
#include "Specification/Specification.h"

enum ColliderType {
    UNDEFINED = 0,
    BOX,
    SPHERE,
    CONE,
    CYLINDER,
    CAPSULE,
    CONVEX_MESH,
    CONCAVE_MESH,
    HEIGHTFIELD,
};

class Object {
public:
    Object(Scene &xScene, u_int uGUID)
        : m_xCollisionbody(rp3d::Transform::identity(), *xScene.GetWorld(), uGUID)
        , m_pxCollisionWorld(xScene.GetWorld())
        , m_uGUID(uGUID)
    {
    }
    ~Object()
    {
        std::list<rp3d::CollisionShape*>::const_iterator xIter;
        for (xIter = m_lpxCollisionShapes.begin(); xIter != m_lpxCollisionShapes.end(); ++xIter) {
            rp3d::CollisionShape* pxShape = *xIter;
            if (pxShape) {
                delete pxShape;
                pxShape = nullptr;
            }
        }
        m_lpxCollisionShapes.clear();
    }

    Vector3<float> GetPosition() const { return m_xCollisionbody.getTransform().getPosition(); }
    void SetPosition(float x, float y, float z) { m_xCollisionbody.setTransform(rp3d::Transform(rp3d::Vector3(x, y, z), m_xCollisionbody.getTransform().getOrientation())); }
    void SetPosition(const Vector3<float>& xVector) { m_xCollisionbody.setTransform(rp3d::Transform(xVector, m_xCollisionbody.getTransform().getOrientation())); }
    Matrix3x3 GetOrientation() const { return m_xCollisionbody.getTransform().getOrientation().getMatrix(); }
    void SetOrientation(const Matrix3x3& xOri) { m_xCollisionbody.setTransform(rp3d::Transform(m_xCollisionbody.getTransform().getPosition(), xOri)); }

    virtual void InitFromSpecification(const Specification* pxSpecification);
    virtual void* GetProxyShapeData() { return nullptr; }

protected:
    rp3d::CollisionBody m_xCollisionbody;
    rp3d::CollisionWorld* m_pxCollisionWorld;
    u_int m_uGUID;
    std::list<rp3d::CollisionShape*> m_lpxCollisionShapes;
};

// Abstract class to handle the multiple inheritance of Object and Renderable
class RenderableObject : public Object, public Renderable {
public:
    RenderableObject::RenderableObject(Scene &xScene, u_int uGUID)
        : Object(xScene, uGUID)
        , Renderable(xScene)
    {
    }

    RenderableObject::~RenderableObject() {}

    // Overrides...
    virtual DirectX::XMMATRIX RenderableObject::GetWorldMatrix() const
    {
        DirectX::XMMATRIX xOri = GetOrientation();
        const Vector3<float> xPos = GetPosition();
        DirectX::XMMATRIX xWorldMatrix = DirectX::XMMatrixTranslation(xPos.x, xPos.y, xPos.z);
        return DirectX::XMMatrixMultiply(xOri, xWorldMatrix);
    }
    virtual void InitFromSpecification(const Specification* pxSpecification) override
    {
        Object::InitFromSpecification(pxSpecification);
        AddToRenderList();
    }
};
