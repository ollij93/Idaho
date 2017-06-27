#pragma once

// Includes...
#include "Entity.h"
#include "InputSystem.h"
#include "Objects/Object.h"

class ObjectController : public Entity {
public:
    ObjectController(Scene &xScene, u_int uGUID, Object* pxObject)
        : PARENT(xScene, uGUID)
        , m_pxObject(pxObject)
        , m_cMovementBools(0)
    {
    }
    ~ObjectController() {}

    // Overrides...
    virtual void Update(float fTimestep) override;
    virtual void RegisterInputCallbacks() override;
    virtual void UnRegisterInputCallbacks() override;

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
