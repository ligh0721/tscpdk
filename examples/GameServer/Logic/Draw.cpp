/* 
 * File:   UnitDraw.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月13日, 下午6:34
 */

#include "CommInc.h"
#include "Draw.h"


// CCallFuncData
CCallFuncData::CCallFuncData( CMultiRefObject* pSel, FUNC_CALLFUNC_ND pCallFunc)
    : m_pSelector(pSel)
    , m_pCallFunc(pCallFunc)
{
    setDbgClassName("CCallFuncData");
}

// CUnitDraw
CUnitDraw::CUnitDraw(const char* pName)
    : m_sName(pName)
    , m_pUnit(NULL)
{
    setDbgClassName("CUnitDraw");
}

CUnitDraw::~CUnitDraw()
{
}

int CUnitDraw::doAnimation( int id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed /*= 1.0f*/ )
{
    return 0;
}

void CUnitDraw::stopAction(int tag)
{
}

void CUnitDraw::setActionSpeed(int tag, float fSpeed)
{
}

bool CUnitDraw::isDoingAction( int id )
{
    return false;
}

void CUnitDraw::setFrame(int id)
{
}

void CUnitDraw::setFlipX(bool bFlipX)
{
}

void CUnitDraw::loadAnimation(int id, const char* pName, int iNotifyFrameIndex, float fDelay)
{
}

void CUnitDraw::loadFrame(int id, const char* pName)
{
}

// CUnitDraw2D
CUnitDraw2D::CUnitDraw2D( const char* pName )
    : CUnitDraw(pName)
    , m_fBaseMoveSpeed(CONST_MIN_MOVE_SPEED)
    , m_bFixed(false)
    , m_bFlip(false)
{
    setDbgClassName("CUnitDraw2D");
}

CUnitDraw2D::~CUnitDraw2D()
{
}

int CUnitDraw2D::doMoveTo( const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed /*= 1.0f*/ )
{
    return 0;
}

const CUnitDraw2D::UNIT_MOVE_PARAMS CUnitDraw2D::CONST_DEFAULT_MOVE_PARAMS;
const float CUnitDraw2D::CONST_MIN_MOVE_SPEED = 1.0f;
const float CUnitDraw2D::CONST_MAX_MOVE_SPEED = 500.0f;
const float CUnitDraw2D::CONST_MIN_MOVE_SPEED_MULRIPLE = 0.2f; // 最小变为基础速度的20%

void CUnitDraw2D::setBaseMoveSpeed(float fMoveSpeed)
{
    fMoveSpeed = max(fMoveSpeed, CONST_MIN_MOVE_SPEED);
    fMoveSpeed = min(fMoveSpeed, CONST_MAX_MOVE_SPEED);
    m_fBaseMoveSpeed = fMoveSpeed;
	updateMoveSpeedDelta();
}

void CUnitDraw2D::setExMoveSpeed(const CExtraCoeff& roExMoveSpeed)
{
    m_oExMoveSpeed = roExMoveSpeed;
    updateMoveSpeedDelta();
}

float CUnitDraw2D::getRealMoveSpeed() const
{
    float fMoveSpeed = getBaseMoveSpeed();
    float fRealMoveSpeed = m_oExMoveSpeed.getValue(fMoveSpeed);
    // 取最小移动速度和最小减速后速度的最大值作为最小移动速度
    float fMinMoveSpeed = fMoveSpeed * CONST_MIN_MOVE_SPEED_MULRIPLE;
    fMinMoveSpeed = max(CONST_MIN_MOVE_SPEED, fMinMoveSpeed);
    // 计算实际移动速度，不得超过上述计算所得的最小值
    fRealMoveSpeed = max(fRealMoveSpeed, fMinMoveSpeed);
    // 计算实际移动速度，不得超过最大移动速度
    return min(fRealMoveSpeed, CONST_MAX_MOVE_SPEED);
}

void CUnitDraw2D::updateMoveSpeedDelta()
{
    float fMoveSpeed = getBaseMoveSpeed();
    if (fMoveSpeed < FLT_EPSILON)
    {
        stopMove();
        return;
    }
	
}

void CUnitDraw2D::move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }
    m_oLastMoveToTarget = roPos;
    
    if (roMoveParams.bAutoFlipX)
    {
        //u->turnTo(roOrg.x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = getPosition().getDistance(roPos) / max(fMoveSpeed, FLT_EPSILON);
    float fSpeed = getRealMoveSpeed() / fMoveSpeed;

    doMoveTo(roPos,
             fDur,
             new CCallFuncData(this,
                               (FUNC_CALLFUNC_ND)&CUnitDraw2D::onMoveDone),
             fSpeed);

    // 突发移动指令，打断旧移动，打断攻击，打断施法
    if (u->isDoingOr(CUnit::kMoving))
    {
        stopAction(kActMoveTo);
    }
    if (u->isDoingOr(CUnit::kCasting) && roMoveParams.bCancelCast)
    {
        //stopCast();
    }
    
    if (u->isDoingOr(CUnit::kSpinning) == false)
    {
        doAnimation(kActMove,
                    NULL,
                    INFINITE,
                    NULL,
                    fSpeed);
    }

    u->startDoing(CUnit::kMoving);
    if (roMoveParams.bIntended)
    {
        u->startDoing(CUnit::kIntended);
    }
    else
    {
        u->endDoing(CUnit::kIntended);
    }
}

void CUnitDraw2D::follow(int iTargetUnit, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    /*
    if (isDead() || isFixed())
    {
        return;
    }
    M_DEF_GM(pGm);
    CGameUnit* pTarget = getUnitLayer()->getUnitByKey(iTargetKey);
    if (!pTarget)
    {
        return;
    }
    const CCPoint& roPos = pTarget->getPosition();
    m_oLastMoveToTarget = roPos;
    if (isDoingOr(kSuspended))
    {
        return;
    }
    CCPoint roOrg = getPosition();
    if (roMoveParams.bAutoFlipX)
    {
        turnTo(roOrg.x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = pGm->getDistance(roOrg, roPos) / fMoveSpeed;
    CCActionInterval* pSeq = CCSequence::createWithTwoActions(
                                                              CCMoveToNode::create(fDur, pTarget->getSprite(), true, roMoveParams.fMaxOffsetY, 1, pTarget->getHalfOfHeight()),
                                                              CCCallFuncN::create(this, callfuncN_selector(CUnit::onActMoveEnd))
                                                             );
    float fDelta = getRealMoveSpeed() / fMoveSpeed;
    CCSpeed* pActMoveTo = CCSpeed::create(pSeq, fDelta);
    pActMoveTo->setTag(kActMoveTo);
    // 突发移动指令，打断旧移动，打断攻击
    if (isDoingOr(kMoving))
    {
        m_oSprite.stopActionByTag(kActMoveTo);
    }
    if (isDoingOr(kAttacking) && roMoveParams.bCancelAttack)
    {
        stopAttack();
    }
    if (isDoingOr(kCasting) && roMoveParams.bCancelCast)
    {
        stopCast();
    }
    m_oSprite.runAction(pActMoveTo);
    if (!isDoingOr(kSpinning))
    {
        setAnimation(kAnimationMove, -1, fDelta, kActMove, NULL);
    }

    startDoing(kMoving);
    if (roMoveParams.bIntended)
    {
        startDoing(kIntended);
    }
    else
    {
        endDoing(kIntended);
    }
    */
}

//void CUnitDrawForCC::moveAlongPath(CUnitPath* pPath, bool bIntended /*= true*/, bool bRestart /*= false*/, float fBufArrive /*= 5.0*/)
//{
//    if (pPath != m_pMovePath)
//    {
//        CC_SAFE_RETAIN(pPath);
//        CC_SAFE_RELEASE(m_pMovePath);
//        m_pMovePath = pPath;
//    }
//
//    if (!m_pMovePath)
//    {
//        return;
//    }
//
//    if (bRestart)
//    {
//        m_dwPathCurPos = 0;
//    }
//
//    if (m_fPathBufArrive != fBufArrive)
//    {
//        m_fPathBufArrive = MAX(FLT_EPSILON, fBufArrive);
//    }
//
//    setPathIntended(bIntended);
//
//    const CCPoint* pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos);
//    if (pTarget)
//    {
//        UNIT_MOVE_PARAMS oMp;
//        oMp.bIntended = m_bPathIntended;
//        moveTo(*pTarget, oMp);
//    }
//}
#if 0
void CUnitDraw2D::setPathIntended(bool bPathIntended /*= true*/)
{
    m_bPathIntended = bPathIntended;
}

bool CUnitDraw2D::isPathIntended() const
{
    return m_bPathIntended;
}
#endif
void CUnitDraw2D::stopMove()
{
    stopAction(kActMove);
    stopAction(kActMoveTo);
}

void CUnitDraw2D::onMoveDone(CMultiRefObject* pUnit, CCallFuncData* pData)
{
    stopMove();
}

void CUnitDraw2D::startMoveAct(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams/* = CONST_DEFAULT_MOVE_PARAMS*/)
{
}

void CUnitDraw2D::stopMoveAct()
{
}
