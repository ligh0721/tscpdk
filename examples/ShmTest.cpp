#include <tscpdk/TSCPDK.h>

USING_NS_TSCPDK;

int main(int argc, char* argv[])
{
    CShmStream oShm;
    //oShm.Init("TestShm", 1024);
    oShm.Attach("TestShm", 1024);
    char szTest[1024];
    oShm.Read(szTest, 1, 7);
    
    return 0;
}

