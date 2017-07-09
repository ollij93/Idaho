// Includes...
#include "GUIElement.h"
#include "Scene.h"
#include "Graphics/2D/Renderable2D.h"
#include "Graphics/2D/Text.h"

/*
 * Load function for GUIElements
 */
template<> GUIElement*
LoadSystem::AddToSceneFromElement<GUIElement>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    tinyxml2::XMLElement* pxOffsetElement = nullptr;
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");

    GUIElement* pxGUI = new GUIElement;

    // Load Images
    for (tinyxml2::XMLElement* pxImageElement = pxElement->FirstChildElement("Image");
        pxImageElement;
        pxImageElement = pxImageElement->NextSiblingElement("Image")) {
        Renderable2D* pxImage = AddToSceneFromElement<Renderable2D>(pxImageElement, pxScene);
        if (pxImage) {
            Vector2<int> xOffset = Vector2<int>::ZeroVector();
            pxOffsetElement = pxImageElement->FirstChildElement("Offset");
            if (pxOffsetElement) {
                xOffset = GetFromElement<Vector2<int>>(pxOffsetElement);
            }
            pxImage->SetPosition(xOffset);

            GUIElement::GUIChild* pxChild = new GUIElement::GUIChild;
            pxChild->m_pxObject = pxImage;
            pxChild->m_xOffset = xOffset;

            pxGUI->m_lpxChildren.push_back(pxChild);
        }
    }

    // Load Text
    for (tinyxml2::XMLElement* pxTextElement = pxElement->FirstChildElement("Text");
        pxTextElement;
        pxTextElement = pxTextElement->NextSiblingElement("Text")) {
        Text* pxText = AddToSceneFromElement<Text>(pxTextElement, pxScene);
        if (pxText) {
            Vector2<int> xOffset = Vector2<int>::ZeroVector();
            pxOffsetElement = pxTextElement->FirstChildElement("Offset");
            if (pxOffsetElement) {
                xOffset = GetFromElement<Vector2<int>>(pxOffsetElement);
            }
            pxText->SetPosition(xOffset);

            GUIElement::GUIChild* pxChild = new GUIElement::GUIChild;
            pxChild->m_pxObject = pxText;
            pxChild->m_xOffset = xOffset;

            pxGUI->m_lpxChildren.push_back(pxChild);
        }
    }

    // Load Child elements
    for (tinyxml2::XMLElement* pxChildElement = pxElement->FirstChildElement("GUIElement");
        pxChildElement;
        pxChildElement = pxChildElement->NextSiblingElement("GUIElement")) {
        GUIElement* pxChildGUI = AddToSceneFromElement<GUIElement>(pxChildElement, pxScene);
        if (pxChildGUI) {
            Vector2<int> xOffset = Vector2<int>::ZeroVector();
            pxOffsetElement = pxChildElement->FirstChildElement("Offset");
            if (pxOffsetElement) {
                xOffset = GetFromElement<Vector2<int>>(pxOffsetElement);
            }
            pxChildGUI->SetPosition(xOffset);

            GUIElement::GUIChild* pxChild = new GUIElement::GUIChild;
            pxChild->m_pxObject = pxChildGUI;
            pxChild->m_xOffset = xOffset;

            pxGUI->m_lpxChildren.push_back(pxChild);
        }
    }

    if (pxPosElement) {
        Vector2<int> xPos = GetFromElement<Vector2<int>>(pxPosElement);
        pxGUI->SetPosition(xPos);
    }

    return pxGUI;
}

/*
 * Set the position of the gui element and all its child objects
 */
void
GUIElement::SetPosition(Vector2<int> xPos, bool bCentred /*= false*/)
{
    PARENT::SetPosition(xPos);
    std::list<GUIChild*>::const_iterator xIter;
    for (xIter = m_lpxChildren.begin();
        xIter != m_lpxChildren.end();
        ++xIter) {
        GUIChild* pxChild = *xIter;
        pxChild->m_pxObject->SetPosition(xPos + pxChild->m_xOffset);
    }
}
