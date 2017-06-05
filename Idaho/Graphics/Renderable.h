#pragma once

// Includes...
#include "Core/Math.h"
#include "Core/Types.h"
#include "Graphics/TextureHandler.h"

// Forward Declarations...
struct ID3D11Buffer;

struct Vertex {
    Vector3<float> m_xPosition;
    Vector2<float> m_xTexture;
    Vector3<float> m_xNormal;
};

class Renderable {
public:
    Renderable()
        : m_uIndexCount(0)
        , m_uVertexCount(0)
        , m_pxVertices(nullptr)
        , m_pxVertexBuffer(nullptr)
        , m_pxIndexBuffer(nullptr)
        , m_uTextureHash(uHASH_UNSET)
    {
    }
    virtual ~Renderable() {}

    virtual bool Init();
    virtual void Shutdown();
    void Render();

    // Getters & Setters...
    u_int GetIndexCount() const { return m_uIndexCount; }
    virtual DirectX::XMMATRIX GetWorldMatrix() const { return DirectX::XMMatrixIdentity(); }
    virtual ID3D11ShaderResourceView* GetTexture() const { return TextureHandler::GetTextureByHash(m_uTextureHash); }

protected:
    u_int m_uIndexCount;
    u_int m_uVertexCount;
    Vertex* m_pxVertices;
    ID3D11Buffer* m_pxVertexBuffer;
    ID3D11Buffer* m_pxIndexBuffer;

    Hash m_uTextureHash;
};
