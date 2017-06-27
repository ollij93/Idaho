// Includes...
#include "ObjectController.h"
#include "Core/Assert.h"
#include "Objects/StaticObject.h"

// Globals...
const float gfMOVERATE = 2.f; ///(2 units per second)
const float gfMOUSESENSITIVITY = 0.001f;

void
ObjectController::RegisterInputCallbacks()
{
    InputSystem::RegisterKeyCallback(MoveCallback, this, W_KEY, ANYKEY_MSG);
    InputSystem::RegisterKeyCallback(MoveCallback, this, A_KEY, ANYKEY_MSG);
    InputSystem::RegisterKeyCallback(MoveCallback, this, S_KEY, ANYKEY_MSG);
    InputSystem::RegisterKeyCallback(MoveCallback, this, D_KEY, ANYKEY_MSG);
    InputSystem::RegisterKeyCallback(MoveCallback, this, SHIFT_KEY, ANYKEY_MSG);
    InputSystem::RegisterKeyCallback(MoveCallback, this, SPACE_KEY, ANYKEY_MSG);
    InputSystem::RegisterMouseMoveCallback(MouseMoveCallback, this);
}

void
ObjectController::UnRegisterInputCallbacks()
{
    InputSystem::UnRegisterKeyCallback(MoveCallback, this, W_KEY, ANYKEY_MSG);
    InputSystem::UnRegisterKeyCallback(MoveCallback, this, A_KEY, ANYKEY_MSG);
    InputSystem::UnRegisterKeyCallback(MoveCallback, this, S_KEY, ANYKEY_MSG);
    InputSystem::UnRegisterKeyCallback(MoveCallback, this, D_KEY, ANYKEY_MSG);
    InputSystem::UnRegisterKeyCallback(MoveCallback, this, SHIFT_KEY, ANYKEY_MSG);
    InputSystem::UnRegisterKeyCallback(MoveCallback, this, SPACE_KEY, ANYKEY_MSG);
    InputSystem::UnRegisterMouseMoveCallback(MouseMoveCallback, this);
}

void
ObjectController::MoveCallback(void* pContext, KeyMapping eKey, KeyMessageType eMsg)
{
    bool bDown = (eMsg == DOWN_MSG);
    ObjectController* pxThis = static_cast<ObjectController*>(pContext);
    if (pxThis) {
        switch (eKey) {
            case W_KEY:
                pxThis->m_xMovement.bForward = bDown;
                break;
            case A_KEY:
                pxThis->m_xMovement.bLeft = bDown;
                break;
            case S_KEY:
                pxThis->m_xMovement.bBackward = bDown;
                break;
            case D_KEY:
                pxThis->m_xMovement.bRight = bDown;
                break;
            case SHIFT_KEY:
                pxThis->m_xMovement.bDown = bDown;
                break;
            case SPACE_KEY:
                pxThis->m_xMovement.bUp = bDown;
                break;
            default:
                ASSERT(false, "Recieved callback for invalid key message.");
                break;
        }
    }
}

void
ObjectController::MouseMoveCallback(void* pContext, int iMoveX, int iMoveY)
{
    ObjectController* pxThis = static_cast<ObjectController*>(pContext);
    if (pxThis && pxThis->m_pxObject) {
        Matrix3x3 xOri = pxThis->m_pxObject->GetOrientation();
        xOri.RotateWorldY(gfMOUSESENSITIVITY * iMoveX);
        xOri.RotateLocalX(gfMOUSESENSITIVITY * iMoveY);
        // Check the object isn't over-rotated (Local Y axis opposite direction to world Y axis)
        if (xOri.yy < 0.f) {
            float fCorrectionAngle = asin(xOri.yy);
            if (xOri.yz > 0.f) {
                // Local positive z axis is projected onto the positive world y axis so object looking upwards (rotate down)
                fCorrectionAngle = -fCorrectionAngle;
            }
            xOri.RotateLocalX(fCorrectionAngle);
        }
        pxThis->m_pxObject->SetOrientation(xOri);
    }
}

void
ObjectController::Update(float fTimestep)
{
    PARENT::Update(fTimestep);

    Vector3<float> xMove = Vector3<float>::ZeroVector();
    const float fMoveDelta = fTimestep * gfMOVERATE;

    if (m_xMovement.bLeft) {
        xMove.x += fMoveDelta;
    }
    if (m_xMovement.bRight) {
        xMove.x -= fMoveDelta;
    }
    if (m_xMovement.bUp) {
        xMove.y += fMoveDelta;
    }
    if (m_xMovement.bDown) {
        xMove.y -= fMoveDelta;
    }
    if (m_xMovement.bForward) {
        xMove.z += fMoveDelta;
    }
    if (m_xMovement.bBackward) {
        xMove.z -= fMoveDelta;
    }

    xMove = m_pxObject->GetOrientation() * xMove;

    if (m_pxObject) {
        m_pxObject->SetPosition(m_pxObject->GetPosition() + xMove);
    }
}
