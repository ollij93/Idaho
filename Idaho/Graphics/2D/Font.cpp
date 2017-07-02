// External Includes...
#include "TinyXML/tinyxml2.h"

// Includes...
#include "Font.h"
#include "Renderable2D.h"
#include "Core/Assert.h"

// Macros...
#define MAX_FONT_FILENAME_SIZE 256

// Statics...
std::list<Font*> Font::s_lpxFontList;

Font::Font(const char* pszFontname)
    : m_uHash(uHASH_UNSET)
    , m_uTexHash(uHASH_UNSET)
    , m_uHeight(0)
    , m_axHandles()
{
    LoadFromFile(pszFontname);
    s_lpxFontList.push_back(this);
}

Font::~Font() {}

bool
Font::LoadAll()
{
    // Find all textures in the directory and load them in.
    char acFilename[MAX_FONT_FILENAME_SIZE];
    errno_t iErrno;
    WIN32_FIND_DATA xFindData;
    HANDLE hFile = FindFirstFile("Fonts/*.font", &xFindData);
    ASSERT(hFile != INVALID_HANDLE_VALUE, "Failed to find any fonts to load.");
    if (hFile == INVALID_HANDLE_VALUE) { return false; }

    do {
        if (!(xFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            iErrno = strcpy_s(acFilename, MAX_FONT_FILENAME_SIZE, "Fonts/");
            if (!iErrno) {
                iErrno = strcat_s(acFilename, MAX_FONT_FILENAME_SIZE, xFindData.cFileName);
            }
            ASSERT(!iErrno, "Failed to copy font filename to buffer. Filename too long?");
            if (iErrno) { return false; }

            Font* pxFont = new Font(acFilename);
        }
    } while (FindNextFile(hFile, &xFindData));
    FindClose(hFile);

    return true;
}

void
Font::ShutdownAll()
{
    while (s_lpxFontList.size() > 0) {
        Font* pxFont = s_lpxFontList.back();
        delete pxFont;
        pxFont = nullptr;
        s_lpxFontList.pop_back();
    }
}

const Font*
Font::GetFontByHash(Hash uHash)
{
    std::list<Font*>::const_iterator xIter;
    for (xIter = s_lpxFontList.begin(); xIter != s_lpxFontList.end(); ++xIter) {
        Font* pxFont = *xIter;
        if (pxFont && pxFont->m_uHash == uHash) {
            return pxFont;
        }
    }
    return nullptr;
}

void
Font::LoadFromFile(const char* pszFilename)
{
    FILE* pxFile = nullptr;
    fopen_s(&pxFile, pszFilename, "rb");
    ASSERT(pxFile, "Failed to open file for loading font.");
    if (!pxFile) { return; }

    float fCurrentLeft = 0.f;
    tinyxml2::XMLDocument xDoc;
    xDoc.LoadFile(pszFilename);

    tinyxml2::XMLElement* pxFontEle = nullptr;
    pxFontEle = xDoc.FirstChildElement("Font");
    if (pxFontEle) {
        const char* pszName = pxFontEle->Attribute("name");
        m_uHash = GetHash(pszName);

        const char* pszTexName = pxFontEle->Attribute("texture");
        m_uTexHash = GetHash(pszTexName);
        m_uHeight = pxFontEle->UnsignedAttribute("height");
        u_int uWidth = pxFontEle->UnsignedAttribute("texturewidth");
        
        tinyxml2::XMLElement* pxCharEle = nullptr;
        pxCharEle = pxFontEle->FirstChildElement("Character");
        while (pxCharEle) {
            const u_int uFontChar = pxCharEle->UnsignedAttribute("char");
            const u_int uCharWidth = pxCharEle->UnsignedAttribute("width");
            float fLeft = fCurrentLeft;
            float fRight = fLeft + (float)uCharWidth / (float)uWidth;

            m_axHandles[uFontChar].m_fLeft = fLeft;
            m_axHandles[uFontChar].m_fRight = fRight;
            m_axHandles[uFontChar].m_uWidth = uCharWidth;

            fCurrentLeft = fRight + (1.f / (float)uWidth);

            pxCharEle = pxCharEle->NextSiblingElement("Character");
        }
    }

    fclose(pxFile);
}

Renderable2D*
Font::CreateRenderableForCharacter(char cChar, Scene& xScene) const
{
    FontCharHandle xHandle = m_axHandles[cChar - ' '];
    Renderable2D* pxRenderable = new Renderable2D(xScene, xHandle.m_uWidth, m_uHeight, xHandle.m_fLeft, xHandle.m_fRight);
    pxRenderable->SetTextureHash(m_uTexHash);
    bool bSuccess = pxRenderable->Init();
    if (bSuccess) {
        return pxRenderable;
    }

    ASSERT(false, "Failed to initialise a character renderable.");
    pxRenderable->Shutdown();
    delete pxRenderable;
    return nullptr;
}
