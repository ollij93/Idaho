#pragma once

// Includes...
#include "Object.h"
#include "Graphics/Model.h"
#include "Graphics/Shader/LightShader.h"

class StaticObject : public RenderableObject {
public:
    StaticObject(Scene &xScene, u_int uGUID)
        : RenderableObject(xScene, uGUID)
        , m_xModel(xScene)
    {
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

    virtual void AddToRenderList() override { LightShader::AddToRenderList(this); }
    virtual void RemoveFromRenderList() override { LightShader::RemoveFromRenderList(this); }

private:
    Model m_xModel;
    typedef RenderableObject PARENT;
};

