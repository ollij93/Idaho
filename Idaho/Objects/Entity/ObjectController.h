#pragma once

// Includes...
#include "Entity.h"
#include "InputSystem.h"
#include "Objects/Object.h"

class ObjectController : public Entity {
public:
    ObjectController(rp3d::CollisionWorld &xWorld, u_int uGUID, Object* pxObject)
        : PARENT(xWorld, uGUID)
        , m_pxObject(pxObject)
        , m_cMovementBools(0)
    {
        InputSystem::RegisterKeyCallback(MoveCallback, this, W_KEY, ANYKEY_MSG);
        InputSystem::RegisterKeyCallback(MoveCallback, this, A_KEY, ANYKEY_MSG);
        InputSystem::RegisterKeyCallback(MoveCallback, this, S_KEY, ANYKEY_MSG);
        InputSystem::RegisterKeyCallback(MoveCallback, this, D_KEY, ANYKEY_MSG);
        InputSystem::RegisterKeyCallback(MoveCallback, this, SHIFT_KEY, ANYKEY_MSG);
        InputSystem::RegisterKeyCallback(MoveCallback, this, SPACE_KEY, ANYKEY_MSG);
        InputSystem::RegisterMouseMoveCallback(MouseMoveCallback, this);
    }
    ~ObjectController() {}

    // Overrides...
    virtual void Update(float fTimestep) override;

private:
    static void MoveCallback(void* pContext, KeyMapping eKey, KeyMessageType eMsg);
    static void MouseMoveCallback(void* pContext, int iMoveX, int iMoveY);

    union {
        char m_cMovementBools;
        struct {
            bool bLeft : 1;
            bool bRight : 1;
            bool bUp : 1;
            bool bDown : 1;
            bool bForward : 1;
            bool bBackward : 1;
            // 2 bits remain
        } m_xMovement;
    };

    Object* m_pxObject;

    typedef Entity PARENT;
};
