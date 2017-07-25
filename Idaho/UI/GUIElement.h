#pragma once

// Includes...
#include "Object2D.h"
#include "LoadSystem.h"

// Forward Declarations...
class Text;
class Renderable2D;

class GUIElement : public Object2D {
public:
    GUIElement()
        : m_lpxChildren()
    {}
    ~GUIElement() {}

    virtual void SetPosition(Vector2<int> xPos, bool bCentred = false) override;

protected:
    Vector2<int> m_xPos;
    struct GUIChild {
        Object2D* m_pxObject;
        Vector2<int> m_xOffset;
    };
    std::list<GUIChild*> m_lpxChildren;

private:
    typedef Object2D PARENT;
    friend LoadSystem;
};

