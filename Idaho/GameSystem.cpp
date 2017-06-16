// Includes...
#include "GameSystem.h"
#include "InputSystem.h"
#include "Core/Assert.h"
#include "Graphics/GraphicsSystem.h"
#include "Objects/Entity/Entity.h"
#include "Specification/SpecificationSystem.h"

// Static variables...
GameSystem* Singleton<GameSystem>::s_pxThis = nullptr;

GameSystem::GameSystem()
    : m_lUpdateTime()
{
}

/*
 * Run : Perform the updates required each iteration of the game loop
 */
bool
GameSystem::Run()
{
    ASSERT(s_pxThis, "Attempting to run GameSystem before it is created.");
    InputSystem::Update();

    clock_t lUpdateTime = clock();
    float fUpdateTime = float(lUpdateTime - s_pxThis->m_lUpdateTime) / CLOCKS_PER_SEC;
    Entity::ProcessUpdates((fUpdateTime > 0.2f) ? 0.2f : fUpdateTime);
    GraphicsSystem::Render();

    s_pxThis->m_lUpdateTime = lUpdateTime;
    return true;
}

/*
 * Init : Initialize the instance of the system
 */
bool
GameSystem::Init()
{
    bool bResult;

    bResult = SpecificationSystem::Create();
    ASSERT(bResult, "Failed to create the Specification system");
    if (!bResult) { return false; }

    bResult = InputSystem::Create();
    ASSERT(bResult, "Failed to create the Input system");
    if (!bResult) { return false; }

    bResult = GraphicsSystem::Create();
    ASSERT(bResult, "Failed to create the graphics system");
    if (!bResult) { return false; }

    // Initialize the update time
    m_lUpdateTime = clock();

    return true;
}

/*
 * Shutdown : Shutdown the instance of the system
 */
void
GameSystem::Shutdown()
{
    GraphicsSystem::Destroy();
    InputSystem::Destroy();
    SpecificationSystem::Destroy();
}
