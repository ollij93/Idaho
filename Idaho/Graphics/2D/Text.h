#pragma once

// External Includes...
#include <list>

// Includes...
#include "Core/Math.h"
#include "Core/Types.h"

// Forward Declarations...
class Renderable2D;
class Scene;

class Text {
public:
    Text(Scene& xScene, const char* pszString, Hash uFontHash);
    ~Text();

    void SetPosition(Vector2<int> xPos, bool bCentred = false);
private:
    std::list<Renderable2D*> m_lpxRenderables;
};

