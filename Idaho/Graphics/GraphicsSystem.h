#pragma once

// Includes...
#include "Singleton.h"

/*
 * GraphicsSystem
 *   Singleton class to form a common interface for graphics and rendering tasks
 */
class GraphicsSystem : public Singleton<GraphicsSystem> {
public:
    static void Render();

protected:
    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    GraphicsSystem() {}
    ~GraphicsSystem() {}

    friend Singleton<GraphicsSystem>;
};

