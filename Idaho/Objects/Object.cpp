// Includes...
#include "Object.h"
#include "Core/Assert.h"

void
Object::InitFromSpecification(const Specification* pxSpecification)
{
    ColliderType eType;
    eType = (ColliderType)pxSpecification->GetValue<u_int>(CompileTimeGetHash("Collider"), ColliderType::UNDEFINED);

    ASSERT(eType != CONVEX_MESH && eType != CONCAVE_MESH && eType != HEIGHTFIELD, "Type of collider not yet defined.");
    if (eType == CONVEX_MESH || eType == CONCAVE_MESH || eType == HEIGHTFIELD) { eType = UNDEFINED; }

    switch (eType)
    {
        case BOX: {
            // TODO: Determine dimensions from spec
            // TODO: Determine offset from spec
            rp3d::Transform xTransform(Vector3<float>::ZeroVector(), Matrix3x3::Identity());
            rp3d::BoxShape* pxBox = new rp3d::BoxShape(Vector3<float>(0.5f, 0.5f, 0.5f));
            rp3d::ProxyShape* pxProxy = m_xCollisionbody.addCollisionShape(pxBox, xTransform);
            m_lpxCollisionShapes.push_back(pxBox);
            pxProxy->setUserData(GetProxyShapeData());
            break;
        }
        default:
            break;
    }
}
