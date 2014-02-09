#include <tscpdk/TSCPDK.h>

USING_NS_TSCPDK;


int main(int argc, char* argv[])
{
    CLuaNetSe oL;
    oL.Init();

#if 1
    CStr oErr(128);
    unlikely(!oL.RunFile("./examples/LuaTest.lua", &oErr))
    {
        printf("ERR | %s\n", oErr.GetBuffer());
    }
#else
    luaL_loadfile(oL, "./examples/LuaTest.lua");
    oL.ResumeL(NULL, 0);
#endif

    return 0;
}
