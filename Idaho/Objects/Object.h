#pragma once

// External Includes...
#include "ReactPhysics3D/reactphysics3d.h"

// Includes...
#include "Core/Math.h"
#include "Core/Types.h"
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
    Object(rp3d::CollisionWorld &xWorld, u_int uGUID)
        : m_xCollisionbody(rp3d::Transform::identity(), xWorld, uGUID)
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
    u_int m_uGUID;
    std::list<rp3d::CollisionShape*> m_lpxCollisionShapes;
};
