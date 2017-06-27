#pragma once

// Includes...
#include "Core/Math.h"
#include "Core/Types.h"
#include "Graphics/TextureHandler.h"
#include "Scene.h"

// Forward Declarations...
struct ID3D11Buffer;

struct Vertex {
    Vector3<float> m_xPosition;
    Vector2<float> m_xTexture;
    Vector3<float> m_xNormal;
};

class Renderable {
public:
    Renderable(Scene& xScene)
        : m_uIndexCount(0)
        , m_uVertexCount(0)
        , m_pxVertices(nullptr)
        , m_pxVertexBuffer(nullptr)
        , m_pxIndexBuffer(nullptr)
        , m_bActive(true)
        , m_pxScene(&xScene)
        , m_uTextureHash(uHASH_UNSET)
    {
        xScene.AddToRenderableList(this);
    }
    virtual ~Renderable() {
        m_pxScene->RemoveFromRenderableList(this);
    }

    virtual bool Init();
    virtual void Shutdown();
    virtual void Render();

    virtual void AddToRenderList() = 0;
    virtual void RemoveFromRenderList() = 0;

    // Getters & Setters...
    virtual u_int GetIndexCount() const { return m_uIndexCount; }
    virtual DirectX::XMMATRIX GetWorldMatrix() const { return DirectX::XMMatrixIdentity(); }
    virtual ID3D11ShaderResourceView* GetTexture() const { return TextureHandler::GetTextureByHash(m_uTextureHash); }
    virtual void SetTextureHash(Hash uTextureHash) { m_uTextureHash = uTextureHash; }
    void SetActive(bool bActive);

protected:
    u_int m_uIndexCount;
    u_int m_uVertexCount;
    Vertex* m_pxVertices;
    ID3D11Buffer* m_pxVertexBuffer;
    ID3D11Buffer* m_pxIndexBuffer;
    bool m_bActive;
    Scene* m_pxScene;
    Hash m_uTextureHash;
};
