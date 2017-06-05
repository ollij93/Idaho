// External Includes...
#include <Windows.h>
#include "DirectX/DDSTextureLoader/DDSTextureLoader.h"

// Includes...
#include "TextureHandler.h"
#include "Core/Assert.h"
#include "Graphics/D3DSystem.h"

#define MAX_TEXTURE_FILENAME_SIZE 256

// Statics...
TextureHandler* Singleton<TextureHandler>::s_pxThis = nullptr;

bool
TextureHandler::Init()
{
    // Find all textures in the directory and load them in.
    wchar_t awcFilename[256];
    errno_t iErrno;
    WIN32_FIND_DATAW xFindData;
    TextureHandle xHandle;
    HANDLE hFile = FindFirstFileW(L"Textures/*.dds", &xFindData);
    ASSERT(hFile != INVALID_HANDLE_VALUE, "Failed to find any textures to load.");
    if (hFile == INVALID_HANDLE_VALUE) { return false; }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = wcscpy_s(awcFilename, MAX_TEXTURE_FILENAME_SIZE, L"Textures/");
            if (!iErrno) {
                iErrno = wcscat_s(awcFilename, MAX_TEXTURE_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy model filename to buffer. Filename too long?");
            if (iErrno) { return false; }

            HRESULT hResult = DirectX::CreateDDSTextureFromFile(D3DSystem::GetDevice(),
                awcFilename,
                NULL,
                &(xHandle.m_pxTexture));
            ASSERT(!FAILED(hResult), "Failed to load a texture from file.");
            if (FAILED(hResult)) { return false; }

            xHandle.m_uHash = GetHash(awcFilename);
            m_vxTextures.push_back(xHandle);
        }
    } while (FindNextFileW(hFile, &xFindData));
    FindClose(hFile);

    return true;
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
