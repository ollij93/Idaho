#pragma once

// Includes...
#include "Graphics/Renderable.h"
#include "Graphics/Shader/BitMapShader.h"

class Renderable2D : public Renderable {
public:
    Renderable2D(Scene& xScene, u_int uWidth, u_int uHeight, float fTextureLeft = 0.f, float fTextureRight = 1.f, float fTextureTop = 1.f, float fTextureBottom = 0.f)
        : PARENT(xScene)
        , m_xDimensions(uWidth, uHeight)
        , m_fTextureLeft(fTextureLeft)
        , m_fTextureRight(fTextureRight)
        , m_fTextureTop(fTextureTop)
        , m_fTextureBottom(fTextureBottom)
        , m_xPos()
    {}
    ~Renderable2D() {}

    // Overrides...
    virtual bool Init() override;
    virtual void AddToRenderList() override { BitMapShader::AddToRenderList(this); }
    virtual void RemoveFromRenderList() override { BitMapShader::RemoveFromRenderList(this); }
    virtual DirectX::XMMATRIX GetWorldMatrix() const override;

    // Getters & Setters...
    Vector2<int> GetPosition() const { return m_xPos; }
    void SetPosition(Vector2<int> xPos, bool bCentre = false);
    int GetWidth() const { return m_xDimensions.x; }
    int GetHeight() const { return m_xDimensions.y; }

private:
    Vector2<u_int> m_xDimensions;
    float m_fTextureLeft;
    float m_fTextureRight;
    float m_fTextureTop;
    float m_fTextureBottom;

    Vector2<int> m_xPos;

    typedef Renderable PARENT;
};

