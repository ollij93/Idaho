// External Includes...
#include <Windows.h>
#include "DirectX/DDSTextureLoader/DDSTextureLoader.h"

// Includes...
#include "TextureHandler.h"
#include "Core/Assert.h"
#include "Graphics/D3DSystem.h"
#include "LoadSystem.h"

#define MAX_TEXTURE_FILENAME_SIZE 256

// Statics...
TextureHandler* Singleton<TextureHandler>::s_pxThis = nullptr;

bool
TextureHandler::Init()
{
    // Find all textures in the directory and load them in.
    return LoadSystem::ForFileInDir("Textures", "dds", LoadTextureFromFile);
}

void
TextureHandler::Shutdown()
{
    for (u_int u = 0; u < m_vxTextures.size(); u++) {
        if (m_vxTextures[u].m_pxTexture) {
            m_vxTextures[u].m_pxTexture->Release();
            m_vxTextures[u].m_pxTexture = nullptr;
        }
    }
    m_vxTextures.clear();
}

ID3D11ShaderResourceView*
TextureHandler::GetTextureByHash(Hash uHash)
{
    ASSERT(s_pxThis, "Attempting to GetTexture before creating the texture handler.");
    if (!s_pxThis) { return nullptr; }

    for (u_int u = 0; u < s_pxThis->m_vxTextures.size(); u++) {
        if (s_pxThis->m_vxTextures[u].m_uHash == uHash) {
            return s_pxThis->m_vxTextures[u].m_pxTexture;
        }
    }
    return nullptr;
}

bool
TextureHandler::LoadTextureFromFile(const char* pszFilename)
{
    TextureHandle xHandle;
    WCHAR* pwszFilename = new WCHAR[MAX_FILENAME_SIZE];
    mbstowcs_s(nullptr, pwszFilename, MAX_FILENAME_SIZE, pszFilename, MAX_FILENAME_SIZE);

    HRESULT hResult = DirectX::CreateDDSTextureFromFile(D3DSystem::GetDevice(),
        pwszFilename,
        nullptr,
        &(xHandle.m_pxTexture));
    ASSERT(!FAILED(hResult), "Failed to load a texture from file.");
    if (FAILED(hResult)) { return false; }

    xHandle.m_uHash = GetHash(pszFilename);
    s_pxThis->m_vxTextures.push_back(xHandle);
    return true;
}
