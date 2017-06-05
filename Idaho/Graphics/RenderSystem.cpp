// Includes...
#include "RenderSystem.h"
#include "Renderable.h"
#include "Core/Assert.h"
#include "Graphics/Shader/ColorShader.h"
#include "Graphics/Shader/TextureShader.h"

// Static Variables...
RenderSystem* Singleton<RenderSystem>::s_pxThis = nullptr;

/*
 * Init : Initialize the instance of the render system
 */
bool
RenderSystem::Init()
{
    bool bResult;

    bResult = ColorShader::Create();
    if (!bResult) { return false; }

    bResult = TextureShader::Create();
    if (!bResult) { return false; }

    return bResult;
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
    bool bResult;

    bResult = ColorShader::Render();
    if (!bResult) { return false; }

    bResult = TextureShader::Render();
    if (!bResult) { return false; }

    return bResult;
}
