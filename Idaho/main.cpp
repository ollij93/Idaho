// Includes...
#include "Core/Assert.h"
#include "GameSystem.h"

#include "Scene.h"
#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/StaticObject.h"
#include "Objects/Entity/ObjectController.h"

// Global Variables...
Scene* gapxScenes[2];

void
SwitchToScene(void* pContext, KeyMapping eKey, KeyMessageType eMsg)
{
    if (eKey == KEY_1) {
        Scene::SetActive(gapxScenes[0]);
    } else if (eKey == KEY_2) {
        Scene::SetActive(gapxScenes[1]);
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

    Scene xScene1;

    Camera xCamera1(xScene1, 0);
    xCamera1.SetPosition(-1.f, 1.f, -5.f);
    xScene1.SetActiveCamera(&xCamera1);

    Matrix3x3 xOri;
    xOri.RotateLocalX(Math::PI * 70.f / 180.f);
    xOri.RotateWorldY(Math::PI * 10.f / 180.f);

    Light xLight1(xScene1, 1);
    xLight1.SetOrientation(xOri);
    xLight1.SetAmbientColor({ 0.2f, 0.2f, 0.2f, 1.f });
    xLight1.SetDiffuseColor({ 1.f, 1.f, 1.f, 1.f });
    xScene1.SetActiveLight(&xLight1);

    StaticObject xObject(xScene1, 2);
    xObject.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    StaticObject xObject2(xScene1, 3);
    xObject2.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject2.SetPosition(Vector3<float>(2.f, 0.f, 0.f));
    StaticObject xObject3(xScene1, 4);
    xObject3.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject3.SetPosition(Vector3<float>(0.f, 2.f, 0.f));
    StaticObject xObject4(xScene1, 5);
    xObject4.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject4.SetPosition(Vector3<float>(0.f, 0.f, 2.f));

    ObjectController xController1(xScene1, 6, &xCamera1);

    Scene xScene2;

    Camera xCamera2(xScene2, 100);
    xCamera2.SetPosition(-1.f, 1.f, -5.f);
    xScene2.SetActiveCamera(&xCamera2);

    Light xLight2(xScene2, 101);
    xLight2.SetOrientation(xOri);
    xLight2.SetAmbientColor({ 0.2f, 0.2f, 0.2f, 1.f });
    xLight2.SetDiffuseColor({ 1.f, 0.2f, 0.2f, 1.f });
    xScene2.SetActiveLight(&xLight2);

    StaticObject xObject__(xScene2, 103);
    xObject__.InitFromSpecification(SpecificationSystem::GetSpecificationByHash(GetHash("Block")));
    xObject__.SetPosition(Vector3<float>(2.f, 0.f, 0.f));

    ObjectController xController2(xScene2, 106, &xCamera2);

    Scene::SetActive(&xScene2);
    Scene::SetActive(&xScene1);

    gapxScenes[0] = &xScene1;
    gapxScenes[1] = &xScene2;

    InputSystem::RegisterKeyCallback(SwitchToScene, nullptr, KEY_1, UP_MSG);
    InputSystem::RegisterKeyCallback(SwitchToScene, nullptr, KEY_2, UP_MSG);

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
