/* 
 * File:   TSThread.h
 * Author: thunderliu
 *
 * Created on 2011年12月25日, 上午1:20
 */

#ifndef __TSTHREAD_H__
#define	__TSTHREAD_H__

EXTERN_C_BEGIN
#include <pthread.h>
EXTERN_C_END

#include "TSSynch.h"


NS_TSCPDK_BEGIN

class TSCPDK_API CThread
{
public:

    typedef enum
    {
        CANCEL_DEFFERED = PTHREAD_CANCEL_DEFERRED,
        CANCEL_ASYNCHRONOUS = PTHREAD_CANCEL_ASYNCHRONOUS
    } E_CANCEL_TYPE;
public:
    CThread();
    virtual ~CThread();

    virtual bool Start();
    virtual bool Wait();
    bool Cancel();

    //static CThread* CreateThread();
    static E_CANCEL_TYPE SetCancelType(E_CANCEL_TYPE eType);

protected:
    virtual long ThreadProc();
    virtual void OnExit(void* pExitCode);

private:
    static void* ThreadRoutine(CThread* pInstance);
    static void CleanUpRoutine(CThread* pInstance);

protected:
    pthread_t m_uHdl;
    void* m_pExitCode;

};

class CCriticalSection
{
public:
protected:
    CMutex m_oMutex;
};

template <typename THREAD_TYPE>
class CThreadGroup
{
public:
    CThreadGroup();
    virtual ~CThreadGroup();
    
public:
    void CreateThreads(int iChildCount);
    bool Wait();
    bool Release();
    
protected:
    int m_iCount;
    THREAD_TYPE* m_pThreads;
};


NS_TSCPDK_END

#include "TSThread.inl"

#endif	/* __TSTHREAD_H__ */

