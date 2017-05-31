#pragma once

class GameSystem {
public:
    static bool Create();
    static void Destroy();

    static bool Run();

protected:
    GameSystem();
    ~GameSystem() {}

private:
    bool Init();
    void Shutdown();

    static GameSystem* s_pxThis;
};
