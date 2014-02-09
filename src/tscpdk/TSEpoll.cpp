/* 
 * File:   TSEpoll.cpp
 * Author: thunderliu
 * 
 * Created on 2011年12月20日, 下午6:22
 */

#define LIBTSCPDK_SRC

#include "TSPlatform.h"
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

#include "TSTimer.h"
#include "TSEpoll.h"


NS_TSCPDK_BEGIN

#if !defined(TSCPDK_MSVC)


// CEpollFramework

bool CNetEventDriver::Init(uint32_t uMaxRegisterIo)
{
    bool bRet = m_oEp.CreateEpoll();

    if (!bRet)
    {
        return false;
    }

    //bRet = m_oTm.Init();

    m_oIoContextsMap.Init(2000, 10, 0);

    return true;
}

bool CNetEventDriver::RegisterIo(CSocket* pIo, uint32_t dwEvents)
{
    //ASSERT(pIo);
    bool bNew;
    CIoContextHash::CHashNode* pNode = m_oIoContextsMap.FindNodeToSet(pIo->GetHandle(), &bNew);

    if (!pNode || IS_INVALID_NODE(pNode))
    {
        return false;
    }

    bool bRet;
    if (!bNew && pNode->tMapped.IsInUse())
    {
        bRet = m_oEp.Control(CEpoll::CTL_MODIFY, pIo->GetHandle(), dwEvents);
    }
    else
    {
        //pIo->SetFrameworkHandle(this);
        bRet = m_oEp.Control(CEpoll::CTL_ADD, pIo->GetHandle(), dwEvents);
    }

    if (!bRet)
    {
        return false;
    }

    time_t uTime = time(NULL);
    pNode->tMapped.SetIo(pIo);
    pNode->tMapped.SetCreateTime(uTime);
    pNode->tMapped.SetLastAccessTime(uTime);

    return true;
}

bool CNetEventDriver::UnregisterIo(CSocket* pIo)
{
    //ASSERT(pIo);
    CIoContextHash::CHashNode* pNode = m_oIoContextsMap.FindNode(pIo->GetHandle());

    if (!pNode || IS_INVALID_NODE(pNode))
    {
        return false;
    }

    if (!pNode->tMapped.IsInUse())
    {
        return false;
    }

    bool bRet = m_oEp.Control(CEpoll::CTL_DELETE, pIo->GetHandle(), 0);
    if (!bRet)
    {
        return false;
    }

    pNode->tMapped.Clear();
    m_oIoContextsMap.ClearNode(*pNode); // 实际无需释放Node，只需CIoContext::Clear()

    return true;
}

bool CNetEventDriver::ModifyIo(CSocket* pIo, uint32_t dwEvents)
{
    //ASSERT(pIo);
    CIoContextHash::CHashNode* pNode = m_oIoContextsMap.FindNode(pIo->GetHandle());

    if (!pNode || IS_INVALID_NODE(pNode))
    {
        return false;
    }

    if (!pNode->tMapped.IsInUse())
    {
        return false;
    }

    bool bRet = m_oEp.Control(CEpoll::CTL_MODIFY, pIo->GetHandle(), dwEvents);
    if (!bRet)
    {
        return false;
    }

    time_t uTime = time(NULL);
    pNode->tMapped.SetLastAccessTime(uTime);

    return true;
}

bool CNetEventDriver::NetEventLoop()
{
    if (m_oEp.GetHandle() < 0)
    {
        return false;
    }
    
    long lTimeout = INFINITE;

    for (;;)
    {
        m_oTm.Update();
        
        if (!m_oTm.PeekTimeout(&lTimeout))
        {
            //LOG_ERR("DGB | No Timer!");
        }
        else
        {
            //LOG_ERR("DGB | Timer Head(%ld)", lTimeout);
        }

        if (!GetEvent((int)lTimeout))
        {
            // err
            return false;
        }

        if (m_iEpResult < 0)
        {
            // SIGNAL

            continue;
        }

        // dispatch event
        DispatchEvent();
    }

    return true;
}

bool CNetEventDriver::GetEvent(int iTimeout)
{
    //printf("epoll_wait: %d MSec\n", iTimeout);
    m_iEpResult = m_oEp.Wait(&m_oEvent, 1, iTimeout);

    printf("[%d|%lu] Epoll Wait Ret %d %d\n", getpid(), pthread_self(), m_iEpResult, errno);
    if (m_iEpResult < 0)
    {
        // SIGNALED

        //break;
        // or
        //continue;
        return true;
    }

    return true;
}

bool CNetEventDriver::DispatchEvent()
{
    bool bRes;

    if (!m_iEpResult)
    {
        // timeout
        return true;
    }
    
    //LOG_ERR("DBG | epoll event");

    CIoContextHash::CHashNode* pNode = m_oIoContextsMap.FindNode(m_oEvent.GetHandle());

    if (!pNode || IS_INVALID_NODE(pNode))
    {
        return false;
    }

    if (m_oEvent.GetEvents() & CEpollEvent::EV_IN)
    {
        //LOG_ERR("DBG | Framework Event: Read, fd(%d)", pNode->tMapped.GetIo()->GetHandle());
        //LOG_POS("MSG | OnRead Begin %d", getpid());
        //CStopWatch oSw;
        //oSw.Start();
        //LOG_ERR("DBG | epoll read event");
        bRes = pNode->tMapped.GetIo()->OnRead();
        //LOG_POS("MSG | OnRead End %d", getpid());
        //LOG_POS("MSG | OnRead cost %u usecs", oSw.Stop());
        if (!bRes)
        {
            pNode->tMapped.GetIo()->Close();
        }
    }
    else if (m_oEvent.GetEvents() & CEpollEvent::EV_OUT)
    {
        //LOG_ERR("DBG | Framework Event: Write");
        bRes = pNode->tMapped.GetIo()->OnWrite();
        if (!bRes)
        {
            pNode->tMapped.GetIo()->Close();
        }
    }
    else if (m_oEvent.GetEvents() & CEpollEvent::EV_ERR)
    {
        //LOG_ERR("DBG | Framework Event: Error");
        bRes = pNode->tMapped.GetIo()->OnError();
        if (!bRes)
        {
            //pIoContext->GetIo()->Close();
        }
        pNode->tMapped.GetIo()->Close();
    }
    else
    {
    }

    return true;
}

CNetEventDriver* CNetEventDriver::Instance()
{
    static CNetEventDriver* pInst = NULL;
    unlikely (!pInst)
    {
        pInst = new CNetEventDriver;
    }
    
    return pInst;
}

#endif

NS_TSCPDK_END

