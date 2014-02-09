/* 
 * File:   Application.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月4日, 上午3:00
 */

#include "CommInc.h"
#include "Application.h"
#include "MultiRefObject.h"
#include <tscpdk/TSCPDK.h>

USING_NS_TSCPDK;


// CLogicApplication
CLogicApplication * CLogicApplication::sm_pSharedApplication = NULL;

CLogicApplication::CLogicApplication()
: m_lAnimationInterval(0)
, m_lLastTime(0)
, m_fElapsedTime(0.0f)
{
	assert(!sm_pSharedApplication);
	sm_pSharedApplication = this;
}

CLogicApplication::~CLogicApplication()
{
	assert(this == sm_pSharedApplication);
	sm_pSharedApplication = NULL;
}

int CLogicApplication::run()
{
    m_fElapsedTime = 0.0f;
	// Initialize instance and cocos2d.
	if (!applicationDidFinishLaunching())
	{
		return 0;
	}

    m_lLastTime = 0;
	for (;;)
    {
        long lCurTime = getCurMSec();
        if (m_lLastTime)
        {
            float dt = (lCurTime - m_lLastTime) * 0.001;
            m_fElapsedTime += dt;
            applicationTick(dt);
            CAutoReleasePool::sharedAutoReleasePool()->releaseObjects();
        }
        m_lLastTime = lCurTime;
		
		lCurTime = getCurMSec();
		if (lCurTime - m_lLastTime < m_lAnimationInterval)
        {
			SleepForMSec(m_lAnimationInterval - lCurTime + m_lLastTime);
		}
	}
    
	return -1;
}

void CLogicApplication::setAnimationInterval(double interval)
{
	//TODO do something else
	m_lAnimationInterval = interval * 1000.0f;
}

CLogicApplication* CLogicApplication::sharedApplication()
{
	assert(sm_pSharedApplication);
	return sm_pSharedApplication;
}

bool CLogicApplication::applicationDidFinishLaunching()
{
    return true;
}

void CLogicApplication::applicationTick(float fDt)
{
}

long CLogicApplication::getCurMSec()
{
	struct timeval stCurrentTime;
	gettimeofday(&stCurrentTime, NULL);
	return stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec * 0.001; // millseconds
}


// CApplicationLog
void CApplicationLog::Log(const char* pFormat, ...)
{
    fprintf(stdout, "[%03.2f] ", CLogicApplication::sharedApplication()->getElapsedTime());
    va_list argv;
    va_start(argv, pFormat);
    vfprintf(stdout, pFormat, argv);
    va_end(argv);
    fprintf(stdout, "\n");
}

