// External Includes...
#include <D3Dcompiler.h>
#include <fstream>
#include <list>

// Includes...
#include "Shader.h"
#include "Core/Assert.h"
#include "Objects/Camera.h"
#include "Graphics/D3DSystem.h"
#include "Graphics/Renderable.h"

template<typename T>
bool
Shader<T>::Render()
{
    if (!s_pxThis) { return false; }

    ID3D11DeviceContext* pxDeviceContext = D3DSystem::GetDeviceContext();
    ASSERT(pxDeviceContext, "Failed to get the directX device context for rendering shader.");
    if (!pxDeviceContext) { return false; }

    Camera* pxCamera = Camera::GetActive();
    ASSERT(pxCamera, "Failed to get the active camera when rendering a shader");
    if (!pxCamera) { return false; }

    DirectX::XMMATRIX xViewMatrix;
    DirectX::XMMATRIX xProjectionMatrix;
    pxCamera->Render();
    pxCamera->GetProjectionMatrix(xProjectionMatrix);
    pxCamera->GetViewMatrix(xViewMatrix);

    std::list<Renderable*>::const_iterator xIter;
    for (xIter = s_lpxRenderableslist.begin(); xIter != s_lpxRenderableslist.end(); ++xIter) {
        Renderable* pxRenderable = *xIter;
        u_int uIndexCount = pxRenderable->GetIndexCount();

        // Prepare the object for rendering
        pxRenderable->Render();

        // Get the world matrix for the object
        DirectX::XMMATRIX xWorldMatrix = pxRenderable->GetWorldMatrix();
        ID3D11ShaderResourceView* pxTexture = pxRenderable->GetTexture();

        // Render the object with the shader
        bool bResult = s_pxThis->SetShaderParameters(pxDeviceContext,
            xWorldMatrix,
            xViewMatrix,
            xProjectionMatrix,
            pxTexture);
        ASSERT(bResult, "Failed to set shader parameters");
        if (!bResult) { return false; }

        s_pxThis->RenderShader(uIndexCount);
    }

    return true;
}

template<typename T>
bool
Shader<T>::Init()
{
    HRESULT hResult;
    ID3D10Blob* pxErrorMessage = nullptr;
    ID3D10Blob* pxPixelShaderBuffer = nullptr;
    ID3D10Blob* pxVertexShaderBuffer = nullptr;
    ID3D11Device* pxDevice = D3DSystem::GetDevice();

    ASSERT(pxDevice, "Failed to get the directX device when initializing shader.");
    if (!pxDevice) { return false; }

    // Compile the shader code
    pxVertexShaderBuffer = CompileVertexShader();
    ASSERT(pxVertexShaderBuffer, "Failed to compile the vertex shader.");
    if (!pxVertexShaderBuffer) { return false; }

    pxPixelShaderBuffer = CompilePixelShader();
    ASSERT(pxPixelShaderBuffer, "Failed to compile the pixel shader.");
    if (!pxVertexShaderBuffer) { return false; }

    // Create the shaders from the buffers
    hResult = pxDevice->CreateVertexShader(pxVertexShaderBuffer->GetBufferPointer(),
        pxVertexShaderBuffer->GetBufferSize(),
        NULL,
        &m_pxVertexShader);
    ASSERT(!FAILED(hResult), "Failed to create the vertex shader from the buffer.");
    if (FAILED(hResult)) { return false; }

    hResult = pxDevice->CreatePixelShader(pxPixelShaderBuffer->GetBufferPointer(),
        pxPixelShaderBuffer->GetBufferSize(),
        NULL,
        &m_pxPixelShader);
    ASSERT(!FAILED(hResult), "Failed to create the pixel shader from the buffer.");
    if (FAILED(hResult)) { return false; }

    // These functions can be overwritten by individual shaders
    // Create the Matrix buffer and the vertex input layout
    if (!CreateMatrixBuffer()) { return false; }
    if (!CreateVertexInputLayout(pxVertexShaderBuffer)) { return false; }
    if (!CreateSamplerState()) { return false; }

    // Cleanup
    pxVertexShaderBuffer->Release();
    pxVertexShaderBuffer = nullptr;

    pxPixelShaderBuffer->Release();
    pxPixelShaderBuffer = nullptr;

    return true;
}


template<typename T>
void
Shader<T>::Shutdown()
{
    if (m_pxSamplerState) {
        m_pxSamplerState->Release();
        m_pxSamplerState = nullptr;
    }
    if (m_pxLayout) {
        m_pxLayout->Release();
        m_pxLayout = nullptr;
    }
    if (m_pxMatrixBuffer) {
        m_pxMatrixBuffer->Release();
        m_pxMatrixBuffer = nullptr;
    }
    if (m_pxPixelShader) {
        m_pxPixelShader->Release();
        m_pxPixelShader = nullptr;
    }
    if (m_pxVertexShader) {
        m_pxVertexShader->Release();
        m_pxVertexShader = nullptr;
    }
}

template<typename T>
bool
Shader<T>::SetShaderParameters(ID3D11DeviceContext* pxDeviceContext,
    const DirectX::XMMATRIX& xWorldMatrix,
    const DirectX::XMMATRIX& xViewMatrix,
    const DirectX::XMMATRIX& xProjectionMatrix,
    ID3D11ShaderResourceView* pxTexture)
{
    DirectX::XMMATRIX xTransposedWorldMatrix;
    DirectX::XMMATRIX xTransposedViewMatrix;
    DirectX::XMMATRIX xTransposedProjectionMatrix;

    HRESULT hResult;
    D3D11_MAPPED_SUBRESOURCE xMappedResource;
    ShaderMatrixBuffer* pxDataPtr;
    u_int uBufferNumber;

    // Transpose the matrices to prepare them for the shader.
    xTransposedWorldMatrix = DirectX::XMMatrixTranspose(xWorldMatrix);
    xTransposedViewMatrix = DirectX::XMMatrixTranspose(xViewMatrix);
    xTransposedProjectionMatrix = DirectX::XMMatrixTranspose(xProjectionMatrix);

    // Lock the constant buffer so it can be written to.
    hResult = pxDeviceContext->Map(m_pxMatrixBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &xMappedResource);
    ASSERT(!FAILED(hResult), "Failed to map the matrix buffer for a shader.");
    if (FAILED(hResult)) { return false; }

    // Get a pointer to the data in the constant buffer.
    pxDataPtr = (ShaderMatrixBuffer*)xMappedResource.pData;

    // Copy the matrices into the constant buffer.
    pxDataPtr->xWorldMatrix = xTransposedWorldMatrix;
    pxDataPtr->xViewMatrix = xTransposedViewMatrix;
    pxDataPtr->xProjectionMatrix = xTransposedProjectionMatrix;

    // Unlock the constant buffer.
    pxDeviceContext->Unmap(m_pxMatrixBuffer, 0);

    // Set the position of the constant buffer in the vertex shader.
    uBufferNumber = 0;

    // Finanly set the constant buffer in the vertex shader with the updated values.
    pxDeviceContext->VSSetConstantBuffers(uBufferNumber, 1, &m_pxMatrixBuffer);
    // TODO-OJ It may be possible to make this general (not specific to a single texture)
    pxDeviceContext->PSSetShaderResources(0, 1, &pxTexture);

    return true;
}

template<typename T>
void
Shader<T>::RenderShader(u_int uIndexCount)
{
    ID3D11DeviceContext* pxDeviceContext = D3DSystem::GetDeviceContext();
    // Set the vertex input layout.
    pxDeviceContext->IASetInputLayout(m_pxLayout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
    pxDeviceContext->VSSetShader(m_pxVertexShader, NULL, 0);
    pxDeviceContext->PSSetShader(m_pxPixelShader, NULL, 0);

    // Set the sampler state in the pixel shader.
    pxDeviceContext->PSSetSamplers(0, 1, &m_pxSamplerState);

    // Render the triangle.
    pxDeviceContext->DrawIndexed(uIndexCount, 0, 0);
}

template<typename T>
void
Shader<T>::OutputShaderErrorMessage(ID3D10Blob* pxErrorMsg)
{
#ifdef DEBUG
    char* pszCompileErrors;
    unsigned long long ullBufferSize;
    std::ofstream fout;

    if (pxErrorMsg) {
        pszCompileErrors = (char*)(pxErrorMsg->GetBufferPointer());
        ullBufferSize = pxErrorMsg->GetBufferSize();
        fout.open("shader-error.txt");

        for (unsigned long long ull = 0; ull < ullBufferSize; ull++) {
            fout << pszCompileErrors[ull];
        }

        fout.close();
    }
#endif

    if (pxErrorMsg) {
        pxErrorMsg->Release();
        pxErrorMsg = nullptr;
    }

    ASSERT(false, "Error compiling shader. Check shader-error.txt for message.");
}

template<typename T>
ID3D10Blob*
Shader<T>::CompileVertexShader()
{
    HRESULT hResult;
    ID3D10Blob* pxErrorMessage = nullptr;
    ID3D10Blob* pxVertexShaderBuffer = nullptr;

    hResult = D3DCompileFromFile(m_pwszVertexFilename,
        NULL,
        NULL,
        "main",
        "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        &pxVertexShaderBuffer,
        &pxErrorMessage);
    if (FAILED(hResult)) {
        OutputShaderErrorMessage(pxErrorMessage);
        return nullptr;
    }

    return pxVertexShaderBuffer;
}

template<typename T>
ID3D10Blob*
Shader<T>::CompilePixelShader()
{
    HRESULT hResult;
    ID3D10Blob* pxErrorMessage = nullptr;
    ID3D10Blob* pxPixelShaderBuffer = nullptr;

    hResult = D3DCompileFromFile(m_pwszPixelFilename,
        NULL,
        NULL,
        "main",
        "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        &pxPixelShaderBuffer,
        &pxErrorMessage);
    if (FAILED(hResult)) {
        OutputShaderErrorMessage(pxErrorMessage);
        return nullptr;
    }

    return pxPixelShaderBuffer;
}

template<typename T>
bool
Shader<T>::CreateMatrixBuffer()
{
    HRESULT hResult;
    ID3D11Device* pxDevice = D3DSystem::GetDevice();
    ASSERT(pxDevice, "Failed to get the device from the D3DSystem.");
    if (!pxDevice) { return false; }

    D3D11_BUFFER_DESC xMatrixBufferDesc;
    ZeroMemory(&xMatrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

    // Setup the description of the matrix buffer for the vertex shader
    xMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    xMatrixBufferDesc.ByteWidth = sizeof(ShaderMatrixBuffer);
    xMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    xMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    // Create the matrix buffer from the description
    hResult = pxDevice->CreateBuffer(&xMatrixBufferDesc,
        NULL,
        &m_pxMatrixBuffer);
    ASSERT(!FAILED(hResult), "Failed to create matrix buffer for the vertex shader");
    if (FAILED(hResult)) { return false; }

    return true;
}

template<typename T>
bool
Shader<T>::CreateVertexInputLayout(ID3D10Blob* pxVertexShaderBuffer)
{
    HRESULT hResult;
    ID3D11Device* pxDevice = D3DSystem::GetDevice();
    ASSERT(pxDevice, "Failed to get the device from the D3DSystem.");
    if (!pxDevice) { return false; }

    const u_int uNumElements = 3;
    D3D11_INPUT_ELEMENT_DESC axPolygonLayout[uNumElements];
    ZeroMemory(&axPolygonLayout, sizeof(axPolygonLayout));

    axPolygonLayout[0].SemanticName = "POSITION";
    axPolygonLayout[0].SemanticIndex = 0;
    axPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    axPolygonLayout[0].InputSlot = 0;
    axPolygonLayout[0].AlignedByteOffset = 0;
    axPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    axPolygonLayout[0].InstanceDataStepRate = 0;

    axPolygonLayout[1].SemanticName = "TEXCOORD";
    axPolygonLayout[1].SemanticIndex = 0;
    axPolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    axPolygonLayout[1].InputSlot = 0;
    axPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    axPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    axPolygonLayout[1].InstanceDataStepRate = 0;

    axPolygonLayout[2].SemanticName = "NORMAL";
    axPolygonLayout[2].SemanticIndex = 0;
    axPolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    axPolygonLayout[2].InputSlot = 0;
    axPolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    axPolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    axPolygonLayout[2].InstanceDataStepRate = 0;

    // Create the vertex input layout
    hResult = pxDevice->CreateInputLayout(axPolygonLayout,
        uNumElements,
        pxVertexShaderBuffer->GetBufferPointer(),
        pxVertexShaderBuffer->GetBufferSize(),
        &m_pxLayout);
    ASSERT(!FAILED(hResult), "Failed to create the vertex input layout for shader");
    if (FAILED(hResult)) { return false; }

    return true;
}

template<typename T>
bool
Shader<T>::CreateSamplerState()
{
    HRESULT hResult;
    ID3D11Device* pxDevice = D3DSystem::GetDevice();
    ASSERT(pxDevice, "Failed to get the device from the D3DSystem.");
    if (!pxDevice) { return false; }

    D3D11_SAMPLER_DESC xSamplerDesc;
    ZeroMemory(&xSamplerDesc, sizeof(D3D11_SAMPLER_DESC));

    xSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    xSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    xSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    xSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    xSamplerDesc.MipLODBias = 0.0f;
    xSamplerDesc.MaxAnisotropy = 1;
    xSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    xSamplerDesc.MinLOD = 0;
    xSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    hResult = pxDevice->CreateSamplerState(&xSamplerDesc, &m_pxSamplerState);
    if (FAILED(hResult)) { return false; }

    return true;
}

// Include the registry to generate all the specialized class objects from the templated types
#include "ShaderRegistry.h"
