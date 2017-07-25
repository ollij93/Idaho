// Includes...
#include "TextInput.h"
#include "Graphics/2D/Font.h"
#include "Graphics/2D/Renderable2D.h"
#include "Graphics/2D/Text.h"
#include "InputSystem.h"

TextInput::TextInput()
    : PARENT()
    , m_pxText(nullptr)
    , m_bActive(false)
{
    InputSystem::RegisterTextCallback(InputCallback, this);
    InputSystem::RegisterMouseClickCallback(OnClick, this, LBUTTON_UP);
}

/*
 * Load function for TextInput
 */
template<> TextInput*
LoadSystem::AddToSceneFromElement<TextInput>(tinyxml2::XMLElement* pxElement, Scene* pxScene)
{
    tinyxml2::XMLElement* pxOffsetElement = pxElement->FirstChildElement("Offset");
    tinyxml2::XMLElement* pxPosElement = pxElement->FirstChildElement("Position");
    tinyxml2::XMLElement* pxImageElement = pxElement->FirstChildElement("Image");

    TextInput* pxInput = new TextInput;

    // Load Image
    if (pxImageElement) {
        Renderable2D* pxImage = AddToSceneFromElement<Renderable2D>(pxImageElement, pxScene);
        if (pxImage) {
            GUIElement::GUIChild* pxChild = new GUIElement::GUIChild;
            pxChild->m_pxObject = pxImage;
            pxChild->m_xOffset = Vector2<int>::ZeroVector();

            pxInput->m_lpxChildren.push_back(pxChild);
            pxInput->m_pxImage = pxImage;
        }
    }

    // Create Text item
    Text* pxText = new Text(*pxScene, "PLACEHOLD TEXT", CompileTimeGetHash("Default"));
    GUIElement::GUIChild* pxChild = new GUIElement::GUIChild;
    pxChild->m_pxObject = pxText;
    pxChild->m_xOffset = Vector2<int>::ZeroVector();
    if (pxText && pxOffsetElement) {
        Vector2<int> xOffset = GetFromElement<Vector2<int>>(pxOffsetElement);
        pxText->SetPosition(xOffset);
        pxChild->m_xOffset = xOffset;
    }
    pxInput->m_lpxChildren.push_back(pxChild);
    pxInput->m_pxText = pxText;

    if (pxPosElement) {
        Vector2<int> xPos = GetFromElement<Vector2<int>>(pxPosElement);
        pxInput->SetPosition(xPos);
    }

    return pxInput;
}

/*
 * InputCallback : Handle the callback from the input system for the text input messages
 */
void
TextInput::InputCallback(void* pContext, char cInput)
{
    TextInput* pxInstance = static_cast<TextInput*>(pContext);
    if (pxInstance->m_bActive) {
        pxInstance->HandleInput(cInput);
    }
}

/*
 * OnClick : Activate/Deactive the TextInput instance when the mouse is clicked
 */
void
TextInput::OnClick(void* pContext, MouseMessageType eMsg, int iClickX, int iClickY)
{
    TextInput* pxInstance = static_cast<TextInput*>(pContext);
    // If the instance is already active then deactivate it.
    pxInstance->m_bActive = false;
    if (pxInstance->m_pxImage) {
        if (iClickX >= pxInstance->m_pxImage->GetPosition().x
            && iClickX < pxInstance->m_pxImage->GetWidth() + pxInstance->m_pxImage->GetPosition().x
            && iClickY >= pxInstance->m_pxImage->GetPosition().y
            && iClickY < pxInstance->m_pxImage->GetHeight() + pxInstance->m_pxImage->GetPosition().y) {
            // Click was inside the image so mark as active
            pxInstance->m_bActive = true;
        }
    }
}

/*
 * HandleInput
 */
void
TextInput::HandleInput(char cInput)
{
    std::string sString = m_pxText->GetString();
    switch (cInput) {
        case '\b':
            // Handle backspace
            if (sString.length() > 0) {
                sString.pop_back();
            }
            break;
        default:
            // Check if character in font
            char cOffset = ' ' - FontChars::FONTCHAR_SPACE;
            if (cInput - cOffset < FontChars::NUM_FONTCHARS
                && cInput - cOffset >= 0) {
                sString += cInput;
            }
            break;
    }
    m_pxText->SetString(sString);
}
