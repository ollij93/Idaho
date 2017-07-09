// Includes...
#include "Core/Assert.h"
#include "GameSystem.h"

#include "LoadSystem.h"
#include "Scene.h"
#include "Graphics/2D/Renderable2D.h"
#include "Graphics/2D/Text.h"
#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/StaticObject.h"
#include "Objects/Entity/ObjectController.h"

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
    WindowManager::InitWindow(uScreenWidth, uScreenHeight);
    ASSERT(WindowManager::GetHandle(), "Failed to initialize the game window.");

    // Create the game system
    bResult = GameSystem::Create();
    ASSERT(bResult, "Failed to create GameSystem");

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
    WindowManager::ShutdownWindow();

    return 0;
}
