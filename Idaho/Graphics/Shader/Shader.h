#pragma once

// External Includes...
#include <d3d11.h>

// Includes...
#include "Singleton.h"
#include "Core/Assert.h"

template<typename T>
class Shader : public Singleton<T> {
public:
    static bool Render()
    {
        if (!s_pxThis) { return false; }

        ID3D11DeviceContext* pxDeviceContext = D3DSystem::GetDeviceContext();
        ASSERT(pxDeviceContext, "Failed to get the directX device context for rendering shader.");
        if (!pxDeviceContext) { return false; }

        DirectX::XMMATRIX xViewMatrix;
        DirectX::XMMATRIX xProjectionMatrix;
        // TODO : This should all be accessed via the camera
        D3DSystem::GetProjectionMatrix(xProjectionMatrix);
        D3DSystem::GetViewMatrix(xViewMatrix);

        // TODO : This will be a loop of renderables
        u_int uIndexCount = pxRenderable->GetIndexCount();

        DirectX::XMMATRIX xWorldMatrix = DirectX::XMMatrixIdentity();

        pxRenderable->Render();

        bool bResult = s_pxThis->SetShaderParameters(pxDeviceContext,
            xWorldMatrix,
            xViewMatrix,
            xProjectionMatrix);
        ASSERT(bResult, "Failed to set shader parameters");
        if (!bResult) { return false; }

        s_pxThis->RenderShader(pxDeviceContext, uIndexCount);

        return true;
    }

    void OutputShaderErrorMessage(ID3D10Blob* pxErrorMsg)
    {
    #ifdef DEBUG
        char* pszCompileErrors;
        unsigned long long ullBufferSize;
        ofstream fout;

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

protected:
    Shader()
        : m_pxVertexShader(nullptr)
        , m_pxPixelShader(nullptr)
        , m_pxLayout(nullptr)
        , m_pxMatrixBuffer(nullptr)
    {
    }

    /*
     * Init : Initialize the instance of the shader
     */
    virtual bool Init() override
    {
        HRESULT hResult;
        ID3D10Blob* pxErrorMessage = nullptr;
        ID3D10Blob* pxPixelShaderBuffer = nullptr;
        ID3D10Blob* pxVertexShaderBuffer = nullptr;
        ID3D11Device* pxDevice = D3DSystem::GetDevice();

        D3D11_BUFFER_DESC xMatrixBufferDesc;
        ZeroMemory(&xMatrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

        const u_int uNumElements = 2;
        D3D11_INPUT_ELEMENT_DESC axPolygonLayout[uNumElements];
        ZeroMemory(&axPolygonLayout, sizeof(axPolygonLayout));

        ASSERT(pxDevice, "Failed to get the directX device when initializing shader.");
        if (!pxDevice) { return false; }

        // Compile the vertex shader code
        hResult = D3DCompileFromFile(m_pszVertexFilename,
            NULL,
            NULL,
            "main",
            "vs_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &pxVertexShaderBuffer,
            &pxErrorMessage);
        if (FAILED(hResult) {
            OutputShaderErrorMessage(pxErrorMessage);
            return false;
        }

        // Compile the pixel shader code
        hResult = D3DCompileFromFile(m_pszPixelFilename,
            NULL,
            NULL,
            "main",
            "vs_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS,
            0,
            &pxPixelShaderBuffer,
            &pxErrorMessage);
        if (FAILED(hResult) {
            OutputShaderErrorMessage(pxErrorMessage);
            return false;
        }

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

        // TODO-OJ: This next section might need to be overriden so seperate out into other functions

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

        // Create the vertex input layout description
        axPolygonLayout[0].SemanticName = "POSITION";
        axPolygonLayout[0].SemanticIndex = 0;
        axPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        axPolygonLayout[0].InputSlot = 0;
        axPolygonLayout[0].AlignedByteOffset = 0;
        axPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        axPolygonLayout[0].InstanceDataStepRate = 0;

        axPolygonLayout[1].SemanticName = "COLOR";
        axPolygonLayout[1].SemanticIndex = 0;
        axPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        axPolygonLayout[1].InputSlot = 0;
        axPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        axPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        axPolygonLayout[1].InstanceDataStepRate = 0;

        // Create the vertex input layout
        hResult = pxDevice->CreateInputLayout(axPolygonLayout,
            uNumElements,
            pxVertexShaderBuffer->GetBufferPointer(),
            pxVertexShaderBuffer->GetBufferSize(),
            &m_pxLayout);
        ASSERT(!FAILED(hResult), "Failed to create the vertex input layout for shader");
        if (FAILED(hResult)) { return false; }

        // Cleanup
        pxVertexShaderBuffer->Release();
        pxVertexShaderBuffer = nullptr;

        pxPixelShaderBuffer->Release();
        pxPixelShaderBuffer = nullptr;

        return true;
    }

    /*
     * Shutdown : Shutdown the instance of the shader
     */
    virtual void Shutdown() override
    {
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

    struct ShaderMatrixBuffer {
        DirectX::XMMATRIX xWorldMatrix;
        DirectX::XMMATRIX xViewMatrix;
        DirectX::XMMATRIX xProjectionMatrix;
    };

    ID3D11VertexShader* m_pxVertexShader;
    ID3D11PixelShader* m_pxPixelShader;
    ID3D11InputLayout* m_pxLayout;
    ID3D11Buffer* m_pxMatrixBuffer;

    const char* const m_pszVertexFilename;
    const char* const m_pszPixelFilename;
};