#pragma once

// External includes...
#include <Windows.h>
#include <list>

// Includes...
#include "Singleton.h"

enum KeyMapping {
    LBUTTON = 0x01,
    RBUTTON,
    CANCEL,
    MBUTTON,
    // Undefined
    BACKSPACE_KEY = 0x08,
    TAB_KEY,
    // Undefined
    CLEAR_KEY = 0x0C,
    ENTER_KEY,
    // Undefined
    SHIFT_KEY = 0x10,
    CTRL_KEY,
    MENU_KEY,
    PAUSE_KEY,
    CAPSLOCK_KEY,
    // Undefined
    ESC_KEY = 0x1B,
    // Undefined
    SPACE_KEY = 0x20,
    PAGEUP_KEY,
    PAGEDOWN_KEY,
    END_KEY,
    HOME_KEY,
    LEFT_KEY,
    UP_KEY,
    RIGHT_KEY,
    DOWN_KEY,
    SELECT_KEY,
    // Undefined
    EXECUTE_KEY = 0x2B,
    PRINTSCREEN_KEY,
    INSERT_KEY,
    DELETE_KEY,
    HELPER_KEY,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    // Undefined
    A_KEY = 0x41,
    B_KEY,
    C_KEY,
    D_KEY,
    E_KEY,
    F_KEY,
    G_KEY,
    H_KEY,
    I_KEY,
    J_KEY,
    K_KEY,
    L_KEY,
    M_KEY,
    N_KEY,
    O_KEY,
    P_KEY,
    Q_KEY,
    R_KEY,
    S_KEY,
    T_KEY,
    U_KEY,
    V_KEY,
    W_KEY,
    X_KEY,
    Y_KEY,
    Z_KEY,
    LWIN_KEY,
    RWIN_KEY,
    APPS_KEY,
    // Reserved
    SLEEP_KEY = 0x5F,
    NUMPAD_0,
    NUMPAD_1,
    NUMPAD_2,
    NUMPAD_3,
    NUMPAD_4,
    NUMPAD_5,
    NUMPAD_6,
    NUMPAD_7,
    NUMPAD_8,
    NUMPAD_9,
    MULTIPLY_KEY,
    ADD_KEY,
    SEPARATOR_KEY,
    SUBTRACT_KEY,
    DECIMAL_KEY,
    DIVIDE_KEY,
    F1_KEY,
    F2_KEY,
    F3_KEY,
    F4_KEY,
    F5_KEY,
    F6_KEY,
    F7_KEY,
    F8_KEY,
    F9_KEY,
    F10_KEY,
    F11_KEY,
    F12_KEY,
    F13_KEY,
    F14_KEY,
    F15_KEY,
    F16_KEY,
    F17_KEY,
    F18_KEY,
    F19_KEY,
    F20_KEY,
    F21_KEY,
    F22_KEY,
    F23_KEY,
    F24_KEY,
    // Unassigned
    NUMLOCK_KEY = 0x90,
    SCROLLLOCK_KEY,
};
enum KeyMessageType {
    ANYKEY_MSG = 0,
    DOWN_MSG,
    UP_MSG,
};
enum MouseMessageType {
    ANYMOUSE_MSG = 0,
    LBUTTON_DOWN,
    LBUTTON_UP,
    LBUTTON_DBLCLICK,
    RBUTTON_DOWN,
    RBUTTON_UP,
    RBUTTON_DBLCLICK,
};

class InputSystem : public Singleton<InputSystem> {
public:
    // Callback types...
    typedef void(*KeyCallbackFunction)(void*, KeyMapping, KeyMessageType);
    static void RegisterKeyCallback(KeyCallbackFunction pvfnFunc, void* pContext, KeyMapping eKey, KeyMessageType eMessage);
    static void UnRegisterKeyCallback(KeyCallbackFunction pvfnFunc, void* pContext, KeyMapping eKey, KeyMessageType eMessage);

    typedef void(*MouseMoveCallbackFunction)(void*, int, int);
    static void RegisterMouseMoveCallback(MouseMoveCallbackFunction pvfnFunc, void* pContext);
    static void UnRegisterMouseMoveCallback(MouseMoveCallbackFunction pvfnFunc, void* pContext);

    typedef void(*MouseClickCallbackFunction)(void*, MouseMessageType, int, int);
    static void RegisterMouseClickCallback(MouseClickCallbackFunction pvfnFunc, void* pContext, MouseMessageType eMessage);
    static void UnRegisterMouseClickCallback(MouseClickCallbackFunction pvfnFunc, void* pContext, MouseMessageType eMessage);

    // Statics...
    static LRESULT CALLBACK HandleWindowsMessage(HWND gHWND, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void Update();

private:
    InputSystem()
        : PARENT()
        , m_lxKeyCallbacks()
        , m_lxMouseMoveCallbacks()
        , m_lxMouseClickCallbacks()
    {
    }

    virtual bool Init() override;
    virtual void Shutdown() override;

    static KeyMessageType TranslateKeyMessageType(UINT uMsg);
    static MouseMessageType TranslateMouseClickMessageType(UINT uMsg);

    // Callback structures...
    struct KeyCallbackRegister {
        KeyCallbackFunction m_pvfnFunc;
        void* m_pContext;
        KeyMapping m_eKey;
        KeyMessageType m_eMessage;

        bool operator==(KeyCallbackRegister xOther)
        {
            return (m_pvfnFunc == xOther.m_pvfnFunc && m_pContext == xOther.m_pContext && m_eKey == xOther.m_eKey && m_eMessage == xOther.m_eMessage);
        }
    };
    std::list<KeyCallbackRegister> m_lxKeyCallbacks;

    struct MouseMoveCallbackRegister {
        MouseMoveCallbackFunction m_pvfnFunc;
        void* m_pContext;

        bool operator==(MouseMoveCallbackRegister xOther)
        {
            return (m_pvfnFunc == xOther.m_pvfnFunc && m_pContext == xOther.m_pContext);
        }
    };
    std::list<MouseMoveCallbackRegister> m_lxMouseMoveCallbacks;

    struct MouseClickCallbackRegister {
        MouseClickCallbackFunction m_pvfnFunc;
        void* m_pContext;
        MouseMessageType m_eMessage;

        bool operator==(MouseClickCallbackRegister xOther)
        {
            return (m_pvfnFunc == xOther.m_pvfnFunc && m_pContext == xOther.m_pContext && m_eMessage == xOther.m_eMessage);
        }
    };
    std::list<MouseClickCallbackRegister> m_lxMouseClickCallbacks;

    // Singleton
    typedef Singleton<InputSystem> PARENT;
    friend Singleton<InputSystem>;
};
