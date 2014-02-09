#include <tscpdk/TSCPDK.h>

#include "ProtoTest.hpp"
#include "TSDebug.h"

USING_NS_TSCPDK;



int main(int argc, char* argv[])
{
#if 1
    CSimpleProtoTypeCompiler::Compile(argv[1], argc == 3 ? argv[2] : NULL);
    return 0;
#elif 0
    CSimpleProtoTypeCompiler::Compile("../examples/WwwwTest.Register.pt", "../examples/WwwwTest.hpp");
    return 0;
#endif

    int iCount = argc == 2 ? atoi(argv[1]) : 1;
    srand(time(NULL));
    
    size_t uSize = 0;
    
    char szBuf[1024];
    CStopWatch oSw;
    oSw.Start();
    for (int i = 0; i < iCount; ++i)
    {
        tsproto::CType_stPack stPack;
        stPack.stHdr().dwSequence() = rand();
        stPack.stHdr().acRecvered(rand() % 8) = rand() % 256;
        tsproto::CType_stField& rField = stPack.stBody().Add_stField();
        rField.Add_cBuf() = 9;
        rField.Add_cBuf() = 8;
        rField.Add_cBuf() = 7;
        rField.Add_cBuf() = 6;
        rField.Add_cBuf() = 5;
        rField.Add_cBuf() = 4;
        rField.Add_cBuf() = 3;
        //rField.Add_cBuf() = 2;

        stPack.cStx0x0A() = 0x0A;
        stPack.cEtx0x03() = 0x03;

        uSize = stPack.SerializeToArray(szBuf, sizeof(szBuf));

        tsproto::CType_stPack stTest;
        stTest.ParseFromArray(szBuf, uSize);
    
    }
    
    CLog::SetOutFile(stdout);
    CDebug::SimpleDump(szBuf, uSize, CDebug::kShowChar | CDebug::kShowOffset);
    printf("%ld us\n", oSw.Start() / iCount);
    //printf("%d, %d\n", (int)stTest.cStx0x0A(), (int)stTest.cEtx0x03());
    printf("Done.\n");
    return 0;
}

