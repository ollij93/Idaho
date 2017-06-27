#pragma once

// Includes...
#include "Renderable.h"

class Model : public Renderable {
public:
    Model(Scene& xScene)
        : Renderable(xScene)
        , m_uModelHash(uHASH_UNSET)
    {}
    virtual ~Model() override {}

    void SetModelHash(Hash uModelHash) { m_uModelHash = uModelHash; }
    void SetTextureHash(Hash uTextureHash) { m_uTextureHash = uTextureHash; }

    virtual bool Init() override;
    virtual void Shutdown();

    virtual void AddToRenderList() override {}
    virtual void RemoveFromRenderList() override {}

private:
    Hash m_uModelHash;

    typedef Renderable PARENT;
};

