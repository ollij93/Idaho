// Includes...
#include "GUIElement.h"
#include "Graphics/2D/Renderable2D.h"
#include "Graphics/2D/Text.h"

GUIElement::GUIElement()
    : m_pxImage(nullptr)
    , m_pxText(nullptr)
{
    InputSystem::RegisterMouseClickCallback(GUIElement::OnClickCallback, this, LBUTTON_UP);
}

GUIElement::~GUIElement()
{
    if (m_pxText) {
        delete m_pxText;
        m_pxText = nullptr;
    }
    if (m_pxImage) {
        m_pxImage->Shutdown();
        delete m_pxImage;
        m_pxImage = nullptr;
    }
}

void
GUIElement::SetImage(Renderable2D* pxImage, Vector2<int> xOffset)
{
    m_pxImage = pxImage;
    m_xImageOffset = xOffset;

    if (m_pxImage) {
        m_pxImage->SetPosition(m_xPosition + m_xImageOffset);
    }
}

void
GUIElement::SetText(Text* pxText, Vector2<int> xOffset)
{
    m_pxText = pxText;
    m_xTextOffset = xOffset;

    if (m_pxText) {
        m_pxText->SetPosition(m_xPosition + m_xTextOffset);
    }
}

void
GUIElement::SetPosition(Vector2<int> xPos)
{
    m_xPosition = xPos;

    if (m_pxImage) {
        m_pxImage->SetPosition(m_xPosition + m_xImageOffset);
    }
    if (m_pxText) {
        m_pxText->SetPosition(m_xPosition + m_xTextOffset);
    }
}

void
GUIElement::OnClickCallback(void* pObject, MouseMessageType eMsg, int iClickX, int iClickY)
{
    GUIElement* pxElement = static_cast<GUIElement*>(pObject);
    bool bHit = false;
    if (pxElement && pxElement->m_pxImage) {
        if (iClickX > pxElement->m_pxImage->GetPosition().x
            && iClickX < pxElement->m_pxImage->GetWidth() + pxElement->m_pxImage->GetPosition().x
            && iClickY > pxElement->m_pxImage->GetPosition().y
            && iClickY < pxElement->m_pxImage->GetHeight() + pxElement->m_pxImage->GetPosition().y) {
            bHit = true;
        }
    }
    if (bHit) {
        pxElement->OnClick();
    }
}
