#pragma once

// Includes...
#include "Singleton.h"

/*
 * RenderSystem
 *   Singleton class for controlling the shaders and rendering process
 */
class RenderSystem : public Singleton<RenderSystem> {
public:
    static bool Render();

protected:
    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    RenderSystem() {}
    ~RenderSystem() {}

    friend Singleton<RenderSystem>;
};

