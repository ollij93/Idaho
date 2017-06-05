#pragma once

// External Includes...
#include <vector>

// Includes...
#include "Singleton.h"
#include "Core/Types.h"

// Forward Declarations...
struct ID3D11ShaderResourceView;

struct TextureHandle{
    Hash m_uHash;
    ID3D11ShaderResourceView* m_pxTexture;
};

class TextureHandler : public Singleton<TextureHandler> {
public:
    static ID3D11ShaderResourceView* GetTextureByHash(Hash uHash);

protected:
    TextureHandler()
        : m_vxTextures()
    {
    }
    ~TextureHandler() {}

    virtual bool Init() override;
    virtual void Shutdown() override;
private:
    std::vector<TextureHandle> m_vxTextures;
    friend Singleton<TextureHandler>;
};

