// Includes...
#include "Core/assert.h"
#include "GameSystem.h"

// Static variables...
GameSystem* GameSystem::s_pxThis = nullptr;

GameSystem::GameSystem()
{
}

/*
 * Create : Singleton create method
 */
bool
GameSystem::Create()
{
    ASSERT(!s_pxThis, "GameSystem is already created.");
    if (s_pxThis) { return false; }

    s_pxThis = new GameSystem;
    return s_pxThis->Init();
}

/*
 * Destroy : Singleton destroy method
 */
void
GameSystem::Destroy()
{
    if (!s_pxThis) { return; }

    s_pxThis->Shutdown();
    delete s_pxThis;
    s_pxThis = nullptr;
}

/*
 * Run : Perform the updates required each iteration of the game loop
 */
bool
GameSystem::Run()
{
    ASSERT(s_pxThis, "Attempting to run GameSystem before it is created.");
    return true;
}

/*
 * Init : Initialize the instance of the system
 */
bool
GameSystem::Init()
{
    return true;
}

/*
 * Shutdown : Shutdown the instance of the system
 */
void
GameSystem::Shutdown()
{
    return;
}
