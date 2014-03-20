/* 
 * File:   TSSynch.h
 * Author: thunderliu
 *
 * Created on 2011年12月13日, 下午6:02
 */

#ifndef __TSSYNCH_H__
#define	__TSSYNCH_H__


EXTERN_C_BEGIN
#include <pthread.h>

#include "TSPlatform.h"
EXTERN_C_END

NS_TSCPDK_BEGIN

class CLockBase
{
public:
    virtual ~CLockBase();

    virtual bool Lock() = 0;
    virtual bool Unlock() = 0;
};

class CMutex : public CLockBase
{
public:
    CMutex();
    virtual ~CMutex();

    virtual bool Lock();
    virtual bool Unlock();
    bool TryLock();

protected:
    pthread_mutex_t m_stMux;
};

class CRLock : public CLockBase
{
public:
    CRLock(pthread_rwlock_t& rwLock);

    virtual bool Lock();
    virtual bool Unlock();
    
protected:
    pthread_rwlock_t& m_rLock;
};

class CWLock : public CLockBase
{
public:
    CWLock(pthread_rwlock_t& rwLock);

    virtual bool Lock();
    virtual bool Unlock();
    
protected:
    pthread_rwlock_t& m_rLock;
};

class CRWLock
{
public:
    CRWLock();
    virtual ~CRWLock();

    virtual CRLock* RLock();
    virtual CWLock* WLock();
    
protected:
    pthread_rwlock_t m_stLock;
    CRLock m_oRLock;
    CWLock m_oWLock;
};

class CGuard
{
public:
    CGuard(CLockBase* pLock);
    ~CGuard();

private:
    CLockBase* m_pLock;
};

NS_TSCPDK_END

#include "TSSynch.inl"

#endif	/* __TSSYNCH_H__ */

