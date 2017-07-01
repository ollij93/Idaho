#pragma once

// External Includes...
#include <d3d11.h>
#include <DirectXMath.h>
#include <list>

// Includes...
#include "Singleton.h"
#include "Core/Types.h"

// Forward Declarations...
struct ID3D10Blob;
struct ID3D11DeviceContext;
class Renderable;

template<typename T>
class Shader : public Singleton<T> {
public:
    static bool Render();

    static void AddToRenderList(Renderable* pxRenderable) { if (pxRenderable) { s_lpxRenderableslist.push_back(pxRenderable); } }
    static void RemoveFromRenderList(Renderable* pxRenderable) { s_lpxRenderableslist.remove(pxRenderable); }

protected:
    Shader()
        : m_pxVertexShader(nullptr)
        , m_pxPixelShader(nullptr)
        , m_pxLayout(nullptr)
        , m_pxMatrixBuffer(nullptr)
        , m_pxSamplerState(nullptr)
        , m_pwszVertexFilename(nullptr)
        , m_pwszPixelFilename(nullptr)
    {
    }

    virtual bool Init() override;
    virtual void Shutdown() override;

    virtual bool SetShaderParameters(ID3D11DeviceContext* pxDeviceContext,
        const DirectX::XMMATRIX& xWorldMatrix,
        const DirectX::XMMATRIX& xViewMatrix,
        const DirectX::XMMATRIX& xProjectionMatrix,
        ID3D11ShaderResourceView* pxTexture);
    void RenderShader(u_int uIndexCount);

    // Functions to be overridden for specialized shaders
    virtual bool CreateMatrixBuffer();
    virtual bool CreateVertexInputLayout(ID3D10Blob* pxVertexShaderBuffer);
    virtual bool CreateSamplerState();
    virtual void GetProjectionMatrix(DirectX::XMMATRIX& xProjectionMatrix);
    virtual void GetViewMatrix(DirectX::XMMATRIX& xViewMatrix);

    struct ShaderMatrixBuffer {
        DirectX::XMMATRIX xWorldMatrix;
        DirectX::XMMATRIX xViewMatrix;
        DirectX::XMMATRIX xProjectionMatrix;
    };

    ID3D11VertexShader* m_pxVertexShader;
    ID3D11PixelShader* m_pxPixelShader;
    ID3D11InputLayout* m_pxLayout;
    ID3D11Buffer* m_pxMatrixBuffer;
    ID3D11SamplerState* m_pxSamplerState;

    const wchar_t* m_pwszVertexFilename;
    const wchar_t* m_pwszPixelFilename;

    // Statics...
    static std::list<Renderable*> s_lpxRenderableslist;

private:
    ID3D10Blob* CompileVertexShader();
    ID3D10Blob* CompilePixelShader();

    void OutputShaderErrorMessage(ID3D10Blob* pxErrorMsg);
};
