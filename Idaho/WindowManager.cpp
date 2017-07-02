// Includes...
#include "WindowManager.h"
#include "InputSystem.h"

// Macro defines...
#define APP_NAME "Idaho"

// Statics...
HWND WindowManager::s_HWND = nullptr;
const bool WindowManager::s_bFullscreen = false;
const bool WindowManager::s_bVSyncEnabled = false;
u_int WindowManager::s_uScreenWidth = 0;
u_int WindowManager::s_uScreenHeight = 0;
const float WindowManager::s_fScreenNear = 0.1f;
const float WindowManager::s_fScreenDepth = 1000.f;

/*
 * InitWindow : Create the Windows objects and initialize the window
 */
void
WindowManager::InitWindow(u_int& uScreenWidth,
    u_int& uScreenHeight)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    DEVMODE dmScreenSettings;
    ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
    u_int uPosX = 0, uPosY = 0;

    // Setup the windows class with default settings.
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = InputSystem::HandleWindowsMessage;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = APP_NAME;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassEx(&wc);

    // Determine the resolution of the clients monitor.
    uScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    uScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (s_bFullscreen) {
        // If full screen set the screen to maximum size of the monitor and 32bit color
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (u_long)uScreenWidth;
        dmScreenSettings.dmPelsHeight = (u_long)uScreenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
    } else {
        // If windowed then set resolution to 800x600.
        uScreenWidth = 800;
        uScreenHeight = 600;

        // Place the window in the middle of the screen.
        uPosX = (GetSystemMetrics(SM_CXSCREEN) - uScreenWidth) / 2;
        uPosY = (GetSystemMetrics(SM_CYSCREEN) - uScreenHeight) / 2;
    }

    RECT xWindowRect = { 0, 0, (LONG)uScreenWidth, (LONG)uScreenHeight };
    AdjustWindowRect(&xWindowRect, WS_OVERLAPPEDWINDOW, false);

    // Create the window with the screen settings and get the handle to it
    s_HWND = CreateWindowEx(WS_EX_APPWINDOW,
        APP_NAME,
        APP_NAME,
        WS_OVERLAPPEDWINDOW,
        uPosX,
        uPosY,
        xWindowRect.right - xWindowRect.left,
        xWindowRect.bottom - xWindowRect.top,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(s_HWND, SW_SHOW);
    SetForegroundWindow(s_HWND);
    SetFocus(s_HWND);

    // Set the global variables
    s_uScreenWidth = uScreenWidth;
    s_uScreenHeight = uScreenHeight;
}

/*
 * ShutdownWindow : Release the Windows objects and destroy the window
 */
void
WindowManager::ShutdownWindow()
{
    // Show the mouse cursor
    ShowCursor(true);

    // Reset the display settings
    ChangeDisplaySettings(NULL, 0);

    // Remove the window
    DestroyWindow(s_HWND);
    s_HWND = nullptr;

    // Remove the application instance
    UnregisterClass(APP_NAME, GetModuleHandle(NULL));
}
