#pragma once

// External Includes...
#include <list>

// Includes...
#include "UI/Object2D.h"
#include "Core/Types.h"

// Forward Declarations...
class Renderable2D;
class Scene;

class Text : public Object2D {
public:
    Text(Scene& xScene, const char* pszString, Hash uFontHash);
    ~Text();

    virtual void SetPosition(Vector2<int> xPos, bool bCentred = false) override;

private:
    std::list<Renderable2D*> m_lpxRenderables;
    typedef Object2D PARENT;
};

