#pragma once

// Includes
#include "Core/Math.h"

class Object2D {
public:
    Object2D()
        : m_xPos()
        , m_xDim()
    {}
    ~Object2D() {}

    Vector2<int> GetPosition() { return m_xPos; }
    virtual void SetPosition(Vector2<int> xPos, bool bCentred = false) { m_xPos = xPos - ((bCentred)? m_xDim : Vector2<int>::ZeroVector())/2; }
    Vector2<int> GetDimensions() { return m_xDim; }
    virtual void SetDimensions(Vector2<int> xDim) { m_xDim = xDim; }

protected:
    Vector2<int> m_xPos;
    Vector2<int> m_xDim;
};
