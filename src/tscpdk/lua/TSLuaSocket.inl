/* 
 * File:   TSLuaSocket.inl
 * Author: thunderliu
 *
 * Created on 2013年11月29日, 下午2:27
 */

#ifndef __TSLUASOCKET_INL__
#define	__TSLUASOCKET_INL__

#include "TSLuaSocket.h"


#if !defined(TSCPDK_MSVC)


NS_TSCPDK_BEGIN

// CLuaSocketProtocol

inline CLuaSocketProtocol::CLuaSocketProtocol(lua_State* pL)
: m_oL(pL)
{
}

inline void CLuaSocketProtocol::SetLuaSe(lua_State* pL)
{
    m_oL.Attach(pL);
}

// CLuaServerSocketProtocol

inline CLuaServerSocketProtocol::CLuaServerSocketProtocol(lua_State* pL)
: CLuaSocketProtocol(pL)
, m_iOnReadFunc(0)
{
}

inline CLuaServerSocketProtocol::~CLuaServerSocketProtocol()
{
}

inline void CLuaServerSocketProtocol::SetOnReadFuncion()
{
    // 函数必须在栈顶
    m_iOnReadFunc = m_oL.GetRegistry().CreateRef();
    m_oL.GetRegistry().RawGet(m_iOnReadFunc);
}

inline void CLuaServerSocketProtocol::UnsetOnReadFunction()
{
    m_oL.GetRegistry().ReleaseRef(m_iOnReadFunc);
    m_iOnReadFunc = 0;
}

// CLuaServerUdpSocket

inline CLuaServerUdpSocket::CLuaServerUdpSocket(lua_State* pL, int iSock)
: CUdpSocket(iSock)
, CLuaServerSocketProtocol(pL)
{
}

inline bool CLuaServerUdpSocket::CLuaServerUdpSocket::OnRead()
{
    CLuaNetSe oCo(m_oL.PushNewThread());
    m_oL.Pop();
    
    oCo.GetRegistry().RawGet(m_iOnReadFunc);
    oCo.ResumeL(m_oL, 0); // 激活请求处理协程

    return true;
}

// CLuaClientSocket

inline CLuaClientSocketProtocol::CLuaClientSocketProtocol(lua_State* pL, lua_State* pMainL)
: CLuaSocketProtocol(pL)
, m_oMainL(pMainL)
{
}

inline CLuaClientSocketProtocol::~CLuaClientSocketProtocol()
{
}
 
// CLuaClientUdpSocket

inline CLuaClientUdpSocket::CLuaClientUdpSocket(CNetEventDriver* pNet, lua_State* pL, lua_State* pMainL, int iSock)
: CUdpSocket(iSock)
, CLuaClientSocketProtocol(pL, pMainL)
, m_pNet(pNet)
, m_bUseTimer(false)
{
}

inline bool CLuaClientUdpSocket::OnRead()
{
    char szBuf[0x10000];
    CSockAddrIn oSa;
    int iRes = RecvFrom(szBuf, sizeof (szBuf), &oSa);

    m_oL.PushNil();
    if (iRes > 0)
    {
        m_oL.PushString(szBuf, iRes);
    }
    else
    {
        m_oL.PushNil();
    }

    return false;
}

inline bool CLuaClientUdpSocket::OnClose()
{
    m_pNet->UnregisterIo(dynamic_cast<CSocket*>(this));

    return true;
}

inline bool CLuaClientUdpSocket::OnClosed()
{
    m_oL.ResumeL(m_oMainL, 2);

    if (m_bUseTimer)
    {
        m_pNet->UnregisterTimer(dynamic_cast<CTimerQueueNode*>(this));
    }
    else
    {
        delete this;
    }

    return true;
}

inline bool CLuaClientUdpSocket::OnTimeout()
{
    m_oL.PushString("recvfrom timeout");
    m_oL.PushNil();

    Close();

    return true;
}

inline void CLuaClientUdpSocket::SetUseTimer(bool bUse)
{
    m_bUseTimer = bUse;
}

NS_TSCPDK_END

#endif // TSCPDK_MSVC

#endif	/* __TSLUASOCKET_INL__ */

