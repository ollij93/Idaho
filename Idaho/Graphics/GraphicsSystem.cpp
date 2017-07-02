// Includes...
#include "GraphicsSystem.h"
#include "D3DSystem.h"
#include "RenderSystem.h"
#include "TextureHandler.h"
#include "Core/Assert.h"
#include "Graphics/2D/Font.h"

// Static Variables...
GraphicsSystem* Singleton<GraphicsSystem>::s_pxThis = nullptr;

/*
 * Render : Function to control the rendering process each frame
 */
void
GraphicsSystem::Render()
{
    D3DSystem::BeginScene();
    RenderSystem::Render();
    D3DSystem::EndScene();
}

/*
 * Init : Initialize the instance of the graphics system
 */
bool
GraphicsSystem::Init()
{
    bool bResult = true;

    // Create the directX interface system
    bResult = D3DSystem::Create();
    ASSERT(bResult, "Failed to create the directX system.");
    if (!bResult) { return false; }

    // Create the rendering system
    bResult = RenderSystem::Create();
    ASSERT(bResult, "Failed to create the render system.");
    if (!bResult) { return false; }

    // Create the texture handler
    bResult = TextureHandler::Create();
    ASSERT(bResult, "Failed to create the texture handler.");
    if (!bResult) { return false; }

    // Create the fonts
    bResult = Font::LoadAll();
    ASSERT(bResult, "Failed to load the fonts.");
    if (!bResult) { return false; }

    return true;
}

/*
 * Shutdown : Shutdown the instance of the graphics system
 */
void
GraphicsSystem::Shutdown()
{
    Font::ShutdownAll();
    TextureHandler::Destroy();
    RenderSystem::Destroy();
    D3DSystem::Destroy();
}
