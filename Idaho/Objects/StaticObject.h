#pragma once

// Includes...
#include "RenderableObject.h"
#include "Graphics/Model.h"
#include "Graphics/Shader/LightShader.h"

class StaticObject : public RenderableObject {
public:
    StaticObject(rp3d::CollisionWorld &xWorld, u_int uGUID)
        : Object(xWorld, uGUID)
        , RenderableObject(xWorld, uGUID)
        , m_xModel()
    {
        LightShader::AddToRenderList(this);
    }
    ~StaticObject()
    {
        m_xModel.Shutdown();
    }

    // Overrides...
    virtual void Render() override { m_xModel.Render(); }
    virtual u_int GetIndexCount() const override { return m_xModel.GetIndexCount(); }
    virtual ID3D11ShaderResourceView* GetTexture() const override { return m_xModel.GetTexture(); }
    virtual void SetTextureHash(Hash uTextureHash) override { m_xModel.SetTextureHash(uTextureHash); }

    virtual void InitFromSpecification(const Specification* pxSpecification) override;
    virtual void* GetProxyShapeData() override;

private:
    Model m_xModel;
    typedef RenderableObject PARENT;
};

