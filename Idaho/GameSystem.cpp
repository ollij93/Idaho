// Includes...
#include "GameSystem.h"
#include "Core/Assert.h"
#include "Graphics/GraphicsSystem.h"

// Static variables...
GameSystem* Singleton<GameSystem>::s_pxThis = nullptr;

GameSystem::GameSystem()
{
}

/*
 * Run : Perform the updates required each iteration of the game loop
 */
bool
GameSystem::Run()
{
    ASSERT(s_pxThis, "Attempting to run GameSystem before it is created.");
    GraphicsSystem::Render();
    return true;
}

/*
 * Init : Initialize the instance of the system
 */
bool
GameSystem::Init()
{
    bool bResult;

    bResult = GraphicsSystem::Create();
    ASSERT(bResult, "Failed to create the graphics system");
    if (!bResult) { return false; }

    return true;
}

/*
 * Shutdown : Shutdown the instance of the system
 */
void
GameSystem::Shutdown()
{
    GraphicsSystem::Destroy();
}
