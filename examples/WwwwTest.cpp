#include <tscpdk/TSCPDK.h>
#include "WwwwTest.hpp"


USING_NS_TSCPDK;

using namespace std;

#pragma pack(push, 1)

struct RCVHEAD
{
    uint16_t wLen;
    uint32_t dwSeq;
};

struct RCVBODY
{
    uint16_t wNameLen;
    char acName[0];
};

struct RCVBODY02
{
    uint16_t wPswLen;
    char acPsw[0];
};

#pragma pack(pop)

#if 0

int test01(int argc, char* argv[])
{
    CUdpSocket s;
    s.Bind(NULL, 22222);
    char acBuf[1024];
    CSockAddrIn oSa;
    //  for (;;)
    //  {
    int len = s.RecvFrom(acBuf, sizeof (acBuf), &oSa);
    acBuf[len] = 0;
    RCVHEAD stHead;
    RCVBODY stBody;
    stHead.wLen = (acBuf[1] << 8) + acBuf[2];
    stHead.dwSeq = (acBuf[3] << 24) + (acBuf[4] << 16) + (acBuf[5] << 8) + acBuf[6];
    stBody.wNameLen = (acBuf[7] << 8) + acBuf[8];
    stBody.wPswLen = (acBuf[stBody.wNameLen + 9] << 8) + acBuf[stBody.wNameLen + 10];
    stBody.acName = (char*)malloc(stBody.wNameLen);
    stBody.acPsw = (char*)malloc(stBody.wPswLen);
    for (int i = 0; i < stBody.wNameLen; i++)
    {
        stBody.acName[i] = acBuf[9 + i];
    }
    for (int i = 0; i < stBody.wPswLen; i++)
    {
        stBody.acPsw[i] = acBuf[11 + stBody.wNameLen + i];
    }

    s.SendTo("Goodbye", 8, &oSa);
}
#endif

int test02(int argc, char* argv[])
{
    CUdpSocket s;
    s.Bind(NULL, 22222);
    char acBuf[1024];
    CSockAddrIn oSa;

    map<string, string> mapStorage;
    for (;;)
    {
        int len = s.RecvFrom(acBuf, sizeof (acBuf), &oSa);

#if 0
        uint16_t wPos = 0;

        char* pStx = (char*)(acBuf + wPos);
        wPos += sizeof (char);

        RCVHEAD* pHdr = (RCVHEAD*)(acBuf + wPos);
        wPos += sizeof (RCVHEAD);

        RCVBODY* pBody = (RCVBODY*)(acBuf + wPos);
        wPos += sizeof (RCVBODY);

        uint16_t wNameLen = ntohs(pBody->wNameLen);
        wPos += wNameLen;

        RCVBODY02* pBody2 = (RCVBODY02*)(acBuf + wPos);
        wPos += sizeof (RCVBODY02);

        uint16_t wPswLen = ntohs(pBody2->wPswLen);
        wPos += wPswLen;

        char* pEtx = (char*)(acBuf + wPos);
        wPos += sizeof (char);

        if (*pStx == 0x02 && *pEtx == 0x03 && wPos == ntohs(pHdr->wLen) && wPos == len)
        {
            printf("%u %u\n", wNameLen, wPswLen);
            if (mapStorage.find(string(pBody->acName, wNameLen)) == mapStorage.end())
            {
                mapStorage.insert(make_pair(string(pBody->acName, wNameLen), string(pBody2->acPsw, wPswLen)));
            }
        }
#endif
        WwwwTest::CType_stPack pack;
        pack.ParseFromArray(acBuf, len);
        
        pack.stHdr().wCmd() = 0x12;
        
        pack.cStx0x02() = 0x02;
        pack.cEtx0x03() = 0x03;
        len = pack.SerializeToArray(acBuf, sizeof(acBuf));
        
        s.SendTo(acBuf, len, &oSa);
        
    }
    return 0;
}

int aaa = 0;
int bbb = 0;
void AddChild()
{
    while (random() % 2) ++aaa;
    ++bbb;
}

int main(int argc, char* argv[])
{
    //return test02(argc, argv);

    srandom(time(NULL));
    for (int i = 0; i < 1000000; ++i)
    {
        AddChild();
    }
    printf("boy: %d\ngirl: %d\n", bbb, aaa);
    WwwwTest::CType_stBody o;
    WwwwTest::CType_stHdr b;
    o.Assign_acName((const uint8_t*)"test", 11);
    b.Assign_acReserved((const uint8_t*)"test");

    return 0;
}

