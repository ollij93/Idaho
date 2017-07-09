// Includes...
#include "Font.h"
#include "Renderable2D.h"
#include "Core/Assert.h"
#include "LoadSystem.h"

// Macros...
#define MAX_FONT_FILENAME_SIZE 256

// Statics...
std::list<Font*> Font::s_lpxFontList;

Font::Font()
    : m_uHash(uHASH_UNSET)
    , m_uTexHash(uHASH_UNSET)
    , m_uHeight(0)
    , m_axHandles()
{
    s_lpxFontList.push_back(this);
}

Font::~Font() {}

bool
Font::LoadAll()
{
    return LoadSystem::ForFileInDir("Fonts", ".font", CreateFromFile);
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

bool
Font::CreateFromFile(FILE* pxFile)
{
    float fCurrentLeft = 0.f;
    tinyxml2::XMLDocument xDoc;
    xDoc.LoadFile(pxFile);

    tinyxml2::XMLElement* pxFontEle = nullptr;
    pxFontEle = xDoc.FirstChildElement("Font");
    if (pxFontEle) {
        Font* pxFont = new Font();
        pxFont->m_uHash = pxFontEle->HashAttribute("name");
        pxFont->m_uTexHash = pxFontEle->HashAttribute("texture");
        pxFont->m_uHeight = pxFontEle->UnsignedAttribute("height");
        u_int uWidth = pxFontEle->UnsignedAttribute("texturewidth");
        
        tinyxml2::XMLElement* pxCharEle = nullptr;
        pxCharEle = pxFontEle->FirstChildElement("Character");
        while (pxCharEle) {
            const u_int uFontChar = pxCharEle->UnsignedAttribute("char");
            const u_int uCharWidth = pxCharEle->UnsignedAttribute("width");
            float fLeft = fCurrentLeft;
            float fRight = fLeft + (float)uCharWidth / (float)uWidth;
            fCurrentLeft = fRight + (1.f / (float)uWidth);

            pxFont->m_axHandles[uFontChar].m_fLeft = fLeft;
            pxFont->m_axHandles[uFontChar].m_fRight = fRight;
            pxFont->m_axHandles[uFontChar].m_uWidth = uCharWidth;

            pxCharEle = pxCharEle->NextSiblingElement("Character");
        }
    }

    fclose(pxFile);
    return true;
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
