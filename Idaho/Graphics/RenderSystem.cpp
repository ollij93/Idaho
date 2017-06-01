// Includes...
#include "RenderSystem.h"
#include "Core/Assert.h"

// Static Variables...
RenderSystem* Singleton<RenderSystem>::s_pxThis = nullptr;

/*
 * Init : Initialize the instance of the render system
 */
bool
RenderSystem::Init()
{
    return true;
}

/*
 * Shutdown : Shutdown the instance of the render system
 */
void
RenderSystem::Shutdown()
{
}

/*
 * Render : For each registered shader call out to the required rendering methods
 */
bool
RenderSystem::Render()
{
    return true;
}
