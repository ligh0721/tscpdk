/* 
 * File:   TSEpoll.h
 * Author: thunderliu
 *
 * Created on 2011年12月20日, 下午6:22
 */

#ifndef __TSEPOLL_H__
#define	__TSEPOLL_H__

#include "TSHash.h"
#include "TSSocket.h"
#include "TSTimer.h"


#if !defined(TSCPDK_MSVC)

NS_TSCPDK_BEGIN


class TSCPDK_API CIoContext
{
public:
    bool IsInUse() const;
    void Clear();

    CSocket* GetIo();
    void SetIo(CSocket* pIo);

    time_t GetCreateTime() const;
    void SetCreateTime(time_t uCreateTime);

    time_t GetLastAccessTime() const;
    void SetLastAccessTime(time_t uCreateTime);

protected:
    CSocket* m_pIo;
    time_t m_uCreateTime;
    time_t m_uLastAccessTime;
};

class TSCPDK_API CEpollEvent : public epoll_event
{
public:

    typedef enum
    {
        EV_IN = EPOLLIN,
        EV_PRI = EPOLLPRI,
        EV_OUT = EPOLLOUT,
        //        EV_RDNORM = EPOLLRDNORM,
        //        EV_RDBAND = EPOLLRDBAND,
        //        EV_WRNORM = EPOLLWRNORM,
        //        EV_WRBAND = EPOLLWRBAND,
        EV_MSG = EPOLLMSG,
        EV_ERR = EPOLLERR,
        ///        EV_HUP = EPOLLHUP,
        //EV_RDHUP = EPOLLRDHUP,
        //        EV_ONESHOT = EPOLLONESHOT,
        //        EV_ET = EPOLLET
    } EVENTS;

public:
    uint32_t GetEvents() const;
    void SetEvents(uint32_t dwEvents);

    int GetHandle() const;
    void SetHandle(int iFd);

};

class TSCPDK_API CEpoll
{
public:

    typedef enum
    {
        CTL_ADD = EPOLL_CTL_ADD,
        CTL_DELETE = EPOLL_CTL_DEL,
        CTL_MODIFY = EPOLL_CTL_MOD
    } CONTROL;

public:
    CEpoll(int iFd = -1);
    virtual ~CEpoll();

    bool CreateEpoll();
    int Wait(CEpollEvent* pEvents, int iMaxEventCount, int iTimeout);
    bool Control(CONTROL eControl, int iFd, uint32_t dwEvents);
    
    int GetHandle() const;
    
protected:
    int m_iFd;
};

class TSCPDK_API CNetEventDriver
{
protected:
    typedef CSimple32Hash<CIoContext> CIoContextHash;

public:
    CNetEventDriver();
    virtual ~CNetEventDriver();

public:
    bool Init(uint32_t uMaxRegisterIo = 10000);
    bool RegisterTimer(CTimerNodeBase* pTimer, const CTime& rInterval, const CTime& rDelay);
    bool UnregisterTimer(CTimerNodeBase* pTimer);
    bool RegisterIo(CSocket* pIo, uint32_t dwEvents);
    bool UnregisterIo(CSocket* pIo);
    bool ModifyIo(CSocket* pIo, uint32_t dwEvents);
    bool NetEventLoop();

protected:
    bool GetEvent(int iTimeout);
    bool DispatchEvent();
    
    static CNetEventDriver* Instance();

protected:
    CEpoll m_oEp;
    CTimerQueue m_oTm;

    CIoContextHash m_oIoContextsMap;

    //static const size_t EPOLL_WAIT_EVENT_COUNT = 1;
    //CEpollEvent m_aoEvents[EPOLL_WAIT_EVENT_COUNT];
    CEpollEvent m_oEvent;
    int m_iEpResult;

};

NS_TSCPDK_END
        
#endif

#include "TSEpoll.inl"

#endif	/* __TSEPOLL_H__ */

