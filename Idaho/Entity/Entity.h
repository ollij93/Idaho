#pragma once

// Includes...
#include "Core/Math.h"

class Entity {
public:
    Entity()
        : m_xPosition(Vector3<float>::ZeroVector())
        , m_xOrientation(Matrix3x3::Identity())
    {
    }
    virtual ~Entity() {}

    Vector3<float> GetPosition() const { return m_xPosition; }
    void SetPosition(float x, float y, float z) { m_xPosition = Vector3<float>(x, y, z); }
    void SetPosition(const Vector3<float>& xVector) { m_xPosition = xVector; }
    Matrix3x3 GetOrientation() const { return m_xOrientation; }
    void SetOrientation(const Matrix3x3& xOri) { m_xOrientation = xOri; }

protected:
    Vector3<float> m_xPosition;
    Matrix3x3 m_xOrientation;
};

