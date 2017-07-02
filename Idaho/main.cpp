// Includes...
#include "Core/Assert.h"
#include "GameSystem.h"

#include "Scene.h"
#include "Graphics/2D/Renderable2D.h"
#include "Graphics/2D/Text.h"
#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/StaticObject.h"
#include "Objects/Entity/ObjectController.h"

// Global Variables...
// TODO: Scene* gpxMenu;
Scene* gpxScene;
Scene* gpxConsole;

void
ToggleConsole(void* pContext, KeyMapping eKey, KeyMessageType eMsg)
{
    if (eKey == BACKTICK_KEY) {
        if (Scene::GetActive() == gpxConsole) {
            Scene::SetActive(gpxScene);
        } else {
            Scene::SetActive(gpxConsole);
        }
    } else {
        ASSERT(false, "SHOULD NOT BE CALLED FOR THIS KEY!");
    }
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

    Scene xScene;
    gpxScene = &xScene;

    Camera xCamera1(xScene, 0);
    xCamera1.SetPosition(-1.f, 1.f, -5.f);
    xScene.SetActiveCamera(&xCamera1);

    Matrix3x3 xOri;
    xOri.RotateLocalX(Math::PI * 70.f / 180.f);
    xOri.RotateWorldY(Math::PI * 10.f / 180.f);

    Light xLight1(xScene, 1);
    xLight1.SetOrientation(xOri);
    xLight1.SetAmbientColor({ 0.2f, 0.2f, 0.2f, 1.f });
    xLight1.SetDiffuseColor({ 1.f, 1.f, 1.f, 1.f });
    xScene.SetActiveLight(&xLight1);

    StaticObject xObject(xScene, 2);
    xObject.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    StaticObject xObject2(xScene, 3);
    xObject2.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject2.SetPosition(Vector3<float>(2.f, 0.f, 0.f));
    StaticObject xObject3(xScene, 4);
    xObject3.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject3.SetPosition(Vector3<float>(0.f, 2.f, 0.f));
    StaticObject xObject4(xScene, 5);
    xObject4.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject4.SetPosition(Vector3<float>(0.f, 0.f, 2.f));

    ObjectController xController1(xScene, 6, &xCamera1);

    Scene xConsoleScene;
    Camera xConsoleCam(xConsoleScene, 0);
    xConsoleScene.SetActiveCamera(&xConsoleCam);
    gpxConsole = &xConsoleScene;
    Renderable2D xBackground(xConsoleScene, uScreenWidth, uScreenHeight);
    xBackground.SetTextureHash(GetHash("Textures/Default_White.dds"));
    xBackground.Init();
    xBackground.SetPosition(Vector2<int>::ZeroVector(), true);
    Text xText(xConsoleScene, "CONSOLE SCREEN", GetHash("Default"));

    Scene::SetActive(&xConsoleScene);
    Scene::SetActive(&xScene);

    InputSystem::RegisterKeyCallback(ToggleConsole, nullptr, BACKTICK_KEY, UP_MSG);

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
