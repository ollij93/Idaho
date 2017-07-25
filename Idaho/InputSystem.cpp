// External Includes...
#include <windowsx.h>

// Includes...
#include "InputSystem.h"
#include "Core/Assert.h"

// Statics...
InputSystem* Singleton<InputSystem>::s_pxThis = nullptr;

/*
 * RegisterKeyCallback : Register a function callback for a keyboard event message
 */
void
InputSystem::RegisterKeyCallback(KeyCallbackFunction pvfnFunc, void* pContext, KeyMapping eKey, KeyMessageType eMessage)
{
    ASSERT(s_pxThis, "Attempting to register key callbacks before the input system is created");
    KeyCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    xReg.m_eKey = eKey;
    xReg.m_eMessage = eMessage;
    s_pxThis->m_lxKeyCallbacks.push_back(xReg);
}

/*
 * UnRegisterKeyCallback : Remove a function callback for a keyboard event message
 */
void
InputSystem::UnRegisterKeyCallback(KeyCallbackFunction pvfnFunc, void* pContext, KeyMapping eKey, KeyMessageType eMessage)
{
    ASSERT(s_pxThis, "Attempting to unregister key callbacks before the input system is created");
    KeyCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    xReg.m_eKey = eKey;
    xReg.m_eMessage = eMessage;
    s_pxThis->m_lxKeyCallbacks.remove(xReg);
}

/*
 * RegisterMouseMoveCallback : Register a function callback for mouse movement
 */
void
InputSystem::RegisterMouseMoveCallback(MouseMoveCallbackFunction pvfnFunc, void* pContext)
{
    ASSERT(s_pxThis, "Attempting to register key callbacks before the input system is created");
    MouseMoveCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    s_pxThis->m_lxMouseMoveCallbacks.push_back(xReg);
}

/*
 * UnRegisterMouseMoveCallback : Remove a function callback for mouse movement
 */
void
InputSystem::UnRegisterMouseMoveCallback(MouseMoveCallbackFunction pvfnFunc, void* pContext)
{
    ASSERT(s_pxThis, "Attempting to unregister key callbacks before the input system is created");
    MouseMoveCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    s_pxThis->m_lxMouseMoveCallbacks.remove(xReg);
}

/*
 * RegisterKeyCallback : Register a function callback for a mouse click event message
 */
void
InputSystem::RegisterMouseClickCallback(MouseClickCallbackFunction pvfnFunc, void* pContext, MouseMessageType eMessage)
{
    ASSERT(s_pxThis, "Attempting to register key callbacks before the input system is created");
    MouseClickCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    xReg.m_eMessage = eMessage;
    s_pxThis->m_lxMouseClickCallbacks.push_back(xReg);
}

/*
 * UnRegisterKeyCallback : Remove a function callback for a mouse click event message
 */
void
InputSystem::UnRegisterMouseClickCallback(MouseClickCallbackFunction pvfnFunc, void* pContext, MouseMessageType eMessage)
{
    ASSERT(s_pxThis, "Attempting to unregister key callbacks before the input system is created");
    MouseClickCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    xReg.m_eMessage = eMessage;
    s_pxThis->m_lxMouseClickCallbacks.remove(xReg);
}

/*
 * RegisterTextCallback : Register a function callback for text input event messages
 */
void
InputSystem::RegisterTextCallback(TextCallbackFunction pvfnFunc, void* pContext)
{
    ASSERT(s_pxThis, "Attempting to register text callbacks before the input system is created");
    TextCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    s_pxThis->m_lxTextCallbacks.push_back(xReg);
}

/*
 * UnRegisterTextCallback : Remove a function callback for text input event messages
 */
void
InputSystem::UnRegisterTextCallback(TextCallbackFunction pvfnFunc, void* pContext)
{
    ASSERT(s_pxThis, "Attempting to unregister text callbacks before the input system is created");
    TextCallbackRegister xReg;
    xReg.m_pvfnFunc = pvfnFunc;
    xReg.m_pContext = pContext;
    s_pxThis->m_lxTextCallbacks.remove(xReg);
}

/*
 * HandleWindowsMessage : Callback function for Windows messages
 */
LRESULT CALLBACK
InputSystem::HandleWindowsMessage(HWND gHWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        // Handle the window specific messages
        case WM_DESTROY:
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        {
            // Handle mouse click
            POINT xPoint;
            GetCursorPos(&xPoint);
            ScreenToClient(gHWND, &xPoint);
            std::list<MouseClickCallbackRegister>::const_iterator xIter;
            for (xIter = s_pxThis->m_lxMouseClickCallbacks.begin(); xIter != s_pxThis->m_lxMouseClickCallbacks.end(); ++xIter) {
                MouseClickCallbackRegister xReg = *xIter;
                if (xReg.m_eMessage == ANYMOUSE_MSG
                    || (uMsg == WM_LBUTTONDOWN && xReg.m_eMessage == LBUTTON_DOWN)
                    || (uMsg == WM_RBUTTONDOWN && xReg.m_eMessage == RBUTTON_DOWN)
                    || (uMsg == WM_LBUTTONUP && xReg.m_eMessage == LBUTTON_UP)
                    || (uMsg == WM_RBUTTONUP && xReg.m_eMessage == RBUTTON_UP)
                    || (uMsg == WM_LBUTTONDBLCLK && xReg.m_eMessage == LBUTTON_DBLCLICK)
                    || (uMsg == WM_RBUTTONDBLCLK && xReg.m_eMessage == RBUTTON_DBLCLICK)) {
                    xReg.m_pvfnFunc(xReg.m_pContext, TranslateMouseClickMessageType(uMsg), xPoint.x, xPoint.y);
                }
            }
            break;
        }
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
            if (wParam == ESC_KEY) {
                PostQuitMessage(0);
            }
            // Handle keyboard input
            std::list<KeyCallbackRegister>::const_iterator xIter;
            for (xIter = s_pxThis->m_lxKeyCallbacks.begin(); xIter != s_pxThis->m_lxKeyCallbacks.end(); ++xIter) {
                KeyCallbackRegister xReg = *xIter;
                if (xReg.m_eKey == wParam
                    && (xReg.m_eMessage == ANYKEY_MSG
                        || ((uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) && xReg.m_eMessage == DOWN_MSG)
                        || ((uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) && xReg.m_eMessage == UP_MSG))) {
                    xReg.m_pvfnFunc(xReg.m_pContext, (KeyMapping)wParam, TranslateKeyMessageType(uMsg));
                }
            }
            break;
        }
        case WM_CHAR:
        {
            // Handle general text input
            std::list<TextCallbackRegister>::const_iterator xIter;
            for (xIter = s_pxThis->m_lxTextCallbacks.begin(); xIter != s_pxThis->m_lxTextCallbacks.end(); ++xIter) {
                TextCallbackRegister xReg = *xIter;
                xReg.m_pvfnFunc(xReg.m_pContext, (char)wParam);
            }
        }
        // Default to let windows handle the message
        default:
            return DefWindowProc(gHWND, uMsg, wParam, lParam);
    }

    return 0;
}

void
InputSystem::Update()
{
    // Handle mouse movement
    POINT xPoint;
    GetCursorPos(&xPoint);
    ScreenToClient(WindowManager::GetHandle(), &xPoint);

    int iMouseMoveX = xPoint.x - WindowManager::GetScreenWidth()/2;
    int iMouseMoveY = xPoint.y - WindowManager::GetScreenHeight()/2;
    std::list<MouseMoveCallbackRegister>::const_iterator xIter;
    for (xIter = s_pxThis->m_lxMouseMoveCallbacks.begin(); xIter != s_pxThis->m_lxMouseMoveCallbacks.end(); ++xIter) {
        MouseMoveCallbackRegister xReg = *xIter;
        xReg.m_pvfnFunc(xReg.m_pContext, iMouseMoveX, iMouseMoveY);
    }

    // Reset the cursor
    if (s_pxThis->m_bLockMouse) {
        xPoint = { (int)WindowManager::GetScreenWidth()/2, (int)WindowManager::GetScreenHeight()/2 };
        ClientToScreen(WindowManager::GetHandle(), &xPoint);
        SetCursorPos(xPoint.x, xPoint.y);
    }
}

bool
InputSystem::Init()
{
    SetCapture(WindowManager::GetHandle());
    POINT xPoint = { (int)WindowManager::GetScreenWidth()/2, (int)WindowManager::GetScreenHeight()/2 };
    ScreenToClient(WindowManager::GetHandle(), &xPoint);
    SetCursorPos(xPoint.x, xPoint.y);
    return true;
}

void
InputSystem::Shutdown()
{
    while (ShowCursor(true) < 0) {}
    ReleaseCapture();
}

KeyMessageType
InputSystem::TranslateKeyMessageType(UINT uMsg)
{
    switch (uMsg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            return DOWN_MSG;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            return UP_MSG;
        default:
            ASSERT(false, "TranslateKeyMessageType called on invalid message!");
            break;
    }
    return ANYKEY_MSG;
}

MouseMessageType
InputSystem::TranslateMouseClickMessageType(UINT uMsg)
{
    switch (uMsg) {
        case WM_LBUTTONDOWN:
            return LBUTTON_DOWN;
        case WM_LBUTTONUP:
            return LBUTTON_UP;
        case WM_LBUTTONDBLCLK:
            return LBUTTON_DBLCLICK;
        case WM_RBUTTONDOWN:
            return RBUTTON_DOWN;
        case WM_RBUTTONUP:
            return RBUTTON_UP;
        case WM_RBUTTONDBLCLK:
            return RBUTTON_DBLCLICK;
        default:
            ASSERT(false, "TranslateMouseClickMessageType called on invalid message!");
            break;
    }
    return ANYMOUSE_MSG;
}
