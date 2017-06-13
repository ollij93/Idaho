// Includes...
#include "RenderableEntity.h"

RenderableEntity::RenderableEntity(rp3d::CollisionWorld &xWorld, u_int uGUID)
    : Object(xWorld, uGUID)
    , Entity(xWorld, uGUID)
    , RenderableObject(xWorld, uGUID)
{
}

RenderableEntity::~RenderableEntity()
{
}
