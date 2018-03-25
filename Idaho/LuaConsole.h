#pragma once

// External Includes...
#include <Lua/lua-5.3.4/src/lua.hpp>

// Includes...
#include "Singleton.h"


class LuaConsole : public Singleton<LuaConsole> {
public:
    // Template functions...
    template<typename T> static void lua_push(lua_State* pxState, T tVal);
protected:
    LuaConsole()
        : m_pxState(nullptr)
    {}
    ~LuaConsole() {}

    void RegisterLuaFunctions();

    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    lua_State* m_pxState;
    friend Singleton<LuaConsole>;
};

