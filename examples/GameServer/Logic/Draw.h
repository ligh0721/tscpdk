/* 
 * File:   UnitDraw.h
 * Author: thunderliu
 *
 * Created on 2014年2月13日, 下午6:34
 */

#ifndef __UNITDRAW_H__
#define	__UNITDRAW_H__

#include "Base.h"
#include "MultiRefObject.h"


class CUnitDraw
{
public:
    CUnitDraw(const char* pName);
    // 返回actionTag
    virtual int doMoveTo(const CPoint& rPos, CMultiRefObject* pSel, FUNC_CALLFUNC_N pEndCallback, float fSpeed = 1.0f);
    virtual int doAnimation(int id, CMultiRefObject* pSel, FUNC_CALLFUNC_N pKeyCallback, int iRepeatTimes, FUNC_CALLFUNC_N pEndCallback, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    
    virtual void setFrame(int id);
    virtual void setFlipX(bool bFlipX);
    
    virtual void loadAnimation(int id, const char* pName, int iKeyFrameIndex, float fDelay);
    virtual void loadFrame(int id, const char* pName);
};

#endif	/* __UNITDRAW_H__ */

