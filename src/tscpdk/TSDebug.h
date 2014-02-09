/* 
 * File:   TSDebug.h
 * Author: thunderliu
 *
 * Created on 2011年12月29日, 上午11:32
 */

#ifndef __TSDEBUG_H__
#define	__TSDEBUG_H__

#include <stdio.h>

#include "TSPlatform.h"

NS_TSCPDK_BEGIN


class TSCPDK_API CDebug
{
public:
    enum DUMP_FLAG
    {
        kShowOffset = 1 << 0,
        kShowChar = 1 << 2
    };
    
public:
    static void SimpleDump(const void* pBuf, size_t uSize, uint32_t dwDumpFlags = kShowOffset);
};

class CStopWatch
{
public:
    CStopWatch();
    virtual ~CStopWatch();

    long Start();
    long Peek();

protected:
    struct timeval m_stTv;
};

typedef enum
{
    E_LL_NONE = 0,
    E_LL_ERR = 1,
    E_LL_DBG = 2
} E_LOG_LVL;

class TSCPDK_API CLog
{
public:
    static void SetLogLevel(E_LOG_LVL eLogLv);
    static void SetOutFile(FILE* pFile);
    static FILE* GetOutFile();

public:
    static E_LOG_LVL m_eLogLv;
    static FILE* m_pFile;
};




#define LOG_OUT(LOG_LVL, fmt, ...) \
do \
{ \
    if (LOG_LVL != E_LL_NONE && LOG_LVL <= CLog::m_eLogLv) \
    { \
        fprintf(CLog::GetOutFile(), (fmt), __VA_ARGS__); \
    } \
} while (false)

#define LOG_DMP(LOG_LVL, buf, size) \
do \
{ \
    if (LOG_LVL != E_LL_NONE && LOG_LVL <= CLog::m_eLogLv) \
    { \
        CDebug::SimpleDump((buf), (size)); \
    } \
} while (false)

//#define LOG_ERR(fmt, args...) LOG_OUT(E_LL_ERR, ("[ "fmt" | %s | %s: %d ]\n"), ##args, __PRETTY_FUNCTION__, __FILE__, __LINE__)
//#define LOG_DBG(fmt, args...) LOG_OUT(E_LL_DBG, ("[ "fmt" | %s | %s: %d ]\n"), ##args, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#ifdef USE_GLOG
#include <glog/logging.h>
#define LOG_ERR(fmt, args...) \
do \
{ \
    char szTmp[128]; \
    snprintf(szTmp, sizeof(szTmp), fmt, ##args); \
    LOG(ERROR) << szTmp; \
} while (false)

#define LOG_DBG(fmt, args...) \
do \
{ \
    char szTmp[128]; \
    snprintf(szTmp, sizeof(szTmp), fmt, ##args); \
    LOG(INFO) << szTmp; \
} while (false)
#elif defined USE_ACELOG
#define LOG_ERR(fmt, args ...) ACE_DEBUG((LM_ERROR, ACE_TEXT("[ " fmt " | (%D|%t) | %s | %s: %d ]\n\n"), ##args, __FUNCTION__, __FILE__, __LINE__))
#define LOG_DBG(fmt, args ...) ACE_DEBUG((LM_DEBUG, ACE_TEXT("[ " fmt " | (%D|%t) | %s | %s: %d ]\n\n"), ##args, __FUNCTION__, __FILE__, __LINE__))
#else
#define LOG_ERR(fmt, ...) LOG_OUT(E_LL_ERR, ("[ " fmt " | %s | %s: %d ]\n"), __VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
#define LOG_DBG(fmt, ...) LOG_OUT(E_LL_DBG, ("[ " fmt " | %s | %s: %d ]\n"), __VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERR_DMP(buf, size) LOG_DMP(E_LL_ERR, buf, size)
#define LOG_DBG_DMP(buf, size) LOG_DMP(E_LL_DBG, buf, size)
#endif // USE_GLOG


#ifdef _DEBUG
#define ASSERT(b) do { unlikely(!b) abort(); } while (false)
#else
#define ASSERT(b) static_cast<void>((b))
#endif

#define VERTIFY(b) do { unlikely(b) abort(); } while (false)

#define UNUSED_ARG(a) do {/* null */} while (&a == 0)

#ifdef TSNETFW_FEATURE_PTRACE
#include <sys/ptrace.h>
#include <sys/user.h>

class CPtrace
{
public:
    CPtrace();

    bool TraceMe();
    bool Attach(pid_t iPid);
    bool Detach();
    bool Continue(int iSignal = 0);
    bool SysCall(int iSignal = 0);
    long ReadText(void* pAddr);
    long ReadData(void* pAddr);
    bool WriteText(const void* pAddr, long lValue);
    bool WriteData(const void* pAddr, long lValue);
    bool ReadRegs(struct user_regs_struct* pRegs);
    bool WriteRegs(const struct user_regs_struct* pRegs);
    bool ReadFpRegs(struct user_fpregs_struct* pRegs);
    bool WriteFpRegs(const struct user_fpregs_struct* pRegs);
    bool Kill();
    bool SingleStep(int iSignal = 0);

    int Wait();
    pid_t GetPid() const;

protected:
    pid_t m_iPid;
};

#endif // TSNETFW_FEATURE_PTRACE

#define UNLIKELY_RET(b, ret, fmt, ...) \
do \
{ \
    unlikely ((b)) \
    { \
        LOG_ERR(fmt, __VA_ARGS__); \
        return (ret); \
    } \
} while (false)


NS_TSCPDK_END

#include "TSDebug.inl"

#endif	/* __TSDEBUG_H__ */

