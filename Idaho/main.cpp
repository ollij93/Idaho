// Includes...
#include "Core/Assert.h"
#include "GameSystem.h"

#include "InputSystem.h"
#include "Scene.h"
#include "Commands.h"

void ToggleConsole(void*, KeyMapping, KeyMessageType)
{
    static bool bConsole = false;

    if (bConsole) {
        Scene::SwitchToScene(2);
    } else {
        Scene::SwitchToScene(1);
    }
    bConsole = !bConsole;
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
    WindowManager::InitWindow(uScreenWidth, uScreenHeight);
    ASSERT(WindowManager::GetHandle(), "Failed to initialize the game window.");

    // Create the game system
    bResult = GameSystem::Create();
    ASSERT(bResult, "Failed to create GameSystem");

    // Register the console toggle key
    InputSystem::RegisterKeyCallback(ToggleConsole, nullptr, BACKTICK_KEY, UP_MSG);

    cmd::RegisterCommand<u_int>(GetHash("Scene.SwitchTo"), Scene::SwitchToScene);

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
