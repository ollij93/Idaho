#pragma once

// Includes...
#include "Shader.h"

class TextureShader : public Shader<TextureShader> {
protected:
    virtual bool Init() override {
        m_pwszVertexFilename = L"Graphics/Shader/VS/TextureVertexShader.hlsl";
        m_pwszPixelFilename = L"Graphics/Shader/PS/TexturePixelShader.hlsl";

        return PARENT::Init();
    }
private:
    typedef Shader<TextureShader> PARENT;
    friend Singleton<TextureShader>;
};

