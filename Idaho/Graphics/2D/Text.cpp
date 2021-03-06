// Includes...
#include "Text.h"
#include "Graphics/2D/Font.h"
#include "Graphics/2D/Renderable2D.h"
#include "LoadSystem.h"

template<> Text*
LoadSystem::AddToSceneFromElement<Text>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    Hash uFontHash = pxElement->HashAttribute("font");
    tinyxml2::XMLElement* pxStringElement = pxElement->FirstChildElement("String");
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");
    const char* pszString = nullptr;
    if (pxStringElement) {
        pszString = pxStringElement->GetText();
    }

    Text* pxText = new Text(*pxScene, pszString, uFontHash);

    if (pxPosElement) {
        Vector2<int> xPos = GetFromElement<Vector2<int>>(pxPosElement);
        pxText->SetPosition(xPos);
    }

    return pxText;
}

Text::Text(Scene& xScene, std::string sString, Hash uFontHash)
    : PARENT()
    , m_uFontHash(uFontHash)
    , m_sString()
    , m_lpxRenderables()
    , m_pxScene(&xScene)
{
    SetString(sString);
}

Text::~Text()
{
    while (m_lpxRenderables.size() > 0) {
        Renderable2D* pxRenderable = m_lpxRenderables.back();
        pxRenderable->Shutdown();
        delete pxRenderable;
        m_lpxRenderables.pop_back();
    }
}

void
Text::SetPosition(Vector2<int> xPos, bool bCentred /* = false */)
{
    PARENT::SetPosition(xPos);
    std::list<Renderable2D*>::const_iterator xIter;

    if (m_lpxRenderables.size() == 0) { return; }

    if (bCentred) {
        u_int uWidth = 0;
        for (xIter = m_lpxRenderables.begin(); xIter != m_lpxRenderables.end(); ++xIter) {
            Renderable2D* pxRenderable = *xIter;
            if (pxRenderable) {
                uWidth += pxRenderable->GetWidth() + 1;
            }
        }
        xPos -= Vector2<int>(uWidth / 2, m_lpxRenderables.back()->GetHeight() / 2);
    }

    u_int uCurrWidth = 0;
    for (xIter = m_lpxRenderables.begin(); xIter != m_lpxRenderables.end(); ++xIter) {
        Renderable2D* pxRenderable = *xIter;
        if (pxRenderable) {
            pxRenderable->SetPosition(xPos + Vector2<int>(uCurrWidth, 0));
            uCurrWidth += pxRenderable->GetWidth() + 1;
        }
    }
}

void
Text::SetString(std::string sString)
{
    // Clear existing string
    std::list<Renderable2D*>::const_iterator xIter;
    for (xIter = m_lpxRenderables.begin();
        xIter != m_lpxRenderables.end();
        ++xIter) {
        Renderable2D* pxRenderable = *xIter;
        if (pxRenderable) {
            pxRenderable->Shutdown();
            delete pxRenderable;
            pxRenderable = nullptr;
        }
    }
    m_lpxRenderables.clear();

    // Fill new string
    const Font* pxFont = Font::GetFontByHash(m_uFontHash);
    if (pxFont) {
        u_int uWidthPos = 0;
        for (u_int u = 0; u < sString.length(); u++) {
            Renderable2D* pxRenderable = pxFont->CreateRenderableForCharacter(sString[u], *m_pxScene);
            if (pxRenderable) {
                pxRenderable->SetPosition(Vector2<int>(uWidthPos, 0));
                uWidthPos += pxRenderable->GetWidth() + 1;
                m_lpxRenderables.push_back(pxRenderable);
            }
        }
    }
    m_sString = sString;
    SetPosition(GetPosition());
}
