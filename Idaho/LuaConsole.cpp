// Includes...
#include "LuaConsole.h"
#include "Core/Assert.h"

// Macro defines...
#define LUA_REG(__name) \
lua_register(m_pxState, #__name, lua_##__name)

#define LUA_FUNC(__type, __name) \
__type __name(); \
int lua_##__name(lua_State* pxState) \
{ \
    __type tVal = __name(); \
    LuaConsole::lua_push<__type>(pxState, tVal); \
    return 1; \
} \
__type __name()

// Statics...
LuaConsole* Singleton<LuaConsole>::s_pxThis = nullptr;

// Template functions...
template<> void
LuaConsole::lua_push<int>(lua_State* pxState, int tVal)
{
    lua_pushinteger(pxState, tVal);
}

// Console methods...
LUA_FUNC(int, GetLockpickLevel)
{
    return 6;
}

/*
 * RegisterLuaFunctions : Register and compile the functions that are
 *                        callable from the lua console
 */
void
LuaConsole::RegisterLuaFunctions()
{
    LUA_REG(GetLockpickLevel);
}

/*
 * Init : Initialize the instance of the console
 */
bool
LuaConsole::Init()
{
    m_pxState = luaL_newstate();
    ASSERT(m_pxState, "Failed to create the lua state object.");
    if (!m_pxState) { return false; }

    luaL_openlibs(m_pxState);

    RegisterLuaFunctions();

    return true;
}

/*
 * Shutdown : Shutdown the instance of the console
 */
void
LuaConsole::Shutdown()
{
    if (m_pxState) {
        lua_close(m_pxState);
        m_pxState = nullptr;
    }
}
