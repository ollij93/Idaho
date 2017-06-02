#pragma once

// Includes...
#include "Shader.h"

/*
 * ColorShader
 *   Simple shader setting constant color of pixels
 */
class ColorShader : public Shader<ColorShader> {
protected:
    ColorShader();

    const char* const m_pszVertexFilename = "ColorVertexShader.hlsl";
    const char* const m_pszPixelFilename = "ColorPixelShader.hlsl";
};
