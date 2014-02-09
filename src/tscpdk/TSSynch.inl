/* 
 * File:   TSSynch.inl
 * Author: thunderliu
 *
 * Created on 2011年12月13日, 下午6:03
 */

#ifndef __TSSYNCH_INL__
#define	__TSSYNCH_INL__


NS_TSCPDK_BEGIN

// CLock

inline CLock::~CLock()
{
}

// CMutex

inline CMutex::CMutex()
{
    pthread_mutex_init(&m_stMux, NULL);
}

inline CMutex::~CMutex()
{
    pthread_mutex_destroy(&m_stMux);
}

inline bool CMutex::Lock()
{
    return pthread_mutex_lock(&m_stMux) == 0;
}

inline bool CMutex::Unlock()
{
    return pthread_mutex_unlock(&m_stMux) == 0;
}

inline bool CMutex::TryLock()
{
    return pthread_mutex_trylock(&m_stMux) == 0;
}


// CGuard

inline CGuard::CGuard(CLock* pLock)
: m_pLock(pLock)
{
    m_pLock->Lock();
}

inline CGuard::~CGuard()
{
    m_pLock->Unlock();
}

NS_TSCPDK_END

#endif	/* __TSSYNCH_INL__ */

