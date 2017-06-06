#pragma once

//Includes...
#include "Shader.h"
#include "Core/Math.h"

class LightShader : public Shader<LightShader> {
protected:
    LightShader()
        : PARENT()
        , m_pxLightBuffer(nullptr)
    {
    }

    virtual bool Init() override;
    virtual void Shutdown() override;

    virtual bool SetShaderParameters(ID3D11DeviceContext* pxDeviceContext,
        const DirectX::XMMATRIX& xWorldMatrix,
        const DirectX::XMMATRIX& xViewMatrix,
        const DirectX::XMMATRIX& xProjectionMatrix,
        ID3D11ShaderResourceView* pxTexture);

private:
    struct ShaderLightBuffer {
        Color xDiffuseColor;
        Color xAmbientColor;
        Vector3<float> xLightDirection;
        float fPadding;
    };

    ID3D11Buffer* m_pxLightBuffer;

    typedef Shader<LightShader> PARENT;
    friend Singleton<LightShader>;
    friend Shader<LightShader>;
};

