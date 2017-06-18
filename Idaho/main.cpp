// External includes...
#include <Windows.h>

// Macro defines...
#define APP_NAME "Idaho"

// Internal includes...
#include "Core/assert.h"
#include "GameSystem.h"

#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/StaticObject.h"
#include "Objects/Entity/ObjectController.h"

// Global Variables...
extern HWND gHWND = nullptr;
extern const bool gbFULLSCREEN = false;
extern u_int guSCREENWIDTH = 0;
extern u_int guSCREENHEIGHT = 0;
extern const float gfSCREENNEAR = 0.1f;
extern const float gfSCREENDEPTH = 1000.f;

/*
 * InitWindow : Create the Windows objects and initialize the window
 */
void
InitWindow(u_int& uScreenWidth,
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

    if (gbFULLSCREEN) {
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
    gHWND = CreateWindowEx(WS_EX_APPWINDOW,
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
    ShowWindow(gHWND, SW_SHOW);
    SetForegroundWindow(gHWND);
    SetFocus(gHWND);

    // Set the global variables
    guSCREENWIDTH = uScreenWidth;
    guSCREENHEIGHT = uScreenHeight;
}

/*
 * InitWindow : Release the Windows objects and destroy the window
 */
void
ShutdownWindow()
{
    // Show the mouse cursor
    ShowCursor(true);

    // Reset the display settings
    ChangeDisplaySettings(NULL, 0);

    // Remove the window
    DestroyWindow(gHWND);
    gHWND = nullptr;

    // Remove the application instance
    UnregisterClass(APP_NAME, GetModuleHandle(NULL));
}

int WINAPI
WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PSTR pScmdline,
    int iCmdshow)
{
    bool bResult;
    u_int uScreenWidth;
    u_int uScreenHeight;
    MSG xMsg;
    ZeroMemory(&xMsg, sizeof(MSG));

    // Create the game window
    InitWindow(uScreenWidth, uScreenHeight);
    ASSERT(gHWND, "Failed to initialize the game window.");

    // Create the game system
    bResult = GameSystem::Create();
    ASSERT(bResult, "Failed to create GameSystem");

    rp3d::CollisionWorld xWorld;

    Camera xCamera(xWorld, 0);
    xCamera.SetPosition(-1.f, 1.f, -5.f);
    Camera::SetActive(&xCamera);

    Matrix3x3 xOri;
//    xOri.RotateLocalX(Math::PI * 70.f / 180.f);
    xOri.RotateWorldY(Math::PI * 10.f / 180.f);

    Light xLight(xWorld, 1);
    xLight.SetOrientation(xOri);
    xLight.SetAmbientColor({ 0.2f, 0.2f, 0.2f, 1.f });
    xLight.SetDiffuseColor({ 1.f, 1.f, 1.f, 1.f });
    xLight.SetSpecularColor({ 1.0f, 1.0f, 0.5f, 1.f });
    Light::SetActive(&xLight);

    StaticObject xObject(xWorld, 2);
    xObject.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    StaticObject xObject2(xWorld, 3);
    xObject2.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject2.SetPosition(Vector3<float>(2.f, 0.f, 0.f));
    StaticObject xObject3(xWorld, 4);
    xObject3.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject3.SetPosition(Vector3<float>(0.f, 2.f, 0.f));
    StaticObject xObject4(xWorld, 5);
    xObject4.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject4.SetPosition(Vector3<float>(0.f, 0.f, 2.f));

    ObjectController xController(xWorld, 6, &xCamera);

    ///////////////////////////////////
    // M A I N L O O P
    ///////////////////////////////////
    while (bResult) {
        // Handle the windows messages.
        if (PeekMessage(&xMsg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&xMsg);
            DispatchMessage(&xMsg);
        }

        if (xMsg.message == WM_QUIT) {
            bResult = false;
        } else {
            // No message to quit from windows so run the game system main function
            bResult = GameSystem::Run();
        }
    }

    // Shutdown the game system
    GameSystem::Destroy();

    // Shutdown the game window
    ShutdownWindow();

    return 0;
}
