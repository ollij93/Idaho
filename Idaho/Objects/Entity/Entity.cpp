// Includes...
#include "Entity.h"

// Statics...
std::list<Entity*> Entity::s_lpxEntityList;

Entity::Entity(rp3d::CollisionWorld &xWorld, u_int uGUID)
    : PARENT(xWorld, uGUID)
{
    s_lpxEntityList.push_back(this);
}

Entity::~Entity()
{
    s_lpxEntityList.remove(this);
}

void
Entity::ProcessUpdates()
{
    std::list<Entity*>::const_iterator xIter;
    for (xIter = s_lpxEntityList.begin(); xIter != s_lpxEntityList.end(); ++xIter) {
        Entity* pxEntity = *xIter;
        if (pxEntity) {
            pxEntity->Update();
        }
    }
}
