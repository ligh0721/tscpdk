#include <tscpdk/TSCPDK.h>


USING_NS_TSCPDK;

class CTestWorker : public CWorker
{
public:
    virtual bool OnWork(CBlock16* pBlock)
    {
        printf("on work %s\n", (char*)pBlock->GetBuffer());
        return true;
    }
    
    virtual bool OnEmpty()
    {
        printf("on empty\n");
        return true;
    }
};

int main(int argc, char* argv[])
{
    CTestWorker oTw;
    oTw.Init(10000);
    oTw.Start();
    
    for (int i = 0; i < 1000; ++i)
    {
        CBlock16* pBlock = CBlock16::CreateBlock(10);
        sprintf((char*)pBlock->GetBuffer(), "000");
        *((char*)pBlock->GetBuffer() + 1) = 0x30 + i % 10;
        oTw.AddBlock(pBlock);
        
        if (i % 211 == 0)
        {
            ::SleepForSec(1);
        }
    }
    
    oTw.Wait();

    return 0;
}

