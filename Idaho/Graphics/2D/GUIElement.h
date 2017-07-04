#pragma once

// Includes...
#include "Core/Math.h"
#include "InputSystem.h"

// Forward Declarations...
class Renderable2D;
class Text;
void ToggleConsole(void*, KeyMapping, KeyMessageType);

class GUIElement {
public:
    GUIElement();
    ~GUIElement();

    void SetImage(Renderable2D* pxImage, Vector2<int> xOffset);
    void SetText(Text* pxText, Vector2<int> xOffset);
    void SetPosition(Vector2<int> xPos);

    void OnClick() { ToggleConsole(nullptr, BACKTICK_KEY, UP_MSG); }

    // Statics...
    static void OnClickCallback(void*, MouseMessageType, int, int);

private:
    Vector2<int> m_xPosition;

    Renderable2D* m_pxImage;
    Vector2<int> m_xImageOffset;

    Text* m_pxText;
    Vector2<int> m_xTextOffset;
};

