/* 
 * File:   TSSynch.inl
 * Author: thunderliu
 *
 * Created on 2011年12月13日, 下午6:03
 */

#ifndef __TSSYNCH_INL__
#define	__TSSYNCH_INL__


NS_TSCPDK_BEGIN


// CLockBase
inline CLockBase::~CLockBase()
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

// CRLock
inline CRLock::CRLock(pthread_rwlock_t& rwLock)
: m_rLock(rwLock)
{
}

inline bool CRLock::Lock()
{
    return pthread_rwlock_rdlock(&m_rLock) == 0;
}

inline bool CRLock::Unlock()
{
    return pthread_rwlock_unlock(&m_rLock) == 0;
}

// CWLock
inline CWLock::CWLock(pthread_rwlock_t& rwLock)
: m_rLock(rwLock)
{
}

inline bool CWLock::Lock()
{
    return pthread_rwlock_wrlock(&m_rLock) == 0;
}

inline bool CWLock::Unlock()
{
    return pthread_rwlock_unlock(&m_rLock) == 0;
}

// CRWLock
inline CRWLock::CRWLock()
: m_oRLock(m_stLock)
, m_oWLock(m_stLock)
{
    pthread_rwlock_init(&m_stLock, NULL);
}

inline CRWLock::~CRWLock()
{
    pthread_rwlock_destroy(&m_stLock);
}

inline CRLock* CRWLock::RLock()
{
    return &m_oRLock;
}

inline CWLock* CRWLock::WLock()
{
    return &m_oWLock;
}

// CGuard
inline CGuard::CGuard(CLockBase* pLock)
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

