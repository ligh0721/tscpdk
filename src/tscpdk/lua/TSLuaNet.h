/* 
 * File:   TSLuaNet.h
 * Author: thunderliu
 *
 * Created on 2013年11月29日, 下午2:29
 */

#ifndef __TSLUANET_H__
#define	__TSLUANET_H__


#if !defined(TSCPDK_MSVC)

NS_TSCPDK_BEGIN


DEF_LUACFUNC(tsnet_createServer);

DEF_LUACFUNC(tsnet_svr_createUdpService);
DEF_LUACFUNC(tsnet_svr_createTcpService);
DEF_LUACFUNC(tsnet_svr_runServices);

DEF_LUACFUNC(tsnet_svc_udpAccess);
DEF_LUACFUNC(tsnet_svc_tcpAccess);

DEF_LUACFUNC(tsnet_udpsvc_recvfrom);
DEF_LUACFUNC(tsnet_udpsvc_sendto);

DEF_LUACFUNC(tsnet_tcpsvc_recv);
DEF_LUACFUNC(tsnet_tcpsvc_send);

DEF_LUACFUNC(tsnet_svr_thread);



// CLuaNetSe

class CLuaNetSe : public CLuaSe
{
public:
    enum SVR_SOCK_TYPE
    {
        kUdp,
        kTcp
    };
    
    static const luaL_Reg CONST_TSNET_FUNCS[];
    static const luaL_Reg CONST_TSNET_SVR_FUNCS[];
    static const luaL_Reg CONST_TSNET_SVC_FUNCS[];
    static const luaL_Reg CONST_TSNET_UDPSVC_FUNCS[];
    static const luaL_Reg CONST_TSNET_TCPSVC_FUNCS[];
    
public:
    CLuaNetSe(bool bNewHandle = true);
    CLuaNetSe(lua_State* pL);
    virtual ~CLuaNetSe();
    
    virtual bool Init(bool bLoadLibs = true);
    
    CNetEventDriver* GetNetEventDriver(int iStackPos = 1);
    CIpSocket* GetServerSocket(int iStackPos = 1);
    SVR_SOCK_TYPE GetServerSocketType(int iStackPos = 1);
    
    int NumberOfServices(int iStackPos = 1);
    
protected:
    
};

class CLuaNetSvrThread : public CThread
{
public:
    CLuaNetSvrThread();
    
    void Init(lua_State* pL, lua_State* pMainL);
    
protected:
    virtual long ThreadProc();
    
protected:
    CLuaNetSe m_oL;
    CLuaNetSe m_oMainL;
};



NS_TSCPDK_END

#endif // TSCPDK_MSVC

#include "TSLuaNet.inl"

#endif	/* __TSLUANET_H__ */

