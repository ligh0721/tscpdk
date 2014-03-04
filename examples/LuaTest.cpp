#include <tscpdk/TSCPDK.h>

#include "lstate.h"
#include "GameServer/lua/lauxlib.h"

USING_NS_TSCPDK;

class CTest
{
public:
    CTest()
    {
        
    }
    
    void onTrigger()
    {
        
    }
};

int lua_getcopy(lua_State* L, int idx)
{
    lua_pushnil(L);
    int copy = lua_gettop(L);
    lua_copy(L, idx, copy);
    
    return copy;
}

int obj_sctor(lua_State* L)
{
    int obj = 1;
    int n = lua_gettop(L) + 1;
    assert(n > obj);
    
    lua_getfield(L, obj, "super");
    int super = lua_gettop(L);
    
    lua_getfield(L, super, "ctor");
    lua_getcopy(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_getcopy(L, argv);
    }
    lua_call(L, n - obj, 0);
    
    return 0;
}

int obj_ctor(lua_State* L)
{
    return 0;
}

int class_type_new(lua_State* L)
{
    int class_type = 1;
    int n = lua_gettop(L) + 1;
    assert(n >= class_type);
    
    lua_newtable(L);
    int obj = lua_gettop(L);
    
    lua_getcopy(L, class_type);
    lua_setmetatable(L, obj);
    
    lua_getfield(L, obj, "ctor");
    lua_getcopy(L, obj);
    for (int argv = 2; argv < n; ++argv)
    {
        lua_getcopy(L, argv);
    }
    lua_call(L, n - class_type, 0);
    
    return 1;
}

int g_class(lua_State* L)
{
    int super = lua_gettop(L);
    
    lua_newtable(L);
    int t = lua_gettop(L);
    
    if (super)
    {
        lua_getcopy(L, super);
        lua_setfield(L, t, "super");
        lua_getcopy(L, super);
        lua_setmetatable(L, t);
    }
    
    lua_getcopy(L, t);
    lua_setfield(L, t, "__index");
    
    lua_pushcfunction(L, obj_sctor);
    lua_setfield(L, t, "sctor");
    
    lua_pushcfunction(L, obj_ctor);
    lua_setfield(L, t, "ctor");
    
    lua_pushcfunction(L, class_type_new);
    lua_setfield(L, t, "new");
    
    return 1;
}

int regClassFunc(lua_State* L)
{
    lua_pushcfunction(L, g_class);
    lua_setglobal(L, "class");
    
    return 0;
}

int main(int argc, char* argv[])
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    regClassFunc(L);
    
    //luaL_loadfile(L, "../examples/LuaTest2.lua");
    //lua_resume(L, NULL, 0);
    luaL_dofile(L, "../examples/LuaTest2.lua");
    
    lua_getglobal(L, "a");
    int a = lua_gettop(L);
    lua_getfield(L, a, "test");
    lua_call(L, 0, 0);

    return 0;
}
