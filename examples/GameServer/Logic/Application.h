/* 
 * File:   Application.h
 * Author: thunderliu
 *
 * Created on 2014年2月4日, 上午3:00
 */

#ifndef __APPLICATION_H__
#define	__APPLICATION_H__

#include "Action.h"



class CLogicApplication
{
protected:
    static CLogicApplication* sm_pSharedApplication;
    
public:
	CLogicApplication();
	virtual ~CLogicApplication();

protected:
    long m_lAnimationInterval;  // micro second
    long m_lLastTime;
    
public:
	void setAnimationInterval(double interval);
	int run();

	static CLogicApplication* sharedApplication();
    static long getCurMSec();
    
    virtual bool applicationDidFinishLaunching();
    virtual void applicationTick(float fDt);
    
    M_SYNTHESIZE_READONLY(float, m_fElapsedTime, ElapsedTime);
    
};

class CApplicationLog
{
public:
    static void Log(const char* pFormat, ...);
};

#define LOG(fmt, ...) CApplicationLog::Log((fmt), __VA_ARGS__)

#endif	/* __APPLICATION_H__ */

