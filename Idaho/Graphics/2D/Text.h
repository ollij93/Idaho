#pragma once

// External Includes...
#include <list>
#include <string>

// Includes...
#include "UI/Object2D.h"
#include "Core/Types.h"

// Forward Declarations...
class Renderable2D;
class Scene;

class Text : public Object2D {
public:
    Text(Scene& xScene, std::string sString, Hash uFontHash);
    ~Text();

    // Overrides...
    virtual void SetPosition(Vector2<int> xPos, bool bCentred = false) override;

    // Getters & Setters...
    void SetString(std::string sString);
    const char* GetString() const { return m_sString.c_str(); }

private:
    Hash m_uFontHash;
    std::string m_sString;
    std::list<Renderable2D*> m_lpxRenderables;
    Scene* m_pxScene;

    typedef Object2D PARENT;
};

