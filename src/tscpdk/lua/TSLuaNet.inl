/* 
 * File:   TSLuaNet.inl
 * Author: thunderliu
 *
 * Created on 2013年11月29日, 下午2:29
 */

#ifndef __TSLUANET_INL__
#define	__TSLUANET_INL__

#include "TSLuaNet.h"


#if !defined(TSCPDK_MSVC)

NS_TSCPDK_BEGIN

inline CLuaNetSe::CLuaNetSe(bool bNewHandle)
: CLuaSe(bNewHandle)
{
}

inline CLuaNetSe::CLuaNetSe(lua_State* pL)
: CLuaSe(pL)
{
}

inline CLuaNetSe::~CLuaNetSe()
{
}
    
inline CNetEventDriver* CLuaNetSe::GetNetEventDriver(int iStackPos)
{
    PushTableField("_netEventDriver", iStackPos);
    CNetEventDriver* pNet = (CNetEventDriver*)ToUserData(CONST_STACK_TOP);
    Pop();
    return pNet;
}

inline CIpSocket* CLuaNetSe::GetServerSocket(int iStackPos)
{
    PushTableField("_sock", iStackPos);
    CIpSocket* pSock = (CIpSocket*)ToUserData(CONST_STACK_TOP);
    Pop();
    return pSock;
}

inline CLuaNetSe::SVR_SOCK_TYPE CLuaNetSe::GetServerSocketType(int iStackPos)
{
    PushTableField("sockType", iStackPos);
    SVR_SOCK_TYPE eType = (SVR_SOCK_TYPE)ToInteger(CONST_STACK_TOP);
    Pop();
    return eType;
}

inline int CLuaNetSe::NumberOfServices(int iStackPos)
{
    PushTableField("numberOfServices", iStackPos);
    int iCount = ToInteger(CONST_STACK_TOP);
    Pop();
    return iCount;
}

// CLuaNetSvrThread

inline CLuaNetSvrThread::CLuaNetSvrThread()
: m_oL(false)
, m_oMainL(false)
{
}

inline void CLuaNetSvrThread::Init(lua_State* pL, lua_State* pMainL)
{
    m_oL.Attach(pL);
    m_oMainL.Attach(pMainL);
}


NS_TSCPDK_END

#endif // TSCPDK_MSVC

#endif	/* __TSLUANET_INL__ */

