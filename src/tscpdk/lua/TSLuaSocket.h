/* 
 * File:   TSLuaSocket.h
 * Author: thunderliu
 *
 * Created on 2013年11月29日, 下午2:26
 */

#ifndef __TSLUASOCKET_H__
#define	__TSLUASOCKET_H__

#include "TSLuaNet.h"

#if !defined(TSCPDK_MSVC)

NS_TSCPDK_BEGIN


class CLuaSocketProtocol
{
public:
    CLuaSocketProtocol(lua_State* pL);
    
    void SetLuaSe(lua_State* pL);

protected:
    CLuaNetSe m_oL;
};

class CLuaServerSocketProtocol : public CLuaSocketProtocol
{
public:
    CLuaServerSocketProtocol(lua_State* pL);
    virtual ~CLuaServerSocketProtocol();

    void SetOnReadFuncion();
    void UnsetOnReadFunction();

protected:
    int m_iOnReadFunc;
};

class CLuaServerUdpSocket : public CUdpSocket, public CLuaServerSocketProtocol
{
public:
    CLuaServerUdpSocket(lua_State* pL, int iSock = -1);
    virtual bool OnRead();
    
protected:
    
};

class CLuaClientSocketProtocol : public CLuaSocketProtocol
{
public:
    CLuaClientSocketProtocol(lua_State* pL, lua_State* pMainL);
    virtual ~CLuaClientSocketProtocol();

protected:
    CLuaNetSe m_oMainL;
};

class CLuaClientUdpSocket : public CUdpSocket, public CLuaClientSocketProtocol, public CTimerQueueNode
{
public:
    CLuaClientUdpSocket(CNetEventDriver* pNet, lua_State* pL, lua_State* pMainL, int iSock = -1);

    virtual bool OnRead();
    virtual bool OnClose();
    virtual bool OnClosed();
    virtual bool OnTimeout();
    
    void SetUseTimer(bool bUse);
    
protected:
    CNetEventDriver* m_pNet;
    bool m_bUseTimer;
};

NS_TSCPDK_END

#endif // TSCPDK_MSVC

#include "TSLuaSocket.inl"

#endif	/* __TSLUASOCKET_H__ */

