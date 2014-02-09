/* 
 * File:   Application.h
 * Author: thunderliu
 *
 * Created on 2014年2月4日, 上午3:00
 */

#ifndef __APPLICATION_H__
#define	__APPLICATION_H__


class CLogicApplication
{
public:
	CLogicApplication();
	virtual ~CLogicApplication();

	void setAnimationInterval(double interval);
	int run();

	static CLogicApplication* sharedApplication();
    static long getCurMSec();
    
    virtual bool applicationDidFinishLaunching();
    virtual void applicationTick(float fDt);
    
protected:
    long m_lLastTime;
    long m_lAnimationInterval;  // micro second
    static CLogicApplication* sm_pSharedApplication;
    
};


#endif	/* __APPLICATION_H__ */

