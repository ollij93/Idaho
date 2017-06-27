// Includes...
#include "Entity.h"
#include "Scene.h"

Entity::Entity(Scene &xScene, u_int uGUID)
    : PARENT(xScene, uGUID)
    , m_pxScene(&xScene)
{
    if (m_pxScene) {
        m_pxScene->AddToEntityList(this);
    }
}

Entity::~Entity()
{
    if (m_pxScene) {
        m_pxScene->RemoveFromEntityList(this);
        m_pxScene = nullptr;
    }
}
