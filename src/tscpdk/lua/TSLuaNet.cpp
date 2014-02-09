/* 
 * File:   TSLuaNet.cpp
 * Author: thunderliu
 * 
 * Created on 2013年11月29日, 下午2:29
 */

#define LIBTSCPDK_SRC

#include "TSCPDK.h"
#include "TSLuaNet.h"

#if !defined(TSCPDK_MSVC)

NS_TSCPDK_BEGIN


DEF_LUACFUNC(tsnet_createServer) // err, svc = createServer())
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    oL.PushNil();
    
    CLuaTable oSvr = oL.PushTable();
    oSvr.SetFunctions(CLuaNetSe::CONST_TSNET_SVR_FUNCS);
    
    oL.PushInteger(0);
    oSvr.SetField("numberOfServices");
    
    return 2;
}

DEF_LUACFUNC(tsnet_svr_createUdpService)  // err, svc = svr:createUdpService(ip, port)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    CLuaServerUdpSocket* pSock = new CLuaServerUdpSocket(NULL);
    pSock->Bind(oL.ToString(2), oL.ToInteger(3));
    
    CLuaTable oSvr(oL, 1);
    
    int iCount = oL.NumberOfServices(1) + 1;
    oL.PushInteger(iCount);
    oSvr.SetField("numberOfServices");
    
    CLuaTable oSvrServices = oL.PushTable();
    {
        CLuaTable oSvc = oL.PushTable(); 
        {
            oL.PushUserData(pSock);
            oSvc.SetField("_sock");
            oSvc.SetFunctions(CLuaNetSe::CONST_TSNET_SVC_FUNCS);
            oSvc.SetFunctions(CLuaNetSe::CONST_TSNET_UDPSVC_FUNCS);
        }
        oSvrServices.RawSet(iCount);
    }
    oSvr.SetField("services");
    
    oL.PushNil();
    
    oSvr.GetField("services");
    CLuaTable oSvc(oL, oL.GetTopPos());
    oSvc.RawGet(iCount);
    
    return 2;
}

DEF_LUACFUNC(tsnet_svr_createTcpService)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    CLuaServerUdpSocket* pSock = new CLuaServerUdpSocket(oL);
    pSock->Bind(oL.ToString(2), oL.ToInteger(3));
    pSock->SetNonBlock();
    
    CLuaTable oSvr(oL, 1);
    
    int iCount = oL.NumberOfServices(1) + 1;
    oL.PushInteger(iCount);
    oSvr.SetField("numberOfServices");
    
    CLuaTable oSvrServices = oL.PushTable();
    {
        CLuaTable oSvc = oL.PushTable(); 
        {
            oL.PushUserData(pSock);
            oSvc.SetField("_sock");
            oSvc.SetFunctions(CLuaNetSe::CONST_TSNET_SVC_FUNCS);
            oSvc.SetFunctions(CLuaNetSe::CONST_TSNET_TCPSVC_FUNCS);
        }
        oSvrServices.RawSet(iCount);
    }
    oSvr.SetField("services");
    
    oL.PushNil();
    
    oSvr.GetField("services");
    CLuaTable oSvc(oL, oL.GetTopPos());
    oSvc.RawGet(iCount);
    
    return 2;
}

DEF_LUACFUNC(tsnet_svr_runServices)  // err = svr:runServices(instCount)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    int iCount = oL.ToInteger(2);
    CLuaNetSvrThread* aoThreads = new CLuaNetSvrThread[iCount];
    for (int i = 0; i < iCount; ++i)
    {
        CLuaNetSe oThrd(oL.PushNewThread());
        oL.Pop();
        
        oThrd.PushCFunction(tsnet_svr_thread);
        oL.PushValue(1);
        oL.XMove(oL, oThrd, 1);
        aoThreads[i].Init(oThrd, oL);
    }
    
    /*
    CLuaTable oSvr(oL, 1);
    oL.PushInteger(iCount);
    oSvr.SetField("threadCount")
    oL.PushUserData(aoThreads);
    oSvr.SetField("_threads")
    */
    
    for (int i = 0; i < iCount; ++i)
    {
        aoThreads[i].Start();
    }
    
    for (int i = 0; i < iCount; ++i)
    {
        aoThreads[i].Wait();
    }
    
    delete[] aoThreads;
    
    oL.PushNil();
    
    return 1;
}

DEF_LUACFUNC(tsnet_svc_udpAccess)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    size_t uLen = 0;
    const char* pReq = oL.ToString(2, &uLen);
    
    CSockAddrIn oSa(oL.ToString(3), oL.ToInteger(4));
    
    oL.PushTableField("parentThread", 1);
    CLuaNetSe oMain(oL.ToThread(-1));
    oL.Pop();
    
    CNetEventDriver* pNet = oL.GetNetEventDriver();
    
    CLuaClientUdpSocket* pCltUdp = new CLuaClientUdpSocket(pNet, oL, oMain);
    pCltUdp->Bind(NULL, 0);
    
    pNet->RegisterIo(pCltUdp, CEpollEvent::EV_IN);
    if (oL.GetTopPos() == 5)
    {
        CTime oTm(0, oL.ToInteger(5));
        pNet->RegisterTimer(dynamic_cast<CTimerNodeBase*>(pCltUdp), oTm, oTm);
        pCltUdp->SetUseTimer(true);
    }
    
    pCltUdp->SendTo(pReq, uLen, &oSa);

    return oL.YieldL(0, 0, NULL);
}

DEF_LUACFUNC(tsnet_svc_tcpAccess)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
}

DEF_LUACFUNC(tsnet_udpsvc_recvfrom)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    char szBuf[0x10000];
    
    CIpSocket* pSock = oL.GetServerSocket();

    CSockAddrIn oSa;
    int iSize = pSock->RecvFrom(szBuf, sizeof(szBuf), &oSa);
    
    oL.PushNil();
    oL.PushString((const char*)szBuf, iSize);
    oL.PushString(oSa.GetAddr());
    oL.PushInteger(oSa.GetPort());
    
    return 4;
}

DEF_LUACFUNC(tsnet_udpsvc_sendto)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    CIpSocket* pSock = oL.GetServerSocket();

    size_t uLen = 0;
    const char* pReq = oL.ToString(2, &uLen);
    
    CSockAddrIn oSa(oL.ToString(3), oL.ToInteger(4));
    pSock->SendTo(pReq, uLen, &oSa);
    
    oL.PushNil();
    
    return 1;
}

DEF_LUACFUNC(tsnet_tcpsvc_recv)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
}

DEF_LUACFUNC(tsnet_tcpsvc_send)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
}

DEF_LUACFUNC(tsnet_svr_thread)
{
    DEF_LUAOBJ(CLuaNetSe, oL);
    
    CLuaTable oSvr(oL, oL.GetTopPos());
    int iCount = oL.NumberOfServices(1);
    
    oSvr.GetField("services");
    {
        CLuaTable oSvrServices(oL, oL.GetTopPos());
    
        CNetEventDriver oNet;
        oNet.Init(1000);

        for (int i = 1; i <= iCount; ++i)
        {
            oSvrServices.RawGet(i);
            {
                CLuaTable oSvc(oL, oL.GetTopPos());
                
                oL.PushThread(oL);
                oSvc.SetField("parentThread");
                
                oL.PushUserData(&oNet);
                oSvc.SetField("_netEventDriver");
                
                CIpSocket* pSock = oL.GetServerSocket(oSvc);
                oNet.RegisterIo(pSock, CEpollEvent::EV_IN);
                
                CLuaServerSocketProtocol* pSockAsProto = dynamic_cast<CLuaServerSocketProtocol*>(pSock);
                pSockAsProto->SetLuaSe(oL);
                
                oSvc.GetField("onRequest");
                pSockAsProto->SetOnReadFuncion();
                
            }
            oL.Pop();
        }

        oNet.NetEventLoop();
        
        for (int i = 1; i <= iCount; ++i)
        {
            oSvrServices.RawGet(i);
            {
                CLuaTable oSvc(oL, oL.GetTopPos());
                CIpSocket* pSock = oL.GetServerSocket(oSvc);
                CLuaServerSocketProtocol* pSockAsProto = dynamic_cast<CLuaServerSocketProtocol*>(pSock);
                pSockAsProto->UnsetOnReadFunction();
                delete pSock;
            }
            oL.Pop();
        }
    }
    oL.Pop();
    
    return 0;
}



// CLuaNetSe

const luaL_Reg CLuaNetSe::CONST_TSNET_FUNCS[] = {
    {"createServer", tsnet_createServer},
    {NULL, NULL}
};

const luaL_Reg CLuaNetSe::CONST_TSNET_SVR_FUNCS[] = {
    {"createUdpService",    tsnet_svr_createUdpService},
    {"createTcpService",    tsnet_svr_createTcpService},
    {"runServices",         tsnet_svr_runServices},
    {NULL, NULL}
};

const luaL_Reg CLuaNetSe::CONST_TSNET_SVC_FUNCS[] = {
    {"udpAccess",   tsnet_svc_udpAccess},
    {"tcpAccess",   tsnet_svc_tcpAccess},
    {NULL, NULL}
};

const luaL_Reg CLuaNetSe::CONST_TSNET_UDPSVC_FUNCS[] = {
    {"recvfrom",    tsnet_udpsvc_recvfrom},
    {"sendto",      tsnet_udpsvc_sendto},
    {NULL, NULL}
};

const luaL_Reg CLuaNetSe::CONST_TSNET_TCPSVC_FUNCS[] = {
    {"recv",        tsnet_tcpsvc_recv},
    {"send",        tsnet_tcpsvc_send},
    {NULL, NULL}
};


bool CLuaNetSe::Init(bool bLoadLibs)
{
    bool bRes = CLuaSe::Init(bLoadLibs);
    unlikely (!bRes)
    {
        return false;
    }
    
    CLuaTable oTSNet = PushTable();
    oTSNet.SetFunctions(CONST_TSNET_FUNCS);
    SetGlobal("tsnet");
    
    return true;
    
}

// CLuaNetSvrThread
long CLuaNetSvrThread::ThreadProc()
{
    m_oL.ResumeL(m_oMainL, 1);

    return 0;
}

NS_TSCPDK_END

#endif // TSCPDK_MSVC
