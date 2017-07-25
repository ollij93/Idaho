#pragma once

// Includes...
#include "Entity.h"
#include "Graphics/Model.h"
#include "Graphics/Shader/LightShader.h"

class SpinningObject : public RenderableEntity {
public:
    SpinningObject(Scene &xScene, u_int uGUID)
        : PARENT(xScene, uGUID)
        , m_xModel(xScene)
        , m_fRate(0.f)
    {
    }
    ~SpinningObject() {}

    // Overrides...
    virtual void Render() override { m_xModel.Render(); }
    virtual u_int GetIndexCount() const override { return m_xModel.GetIndexCount(); }
    virtual ID3D11ShaderResourceView* GetTexture() const override { return m_xModel.GetTexture(); }
    virtual void SetTextureHash(Hash uTextureHash) override { m_xModel.SetTextureHash(uTextureHash); }
    virtual void Update(float fTimestep) override;
    virtual void InitFromSpecification(const Specification* pxSpecification) override;
    virtual void AddToRenderList() override { LightShader::AddToRenderList(this); }
    virtual void RemoveFromRenderList() override { LightShader::RemoveFromRenderList(this); }

    // Getters & Setters...
    void SetRate(float fRate) { m_fRate = fRate; }

private:
    Model m_xModel;
    float m_fRate;

    typedef RenderableEntity PARENT;
};

