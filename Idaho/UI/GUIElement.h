#pragma once

// Includes...
#include "Object2D.h"
#include "LoadSystem.h"

// Forward Declarations...
class Text;
class Renderable2D;
enum MouseMessageType;

class GUIElement : public Object2D {
public:
    GUIElement()
        : m_vfnOnClick(nullptr)
        , m_iClickData(0)
        , m_lpxChildren()
    {}
    ~GUIElement() {}

    static void OnClick(void* pContext, MouseMessageType eMsg, int iClickX, int iClickY);

    virtual void SetPosition(Vector2<int> xPos, bool bCentred = false) override;

protected:
    void(*m_vfnOnClick)(int);
    int m_iClickData;
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

