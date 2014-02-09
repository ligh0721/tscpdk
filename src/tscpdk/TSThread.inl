/* 
 * File:   TSThread.inl
 * Author: thunderliu
 *
 * Created on 2011年12月25日, 上午1:21
 */

#ifndef __TSTHREAD_INL__
#define	__TSTHREAD_INL__


NS_TSCPDK_BEGIN

// CThread

inline CThread::CThread()
: m_uHdl(pthread_t())
, m_pExitCode(PTHREAD_CANCELED)
{
}

inline CThread::~CThread()
{
}

inline long CThread::ThreadProc()
{
    return 0;
}

inline void CThread::OnExit(void* pExitCode)
{
}

inline CThread::E_CANCEL_TYPE CThread::SetCancelType(E_CANCEL_TYPE eType)
{
    int iOld = 0;
    pthread_setcanceltype((int)eType, &iOld);
    return (E_CANCEL_TYPE)iOld;
}

// CThreadGroup

template <typename THREAD_TYPE>
inline CThreadGroup<THREAD_TYPE>::CThreadGroup()
: m_iCount(0)
, m_pThreads(NULL)
{
}

template <typename THREAD_TYPE>
inline CThreadGroup<THREAD_TYPE>::~CThreadGroup()
{
    if (m_pThreads)
    {
        delete[] m_pThreads;
    }
}

template <typename THREAD_TYPE>
void CThreadGroup<THREAD_TYPE>::CreateThreads(int iChildCount)
{
    m_iCount = iChildCount;
    m_pThreads = new THREAD_TYPE[m_iCount];
    for (int i = 0; i < m_iCount; ++i)
    {
        m_pThreads[i].Start();
    }
}

template <typename THREAD_TYPE>
bool CThreadGroup<THREAD_TYPE>::Wait()
{
    for (int i = 0; i < m_iCount; ++i)
    {
        m_pThreads[i].Wait();
    }
    return true;
}

template <typename THREAD_TYPE>
bool CThreadGroup<THREAD_TYPE>::Release()
{
    delete this;
    return true;
}


NS_TSCPDK_END

#endif	/* __TSTHREAD_INL__ */

