#pragma once

// Includes...
#include "Objects/Object.h"

class Entity : virtual public Object {
public:
    Entity(rp3d::CollisionWorld &xWorld, u_int uGUID);
    ~Entity();

    virtual void Update() = 0;

    // Statics...
    static void ProcessUpdates();

private:
    static std::list<Entity*> s_lpxEntityList;
    typedef Object PARENT;
};

