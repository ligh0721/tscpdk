/* 
 * File:   GameServer.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午7:39
 */

#ifndef __GAMESERVER_H__
#define	__GAMESERVER_H__


USING_NS_TSCPDK;

#include "Logic/Unit.h"
#include "Logic/Application.h"


class CMyWorld : public CWorld
{
public:
    CMyWorld();
    
    virtual void onTick(float dt);
    
    // for test
    virtual void onActEnd(CUnit* pUnit, void* Data);
    
protected:
    int m_iUnitId;
    int m_iUnitId2;
    bool m_bExit;
};

class CMyApp : public CLogicApplication
{
public:
    virtual bool applicationDidFinishLaunching();
    virtual void applicationTick(float fDt);
    
protected:
    CMyWorld* m_pG;
};



#endif	/* __GAMESERVER_H__ */

