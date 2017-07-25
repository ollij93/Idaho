#pragma once

// Includes...
#include "GUIElement.h"
#include "Graphics/2D/Text.h"

// Forward Declarations...
enum MouseMessageType;

class TextInput : public GUIElement {
public:
    TextInput();
    ~TextInput() {}

    static void InputCallback(void* pContext, char cInput);
    static void OnClick(void* pContext, MouseMessageType eMsg, int iClickX, int iClickY);

    // Getters & Setters...
    void SetString(std::string sString) { m_pxText->SetString(sString); }
    std::string GetString() const { return m_pxText->GetString(); }

private:
    void HandleInput(char cInput);

    Text* m_pxText;
    Renderable2D* m_pxImage;
    bool m_bActive;

    friend LoadSystem;
    typedef GUIElement PARENT;
};

