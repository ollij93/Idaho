#pragma once

// External includes...
#include <Windows.h>

// Includes...
#include "Core/Types.h"

class WindowManager {
public:
    static void InitWindow(u_int& uScreenWidth, u_int& uScreenHeight);
    static void ShutdownWindow();

    static HWND GetHandle() { return s_HWND; }
    static bool GetFullscreen() { return s_bFullscreen; }
    static bool GetVSyncEnabled() { return s_bVSyncEnabled; }
    static u_int GetScreenWidth() { return s_uScreenWidth; }
    static u_int GetScreenHeight() { return s_uScreenHeight; }
    static float GetScreenNear() { return s_fScreenNear; }
    static float GetScreenDepth() { return s_fScreenDepth; }
private:
    WindowManager() {}
    ~WindowManager() {}

    static HWND s_HWND;
    static const bool s_bFullscreen;
    static const bool s_bVSyncEnabled;
    static u_int s_uScreenWidth;
    static u_int s_uScreenHeight;
    static const float s_fScreenNear;
    static const float s_fScreenDepth;
};

