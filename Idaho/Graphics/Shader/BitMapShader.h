#pragma once

// Includes...
#include "Shader.h"

class BitMapShader : public Shader<BitMapShader> {
protected:
    virtual bool Init() override
    {
        m_pwszVertexFilename = L"Graphics/Shader/VS/TextureVertexShader.hlsl";
        m_pwszPixelFilename = L"Graphics/Shader/PS/TexturePixelShader.hlsl";

        return PARENT::Init();
    }

    virtual void GetProjectionMatrix(DirectX::XMMATRIX& xProjectionMatrix) override;
    virtual void GetViewMatrix(DirectX::XMMATRIX& xViewMatrix) override;
private:
    typedef Shader<BitMapShader> PARENT;
    friend PARENT;
    friend Singleton<BitMapShader>;
};

