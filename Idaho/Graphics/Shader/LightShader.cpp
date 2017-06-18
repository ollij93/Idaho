// Includes...
#include "LightShader.h"
#include "Core/Assert.h"
#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Graphics/D3DSystem.h"
#include "Graphics/Renderable.h"

// Statics...
LightShader* Singleton<LightShader>::s_pxThis = nullptr;
std::list<Renderable*> Shader<LightShader>::s_lpxRenderableslist;

bool
LightShader::Init()
{
    m_pwszVertexFilename = L"Graphics/Shader/VS/LightVertexShader.hlsl";
    m_pwszPixelFilename = L"Graphics/Shader/PS/LightPixelShader.hlsl";

    if (!PARENT::Init()) { return false; }

    HRESULT hResult;
    D3D11_BUFFER_DESC xLightBufferDesc;
    ZeroMemory(&xLightBufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11_BUFFER_DESC xCameraBufferDesc;
    ZeroMemory(&xCameraBufferDesc, sizeof(D3D11_BUFFER_DESC));

    ID3D11Device* pxDevice = D3DSystem::GetDevice();
    ASSERT(pxDevice, "Failed to get the device from the D3DSystem.");
    if (!pxDevice) { return false; }

    xLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    xLightBufferDesc.ByteWidth = sizeof(ShaderLightBuffer);
    xLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    xLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    hResult = pxDevice->CreateBuffer(&xLightBufferDesc,
        NULL,
        &m_pxLightBuffer);
    ASSERT(SUCCEEDED(hResult), "Failed to create the light buffer for the light shader.");
    if (FAILED(hResult)) { return false; }

    xCameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    xCameraBufferDesc.ByteWidth = sizeof(DirectX::XMVECTOR);
    xCameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    xCameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hResult = pxDevice->CreateBuffer(&xCameraBufferDesc,
        NULL,
        &m_pxCameraBuffer);
    ASSERT(SUCCEEDED(hResult), "Failed to create the camera buffer for the light shader.");
    if (FAILED(hResult)) { return false; }

    return true;
}

void
LightShader::Shutdown()
{
    if (m_pxLightBuffer) {
        m_pxLightBuffer->Release();
        m_pxLightBuffer = nullptr;
    }

    PARENT::Shutdown();
}

bool
LightShader::SetShaderParameters(ID3D11DeviceContext* pxDeviceContext,
    const DirectX::XMMATRIX& xWorldMatrix,
    const DirectX::XMMATRIX& xViewMatrix,
    const DirectX::XMMATRIX& xProjectionMatrix,
    ID3D11ShaderResourceView* pxTexture)
{
    PARENT::SetShaderParameters(pxDeviceContext,
        xWorldMatrix,
        xViewMatrix,
        xProjectionMatrix,
        pxTexture);

    HRESULT hResult;
    D3D11_MAPPED_SUBRESOURCE xMappedResource;
    ShaderLightBuffer* pxDataPtr;
    DirectX::XMVECTOR* pxDataPtrCamera;
    Light* pxLight = Light::GetActive();
    Camera* pxCamera = Camera::GetActive();
    ASSERT(pxLight, "Failed to get the active light for rendering the light shader.");

    // Light Buffer
    hResult = pxDeviceContext->Map(m_pxLightBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &xMappedResource);
    ASSERT(SUCCEEDED(hResult), "Failed to map the light buffer for the light shader.");
    if (FAILED(hResult)) { return false; }

    pxDataPtr = (ShaderLightBuffer*)xMappedResource.pData;

    pxDataPtr->xDiffuseColor = pxLight->GetDiffuseColor();
    pxDataPtr->xAmbientColor = pxLight->GetAmbientColor();
    pxDataPtr->xLightDirection = pxLight->GetOrientation().Column(2);
    pxDataPtr->xSpecularColor = pxLight->GetSpecularColor();
    pxDataPtr->fSpecularPower = 32.f;

    pxDeviceContext->Unmap(m_pxLightBuffer, 0);

    pxDeviceContext->PSSetConstantBuffers(0, 1, &m_pxLightBuffer);

    // Camera Buffer
    hResult = pxDeviceContext->Map(m_pxCameraBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &xMappedResource);
    ASSERT(SUCCEEDED(hResult), "Failed to map the camera buffer for the light shader.");
    if (FAILED(hResult)) { return false; }

    pxDataPtrCamera = (DirectX::XMVECTOR*)xMappedResource.pData;

    *pxDataPtrCamera = pxCamera->GetPosition();

    pxDeviceContext->Unmap(m_pxCameraBuffer, 0);

    pxDeviceContext->VSSetConstantBuffers(1, 1, &m_pxCameraBuffer);

    return true;
}
