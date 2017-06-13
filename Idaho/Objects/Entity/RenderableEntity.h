#pragma once

// Includes...
#include "Entity.h"
#include "Objects/RenderableObject.h"

// Abstract class to handle the multiple inheritance of Entity and RenderableObject
// Note: this class causes an inheritance diamond!
class RenderableEntity : public Entity, public RenderableObject {
public:
    RenderableEntity(rp3d::CollisionWorld &xWorld, u_int uGUID);
    ~RenderableEntity();
};

