#pragma once

// Includes
#include "Core/Math.h"

class Object2D {
public:
    Object2D()
        : m_xPos()
    {}
    ~Object2D() {}

    Vector2<int> GetPosition() { return m_xPos; }
    virtual void SetPosition(Vector2<int> xPos, bool bCentred = false) { m_xPos = xPos; }

protected:
    Vector2<int> m_xPos;
};
