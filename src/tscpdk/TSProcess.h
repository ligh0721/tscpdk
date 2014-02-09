/* 
 * File:   TSProcess.h
 * Author: thunderliu
 *
 * Created on 2011年12月30日, 下午3:58
 */

#ifndef __TSPROCESS_H__
#define	__TSPROCESS_H__


#if !defined(TSCPDK_MSVC)

#include <unistd.h>
#include <wait.h>


NS_TSCPDK_BEGIN

class CProcessGroup;


// CProcess

class CProcess
{
    friend class CProcessGroup;
public:
    CProcess();
    virtual ~CProcess();

protected:
    CProcess(pid_t iPid);

public:
    static CProcess* Attach(pid_t iPid);
    static CProcess* ForkProcess();
    static CProcess* CreateProcess(const char* pPath, char* const pArgv[], char* const pEnvp[]);

    virtual bool Start();
    virtual bool Wait();
    bool IsMe() const;
    pid_t GetPid() const;
    bool Release();


protected:
    virtual int ProcessProc();
    //long ParentProc();

protected:
    pid_t m_iPid;

};


// CProcessGroup

class CProcessGroup
{
protected:
    CProcessGroup(int iCount, pid_t aiPid[]);
    virtual ~CProcessGroup();
    
public:
    static CProcessGroup* ForkHere(int iChildCount);
    bool Wait();
    bool Release();

protected:
    int m_iCount;
    CProcess* m_pProcess;
};

NS_TSCPDK_END

#endif  // TSCPDK_MSVC

#include "TSProcess.inl"

#endif	/* __TSPROCESS_H__ */

