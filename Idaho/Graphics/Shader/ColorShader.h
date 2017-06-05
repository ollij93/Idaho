#pragma once

// Includes...
#include "Shader.h"

/*
 * ColorShader
 *   Simple shader setting constant color of pixels
 */
class ColorShader : public Shader<ColorShader> {
protected:
    virtual bool Init() override {
        m_pwszVertexFilename = L"Graphics/Shader/VS/ColorVertexShader.hlsl";
        m_pwszPixelFilename = L"Graphics/Shader/PS/ColorPixelShader.hlsl";

        return PARENT::Init();
    }
private:
    typedef Shader<ColorShader> PARENT;
    friend Singleton<ColorShader>;
};
