// Includes...
#include "Renderable.h"
#include "Core/Assert.h"
#include "Graphics/D3DSystem.h"

bool
Renderable::Init()
{
    HRESULT hResult;

    D3D11_BUFFER_DESC xVertexBufferDesc;
    ZeroMemory(&xVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    D3D11_SUBRESOURCE_DATA xVertexData;
    ZeroMemory(&xVertexData, sizeof(D3D11_SUBRESOURCE_DATA));

    D3D11_BUFFER_DESC xIndexBufferDesc;
    ZeroMemory(&xIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));

    D3D11_SUBRESOURCE_DATA xIndexData;
    ZeroMemory(&xIndexData, sizeof(D3D11_SUBRESOURCE_DATA));

    ID3D11Device* pxDevice = D3DSystem::GetDevice();
    ASSERT(pxDevice, "Failed to get the directX device to initialize a renderable");
    if (!pxDevice) { return false; }

    Vertex* pxVertices = new Vertex[m_uVertexCount];
    u_int* puIndices = new u_int[m_uIndexCount];
    if (!pxVertices || !puIndices) { return false; }

    for (u_int u = 0; u < m_uVertexCount; u++) {
        pxVertices[u] = m_pxVertices[u];
        puIndices[u] = u;
    }

    // Create the Vertex Buffer
    {
        xVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        xVertexBufferDesc.ByteWidth = sizeof(Vertex) * m_uVertexCount;
        xVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        xVertexData.pSysMem = pxVertices;

        hResult = pxDevice->CreateBuffer(&xVertexBufferDesc,
            &xVertexData,
            &m_pxVertexBuffer);
        ASSERT(!FAILED(hResult), "Failed to create vertex buffer for a renderable");
        if (FAILED(hResult)) { return false; }
    }

    // Create the Index Buffer
    {
        xIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        xIndexBufferDesc.ByteWidth = sizeof(u_int) * m_uIndexCount;
        xIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        xIndexData.pSysMem = puIndices;

        hResult = pxDevice->CreateBuffer(&xIndexBufferDesc,
            &xIndexData,
            &m_pxIndexBuffer);
        ASSERT(!FAILED(hResult), "Failed to create index buffer for a renderable");
        if (FAILED(hResult)) { return false; }
    }

    delete[] pxVertices;
    pxVertices = nullptr;

    delete[] puIndices;
    puIndices = nullptr;

    return true;
}

void
Renderable::Shutdown()
{
    if (m_pxIndexBuffer) {
        m_pxIndexBuffer->Release();
        m_pxIndexBuffer = nullptr;
    }
    if (m_pxVertexBuffer) {
        m_pxVertexBuffer->Release();
        m_pxVertexBuffer = nullptr;
    }
}

void
Renderable::Render()
{
    ID3D11DeviceContext* pxDeviceContext = D3DSystem::GetDeviceContext();
    ASSERT(pxDeviceContext, "Failed to get the device context for rendering a renderable.");
    if (!pxDeviceContext) { return; }

    u_int uSize = sizeof(Vertex);
    u_int uOffset = 0;

    pxDeviceContext->IASetVertexBuffers(0, 1, &m_pxVertexBuffer, &uSize, &uOffset);
    pxDeviceContext->IASetIndexBuffer(m_pxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    pxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
