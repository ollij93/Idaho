#pragma once

// Includes...
#include "Singleton.h"

/*
 * GameSystem
 *   The main singleton class for controlling the game subsystems
 */
class GameSystem : public Singleton<GameSystem> {
public:
    static bool Run();

protected:
    GameSystem();
    ~GameSystem() {}

protected:
    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    friend Singleton<GameSystem>;
};
