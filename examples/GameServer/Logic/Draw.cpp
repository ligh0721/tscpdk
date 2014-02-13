/* 
 * File:   UnitDraw.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月13日, 下午6:34
 */

#include "CommInc.h"
#include "Draw.h"


// CUnitDraw
CUnitDraw::CUnitDraw(const char* pName)
{
}

int CUnitDraw::doMoveTo(const CPoint& rPos, CMultiRefObject* pSel, FUNC_CALLFUNC_N pEndCallback, float fSpeed)
{
    return 0;
}

int CUnitDraw::doAnimation(int id, CMultiRefObject* pSel, FUNC_CALLFUNC_N pKeyCallback, int iRepeatTimes, FUNC_CALLFUNC_N pEndCallback, float fSpeed)
{
    return 0;
}

void CUnitDraw::stopAction(int tag)
{
}

void CUnitDraw::setActionSpeed(int tag, float fSpeed)
{
}

void CUnitDraw::setFrame(int id)
{
}

void CUnitDraw::setFlipX(bool bFlipX)
{
}

void CUnitDraw::loadAnimation(int id, const char* pName, int iKeyFrameIndex, float fDelay)
{
}

void CUnitDraw::loadFrame(int id, const char* pName)
{
}
