#pragma once

// Includes...
#include "Renderable.h"

class Model : public Renderable {
public:
    Model();
    virtual ~Model() override {}

    void SetModelHash(Hash uModelHash) { m_uModelHash = uModelHash; }
    void SetTextureHash(Hash uTextureHash) { m_uTextureHash = uTextureHash; }

    virtual bool Init() override;
    virtual void Shutdown();

private:
    Hash m_uModelHash;

    typedef Renderable PARENT;
};

