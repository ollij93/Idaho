// Includes...
#include "D3DSystem.h"
#include "RenderSystem.h"
#include "Core/Assert.h"
#include "Graphics/Shader/BitMapShader.h"
#include "Graphics/Shader/ColorShader.h"
#include "Graphics/Shader/LightShader.h"
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

    bResult = LightShader::Create();
    if (!bResult) { return false; }

    bResult = BitMapShader::Create();
    if (!bResult) { return false; }

    return bResult;
}

/*
 * Shutdown : Shutdown the instance of the render system
 */
void
RenderSystem::Shutdown()
{
    BitMapShader::Destroy();
    LightShader::Destroy();
    TextureShader::Destroy();
    ColorShader::Destroy();
}

/*
 * Render : For each registered shader call out to the required rendering methods
 */
bool
RenderSystem::Render()
{
    bool bResult;

    // 3D: Enable Z Buffer
    D3DSystem::SetZBufferEnabled(true);

    bResult = ColorShader::Render();
    if (!bResult) { return false; }

    bResult = TextureShader::Render();
    if (!bResult) { return false; }

    bResult = LightShader::Render();
    if (!bResult) { return false; }

    // 2D: Disable Z Buffer
    D3DSystem::SetZBufferEnabled(false);

    bResult = BitMapShader::Render();
    if (!bResult) { return false; }

    return bResult;
}
