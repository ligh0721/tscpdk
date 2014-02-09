/* 
 * File:   TSThread.cpp
 * Author: thunderliu
 * 
 * Created on 2011年12月25日, 上午1:20
 */

#define LIBTSCPDK_SRC

#include "TSPlatform.h"
#include <signal.h>

#include "TSThread.h"

#include <stdio.h>
#include <string.h>


NS_TSCPDK_BEGIN

bool CThread::Start()
{
    if (IS_PTH_VALID(m_uHdl))
    {
        return false;
    }

    int iRes = pthread_create(&m_uHdl, NULL, (void*(*)(void*)) & CThread::ThreadRoutine, this);
    if (iRes < 0)
    {
        perror("pthread create err");
        return false;
    }

    return true;
}

bool CThread::Wait()
{
    void* pExitCode = NULL;
    int iRet = pthread_join(m_uHdl, &pExitCode);
    memset(&m_uHdl, 0, sizeof(m_uHdl));
    return iRet >= 0;
}

bool CThread::Cancel()
{
    bool bRet = pthread_cancel(m_uHdl) >= 0;
    //bool bRet = pthread_kill(m_tHdl, SIGKILL) >= 0;
    //CThread::Wait(); // user should call Wait()
    return bRet;
}

void* CThread::ThreadRoutine(CThread* pInstance)
{
    CThread::SetCancelType(CThread::CANCEL_ASYNCHRONOUS);
    void* pExitCode = NULL;
    pthread_cleanup_push((void(*)(void*)) & CThread::CleanUpRoutine, pInstance);
    pExitCode = (void*)pInstance->ThreadProc();
    pInstance->m_pExitCode = pExitCode;
    pthread_exit(pExitCode);
    pthread_cleanup_pop(0); // 不执行就泄露
    return pExitCode;
}

void CThread::CleanUpRoutine(CThread* pInstance)
{
    //void* pExitCode = NULL;
    //pthread_join(pInstance->m_tHdl, &pExitCode);
    pthread_join(pInstance->m_uHdl, NULL);
    pInstance->OnExit(pInstance->m_pExitCode);
}

NS_TSCPDK_END

