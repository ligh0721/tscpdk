/* 
* File:   UnitDraw.cpp
* Author: thunderliu
* 
* Created on 2014��2��13��, ����6:34
*/

#include "CommInc.h"
#include "Draw.h"
#include "Ability.h"


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

int CUnitDraw::doAnimation( ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed /*= 1.0f*/ )
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

void CUnitDraw::stopAllActions()
{
}

void CUnitDraw::setVisible( bool bVisible /*= true*/ )
{
}

void CUnitDraw::onUnitDying()
{
}

void CUnitDraw::onUnitTick( float dt )
{
}

void CUnitDraw::setFrame(FRM_ID id)
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
    , m_fHalfOfWidth(0.0f)
    , m_fHalfOfHeight(0.0f)
    , m_fHeight(0.0f)
    , m_fBaseMoveSpeed(CONST_MIN_MOVE_SPEED)
    , m_bFixed(false)
    , m_iMoveToActionId(0)
    , m_iMoveActionId(0)
    , m_iCastActionId(0)
    , m_iCastActiveAbilityId(0)
{
    setDbgClassName("CUnitDraw2D");
}

CUnitDraw2D::~CUnitDraw2D()
{
}

void CUnitDraw2D::onUnitDying()
{
    CUnit* u = getUnit();
    if (u->isDead() == false)
    {
        return;
    }

    die();
}

void CUnitDraw2D::onUnitTick(float dt)
{
    CUnit* u = getUnit();
    if (u->isDoingAnd(CUnit::kCasting) && isDoingAction(getCastActionId()) == false)
    {
        // ����ʩ�����Ҳ�����ʩ��������
        CUnit* u = getUnit();
        CUnitDraw2D* td = NULL;
        CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
        if (pAbility != NULL)
        {
            // ���ʩ�������Դ���
            bool bUnitTarget = getCastTarget().getTargetType() == CCommandTarget::kUnitTarget;
            if (bUnitTarget)
            {
                // ������Ե�λΪĿ��ļ���
                CUnit* t = u->getUnit(getCastTarget().getTargetUnit());
                if (t != NULL && t->isDead() == false)
                {
                    // ��λ�����ҵ�λû������
                    td = DCAST(t->getDraw(), CUnitDraw2D*);
                    assert(td != NULL);
                    getCastTarget().setTargetPoint(td->getPosition());
                }
                else
                {
                    u->endDoing(CUnit::kCasting);
                    setCastActiveAbilityId(0);
                    u->endDoing(CUnit::kObstinate);  // û�гɹ�ʩ������Ҫȡ����ִ״̬
                    return;
                }
            }

            if (checkCastTargetDistance(pAbility, getPosition(), td))
            {
                // ʩ��
                setFlipX(getCastTarget().getTargetPoint().x < getPosition().x);
                castSpell(pAbility);
            }
            else if (bUnitTarget && checkCastTargetDistance(pAbility, getLastMoveToTarget(), td) == false)
            {
                moveToCastPosition(pAbility, td);
            }
            return;
        }
        else
        {
            u->endDoing(CUnit::kObstinate);  // û�гɹ�ʩ������Ҫȡ����ִ״̬
        }
    }

}

int CUnitDraw2D::doMoveTo( const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed /*= 1.0f*/ )
{
    return 0;
}

bool CUnitDraw2D::isFlipX() const
{
    return false;
}

void CUnitDraw2D::setFlipX(bool bFlipX)
{
}

const CUnitDraw2D::UNIT_MOVE_PARAMS CUnitDraw2D::CONST_DEFAULT_MOVE_PARAMS;
const float CUnitDraw2D::CONST_MIN_MOVE_SPEED = 1.0f;
const float CUnitDraw2D::CONST_MAX_MOVE_SPEED = 500.0f;
const float CUnitDraw2D::CONST_MIN_MOVE_SPEED_MULRIPLE = 0.2f; // ��С��Ϊ�����ٶȵ�20%

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
    // ȡ��С�ƶ��ٶȺ���С���ٺ��ٶȵ����ֵ��Ϊ��С�ƶ��ٶ�
    float fMinMoveSpeed = fMoveSpeed * CONST_MIN_MOVE_SPEED_MULRIPLE;
    fMinMoveSpeed = max(CONST_MIN_MOVE_SPEED, fMinMoveSpeed);
    // ����ʵ���ƶ��ٶȣ����ó��������������õ���Сֵ
    fRealMoveSpeed = max(fRealMoveSpeed, fMinMoveSpeed);
    // ����ʵ���ƶ��ٶȣ����ó�������ƶ��ٶ�
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
    float spd = getRealMoveSpeed() / fMoveSpeed;
    setActionSpeed(getMoveActionId(), spd);
    setActionSpeed(getMoveToActionId(), spd);
}

void CUnitDraw2D::cmdMove( const CPoint& roPos, bool bObstinate )
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }

    if (u->isDoingOr(CUnit::kCasting))
    {
        u->endDoing(CUnit::kCasting);
        setCastActiveAbilityId(0);
    }

    if (bObstinate)
    {
        u->startDoing(CUnit::kObstinate);
    }
    else
    {
        u->endDoing(CUnit::kObstinate);
    }

    UNIT_MOVE_PARAMS mp;
    mp.bAutoFlipX = true;
    move(roPos);
}

void CUnitDraw2D::move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    CUnit* u = getUnit();

    if (u->isSuspended())
    {
        return;
    }

    m_oLastMoveToTarget = roPos;

    if (roMoveParams.bAutoFlipX)
    {
        setFlipX(getPosition().x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = getPosition().getDistance(roPos) / max(fMoveSpeed, FLT_EPSILON);
    float fSpeed = getRealMoveSpeed() / fMoveSpeed;

    // ͻ���ƶ�ָ���Ͼ��ƶ�����Ϲ��������ʩ��
    if (isDoingAction(getMoveToActionId()))
    {
        stopAction(getMoveToActionId());
        setMoveToActionId(0);
    }
    if (isDoingAction(getCastActionId()))
    {
        stopAction(getCastActionId());
        setCastActionId(0);
    }
    u->startDoing(CUnit::kMoving);

    int id = doMoveTo(roPos,
        fDur,
        new CCallFuncData(this,
        (FUNC_CALLFUNC_ND)&CUnitDraw2D::onMoveDone),
        fSpeed);
    setMoveToActionId(id);

    if (getMoveActionId() == 0 && u->isDoingOr(CUnit::kSpinning) == false)
    {
        int id = doAnimation(
            kAniMove,
            NULL,
            INFINITE,
            NULL,
            fSpeed);
        setMoveActionId(id);
    }
}

void CUnitDraw2D::follow(int iTargetUnit, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
}

//void CUnitDrawForCC::moveAlongPath(CUnitPath* pPath, bool bObstinate /*= true*/, bool bRestart /*= false*/, float fBufArrive /*= 5.0*/)
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
//    setPathObstinate(bObstinate);
//
//    const CCPoint* pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos);
//    if (pTarget)
//    {
//        UNIT_MOVE_PARAMS oMp;
//        oMp.bObstinate = m_bPathObstinate;
//        moveTo(*pTarget, oMp);
//    }
//}
#if 0
void CUnitDraw2D::setPathObstinate(bool bPathObstinate /*= true*/)
{
    m_bPathObstinate = bPathObstinate;
}

bool CUnitDraw2D::isPathObstinate() const
{
    return m_bPathObstinate;
}
#endif
void CUnitDraw2D::stopMove()
{
    stopAction(getMoveToActionId());
    setMoveToActionId(0);

    stopAction(getMoveActionId());
    setMoveActionId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kMoving);

    setFrame(kFrmDefault);
}

void CUnitDraw2D::onMoveDone(CMultiRefObject* pDraw, CCallFuncData* pData)
{
    CUnit* u = getUnit();
    u->endDoing(CUnit::kObstinate);  // �ƶ�����ֹͣ����Ҫȡ����ִ״̬
    stopMove();
}

int CUnitDraw2D::cmdCastSpell( int iActiveAbilityId, bool bObstinate )
{
    CUnit* u = getUnit();

    // ��λ�Ϸ����ж�
    if (u->isSuspended() || u->isDead())
    {
        // ��������
        return -1;
    }

    // ���ܺϷ����ж�
    CActiveAbility* pAbility = u->getActiveAbility(iActiveAbilityId);
    if (pAbility == NULL)
    {
        // û�ҵ�ָ����������
        return -1;
    }

    // ����Ŀ��Ϸ����ж�
    CUnit* t = NULL;
    CUnitDraw2D* td = NULL;
    switch (pAbility->getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        break;

    case CCommandTarget::kUnitTarget:
        t = u->getUnit(getCastTarget().getTargetUnit());
        if (t == NULL)
        {
            // Ŀ�겻����
            return -1;
        }

        td = DCAST(t->getDraw(), CUnitDraw2D*);
        if (td == NULL)
        {
            // Ŀ�겻�߱�2D��
            return -1;
        }

    case CCommandTarget::kPointTarget:
        if (getCastTarget().getTargetType() != pAbility->getCastTargetType())
        {
            // ָ��ʩ��Ŀ��Ϊ���λ��ʱ���뼼��ʩ��Ŀ�겻��
            return -1;
        }
        break;
    }

    if (pAbility->checkConditions() == false)
    {
        return -1;
    }

    // ʩ������Ϸ���
    bool bDisOk = checkCastTargetDistance(pAbility, getPosition(), td);
    if (bDisOk == false && isFixed())
    {
        return -1;
    }


    // ���Խ���ʩ���������� �� ׷����
    if (bObstinate)
    {
        u->startDoing(CUnit::kObstinate);
    }
    else
    {
        u->endDoing(CUnit::kObstinate);
    }

    bool bSameAbility = (getCastActiveAbilityId() == iActiveAbilityId);
    setCastActiveAbilityId(iActiveAbilityId);
    u->startDoing(CUnit::kCasting);

    if (bDisOk == false)
    {
        moveToCastPosition(pAbility, td);
        return 1;
    }

    // ʩ��
    getCastTarget().setTargetPoint(td->getPosition());
    bool bNeedFlipX = getCastTarget().getTargetType() != CCommandTarget::kNoTarget &&
        isFlipX() != (getCastTarget().getTargetPoint().x < getPosition().x);

    if (bSameAbility && isDoingAction(getCastActionId()) && bNeedFlipX == false)
    {
        // �����ͬһ�����������ͷţ���������ͬһ��������������ʩ��
        return 0;
    }

    if (bNeedFlipX)
    {
        setFlipX(getCastTarget().getTargetPoint().x < getPosition().x);
    }
    castSpell(pAbility);

    return 0;
}

int CUnitDraw2D::castSpell(CActiveAbility* pAbility)
{
    // ����ʩչ����������
    if (isDoingAction(getCastActionId()))
    {
        stopAction(getCastActionId());
    }

    CUnit* u = getUnit();
    if (isDoingAction(getMoveActionId()))
    {
        stopMove();
    }

    if (pAbility->isCoolingDown())
    {
        return -1;
    }

    float spd = 1.0f;
    if (getCastActiveAbilityId() == u->getAttackAbilityId())
    {
        CAttackAct* atk = NULL;
        u->getActiveAbility(getCastActiveAbilityId())->dcast(atk);
        spd = atk->getBaseAttackInterval() / max(FLT_EPSILON, atk->getRealAttackInterval());
    }

    CUnitDraw::ANI_ID aniId = (CUnitDraw::ANI_ID)pAbility->getCastRandomAnimation();
    int id = doAnimation(
        aniId,
        new CCallFuncData(
            this,
            (FUNC_CALLFUNC_ND)&CUnitDraw2D::onCastEffect),
        1,
        new CCallFuncData(
            this,
            (FUNC_CALLFUNC_ND)&CUnitDraw2D::onCastDone),
        spd);
    setCastActionId(id);

    return 0;
}

bool CUnitDraw2D::checkCastTargetDistance( CActiveAbility* pAbility, const CPoint& roPos, CUnitDraw2D* td )
{
    CPoint roPos2;
    switch (getCastTarget().getTargetType())
    {
    case CCommandTarget::kNoTarget:
        return true;

    case CCommandTarget::kUnitTarget:
        roPos2 = td->getPosition();
        break;

    case CCommandTarget::kPointTarget:
        roPos2 = getCastTarget().getTargetPoint();
        break;
    }

    if (pAbility->isCastHorizontal() && fabs(roPos.y - roPos2.y) > CActiveAbility::CONST_MAX_HOR_CAST_Y_RANGE)
    {
        return false;
    }

    float fDis = roPos.getDistance(roPos2) - getHalfOfWidth() - (td != NULL ? td->getHalfOfWidth() : 0.0f);
    if (fDis < pAbility->getCastMinRange() || fDis > pAbility->getCastRange())
    {
        return false;
    }

    return true;
}

void CUnitDraw2D::moveToCastPosition(CActiveAbility* pAbility, CUnitDraw2D* td)
{
    float fDis = (td != NULL ? td->getHalfOfWidth() : 0.0f) + getHalfOfWidth() + (pAbility->getCastMinRange() + pAbility->getCastRange()) * 0.5;
    const CPoint& roPos1 = getPosition();
    const CPoint& roPos2 = (td != NULL ? td->getPosition() : getCastTarget().getTargetPoint());

    UNIT_MOVE_PARAMS oMp;
    if (pAbility->isCastHorizontal())
    {
        // ��սʩ��λ������
        move(CPoint(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y), oMp);
    }
    else
    {
        // Զ��ʩ��λ������
        float fA = -(roPos1 - roPos2).getAngle();
        move(roPos2 + CPoint(cos(fA) * fDis, sin(-fA) * fDis), oMp);
    }
}

void CUnitDraw2D::onCastEffect( CMultiRefObject* pDraw, CCallFuncData* pData )
{
    CUnit* u = getUnit();
    CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
    if (pAbility == NULL)
    {
        return;
    }

    LOG("%s%s%s..", getUnit()->getName(), getUnit()->getAttackAbilityId() == pAbility->getId() ? "��" : "ʩ����", pAbility->getName());
    pAbility->coolDown();
    pAbility->onUnitCastAbility();  // onCastAbility��cd�仯���棬���Կ����������cd���߼�
}

void CUnitDraw2D::onCastDone( CMultiRefObject* pDraw, CCallFuncData* pData )
{
    CUnit* u = getUnit();
    CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
    if (pAbility == NULL)
    {
        return;
    }

    if (u->getAttackAbilityId() != 0 && pAbility->getId() == u->getAttackAbilityId())
    {
        setFrame(kFrmDefault);
        return;
    }

    int iCastedActiveAbilityId = getCastActiveAbilityId();
    stopCast();

    if (iCastedActiveAbilityId == u->getAttackAbilityId())
    {
        cmdCastSpell(iCastedActiveAbilityId, isDoingAction(CUnit::kObstinate));
    }
    else
    {
        // ʩ��(�ǹ���)������ȥ����ִ״̬
        u->endDoing(CUnit::kObstinate);
    }
}

void CUnitDraw2D::stopCast()
{
    stopAction(getCastActionId());
    setCastActionId(0);
    setCastActiveAbilityId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kCasting);
    setFrame(kFrmDefault);
}

void CUnitDraw2D::cmdStop()
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }

    stop();
}

void CUnitDraw2D::stop(bool bDefaultFrame)
{
    stopAction(getMoveToActionId());
    setMoveToActionId(0);

    stopAction(getMoveActionId());
    setMoveActionId(0);

    stopAction(getCastActionId());
    setCastActionId(0);
    setCastActiveAbilityId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kMoving | CUnit::kCasting);

    if (bDefaultFrame)
    {
        setFrame(kFrmDefault);
    }

}

void CUnitDraw2D::die()
{
    stop();
    CUnit* u = getUnit();
    u->startDoing(CUnit::kDying);
    doAnimation(
        CUnitDraw::kAniDie,
        NULL,
        1,
        new CCallFuncData(
            this,
            (FUNC_CALLFUNC_ND)&CUnitDraw2D::onDyingDone),
        1.0f);
}

void CUnitDraw2D::onDyingDone( CMultiRefObject* pDraw, CCallFuncData* pData )
{
    CUnit* u = getUnit();
    CWorld* w = u->getWorld();
    if (u->isDead() == false)
    {
        return;
    }

    u->endDoing(0xFFFFFFFF);
    int id = u->getId();
    w->delUnit(id, u->isRevivable());

    if (w->getUnitToRevive(id) != NULL)
    {
        // !!! ֻ���������ĵ�λ�Żᴥ�����¼���������
        u->onDead();
    }
}

CUnitGroup::CUnitGroup()
{
}

CUnitGroup::CUnitGroup( CWorld* pWorld, const CPoint& roPos, float fRadius, int iMaxCount /*= INFINITE*/, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    if (fRadius < FLT_EPSILON)
    {
        return;
    }

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((int)m_vecUnits.size() >= iMaxCount)
        {
            return;
        }
        if (d->getPosition().getDistance(roPos) < fRadius && (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam))))
        {
            m_vecUnits.addObject(u);
        }

        M_MAP_NEXT;
    }
}

CUnitGroup::CUnitGroup( CWorld* pWorld, int iMaxCount /*= INFINITE*/, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        if ((int)m_vecUnits.size() >= iMaxCount)
        {
            return;
        }

        if (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam)))
        {
            m_vecUnits.addObject(u);
        }

        M_MAP_NEXT;
    }
}

CUnit* CUnitGroup::getUnitByIndex( int iIndex )
{
    if (iIndex < 0 || iIndex >= (int)m_vecUnits.size())
    {

        return NULL;
    }

    return m_vecUnits[iIndex];
}

CUnit* CUnitGroup::getRandomUnit()
{
    return m_vecUnits[rand() % m_vecUnits.size()];
}

CUnit* CUnitGroup::getNearestUnitInRange( const CPoint& roPos, float fRadius, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CUnit* pTarget = NULL;
    float fMinDis = FLT_MAX;
    float fDis;

    M_VEC_FOREACH(m_vecUnits)
    {
        CUnit* u = M_VEC_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((fDis = d->getPosition().getDistance(roPos)) < fRadius && fMinDis > fDis && (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam))))
        {
            pTarget = u;
            fMinDis = fDis;
        }
        M_VEC_NEXT;
    }

    return pTarget;
}

void CUnitGroup::addUnit( CUnit* pUnit )
{
    m_vecUnits.addObject(pUnit);
}

CUnit* CUnitGroup::getNearestUnitInRange( CWorld* pWorld, const CPoint& roPos, float fRadius, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CUnit* pTarget = NULL;
    float fMinDis = FLT_MAX;
    float fDis;

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((fDis = d->getPosition().getDistance(roPos)) < fRadius && fMinDis > fDis && (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam))))
        {
            pTarget = u;
            fMinDis = fDis;
        }
        M_MAP_NEXT;
    }

    return pTarget;
}

void CUnitGroup::cleanUnits()
{
    m_vecUnits.delAllObjects();
}

int CUnitGroup::getUnitsCount()
{
    return m_vecUnits.size();
}

bool CUnitGroup::isLivingAllyOf( CUnit* pUnit, CUnitForce* pParam )
{
    return !pUnit->isDead() && pUnit->isAllyOf(pParam);
}

bool CUnitGroup::isLivingEnemyOf( CUnit* pUnit, CUnitForce* pParam )
{
    return !pUnit->isDead() && pUnit->isEnemyOf(pParam);
}

// CProjectile
CProjectile::CProjectile( const char* pName )
    : m_sName(pName)
    , m_pWorld(NULL)
    , m_fHeight(0.0f)
    , m_fMoveSpeed(300.0f)
    , m_fMaxHeightDelta(0.0f)
    , m_iSourceUnit(0)
    , m_iFromUnit(0)
    , m_iToUnit(0)
    , m_pAttackData(NULL)
    , m_eFromToType(kPointToPoint)
    , m_eFireType(kFireFollow)
{
    setDbgClassName("CProjectile");
}

CProjectile::~CProjectile()
{
    setAttackData(NULL);
}

CMultiRefObject* CProjectile::copy() const
{
    CProjectile* ret = new CProjectile(getName());
    ret->setMoveSpeed(getMoveSpeed());
    ret->setMaxHeightDelta(getMaxHeightDelta());
    ret->setSourceUnit(getSourceUnit());
    ret->setPenaltyFlags(getPenaltyFlags());
    ret->setFromToType(getFromToType());
    ret->setFireType(getFireType());
    return ret;
}

int CProjectile::doLinkUnitToUnit( CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone )
{
    return 0;
}

int CProjectile::doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, CCallFuncData* pOnMoveToDone)
{
    return 0;
}

int CProjectile::doMoveTo( const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone )
{
    return 0;
}

int CProjectile::doAnimation( ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone)
{
    return 0;
}

void CProjectile::stopAction( int tag )
{
}

bool CProjectile::isDoingAction( int id )
{
    return false;
}

void CProjectile::stopAllActions()
{
}

void CProjectile::setVisible( bool bVisible /*= true*/ )
{
}

void CProjectile::die()
{
    stopAllActions();

    doAnimation(
        kAniDie,
        hasPenaltyType(kOnDying) ? new CCallFuncData(this, (FUNC_CALLFUNC_ND)&CProjectile::onEffect) : NULL,
        1,
        new CCallFuncData(this, (FUNC_CALLFUNC_ND)&CProjectile::onDyingDone));
}

void CProjectile::step(float dt)
{
    onTick(dt);
}

void CProjectile::onTick( float dt )
{
}

void CProjectile::onMoveDone( CMultiRefObject* pProjectile, CCallFuncData* pData )
{
    die();
}

void CProjectile::onEffect( CMultiRefObject* pProjectile, CCallFuncData* pData )
{
    CWorld* w = getWorld();
    assert(w != NULL);
    CUnit* s = w->getUnit(getSourceUnit());
    if (s == NULL)
    {
        return;
    }

    s->onProjectileEffect(this);
}

void CProjectile::onDyingDone( CMultiRefObject* pProjectile, CCallFuncData* pData )
{
    stopAllActions();

    CWorld* w = getWorld();
    assert(w != NULL);

    w->delProjectile(getId());
}

void CProjectile::setAttackData( CAttackData* pAttackData )
{
    if (pAttackData == m_pAttackData)
    {
        return;
    }

    if (m_pAttackData != NULL)
    {
        m_pAttackData->release();
    }
    else
    {
        pAttackData->retain();
    }

    m_pAttackData = pAttackData;
}

bool CProjectile::hasPenaltyType(PENALTY_FLAG_BIT ePenaltyType) const
{
    return (m_dwPenaltyFlags & (uint32_t)ePenaltyType) != 0;
}

void CProjectile::fireFollow( const CPoint& rFromPoint, int iToUnit, float fDuration, float fMaxHeightDelta )
{
    setFromToType(kPointToUnit);

    CWorld* w = getWorld();
    assert(w != NULL);

    setFromPoint(rFromPoint);
    CUnit* t = w->getUnit(iToUnit);
    assert(t != NULL);
    setToUnit(iToUnit);

    setPosition(rFromPoint);

    stopAllActions();

    doMoveToUnit(
        t,
        true,
        fMaxHeightDelta,
        fDuration,
        new CCallFuncData(this, (FUNC_CALLFUNC_ND)&CProjectile::onMoveDone));

    doAnimation(
        kAniMove,
        NULL,
        INFINITE,
        NULL);
}

void CProjectile::fireLink( int iFromUnit, int iToUnit )
{
    setFromToType(kUnitToUnit);

    CWorld* w = getWorld();
    assert(w != NULL);

    setFromUnit(iFromUnit);;
    setToUnit(iToUnit);
    
    CUnit* u = w->getUnit(iFromUnit);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    
    CUnit* t = w->getUnit(iToUnit);
    CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
    
    assert(u != NULL && t != NULL && d != NULL && td != NULL);

    setFromPoint(d->getPosition());
    setToPoint(td->getPosition());

#if 0
    float fFromHeight = 0.0f;
    float fOffsetX = 0.0f;
    bool bFlipX = getFromPoint().x > getToPoint().x;

    if (getSourceUnit() == getFromUnit())
    {
        // from��Ϊ����Դ
        fOffsetX = bFlipX ? -d->getFirePoint().x : d->getFirePoint().x;
        fFromHeight = d->getHeight() + d->getFirePoint().y;
    }
    else
    {
        fFromHeight = d->getHeight() + d->getHalfOfHeight();
    }

    float fToHeight = td->getHeight() + td->getHalfOfHeight();

    m_oFromPoint.x += fOffsetX;
    setHeight((fFromHeight + fToHeight) / 2);
    
    CPoint oDelta = m_oToPoint - m_oFromPoint;
    float fR = M_RADIANS_TO_DEGREES(-oDelta.getAngle());

    float fScale = sqrt(oDelta.x * oDelta.x + oDelta.y * oDelta.y) / getSprite()->getContentSize().width;
    pTarget->setPosition(ccp((m_oFromPoint.x + m_oToPoint.x) / 2, (m_oFromPoint.y + m_oToPoint.y) / 2));
    pTarget->setRotation(fR);
    pTarget->setScaleX(fScale);
#endif
    stopAllActions();

    doLinkUnitToUnit(
        u,
        t,
        kAniDie,
        hasPenaltyType(kOnDying) ? new CCallFuncData(this, (FUNC_CALLFUNC_ND)&CProjectile::onEffect) : NULL,
        1,
        new CCallFuncData(this, (FUNC_CALLFUNC_ND)&CProjectile::onDyingDone)
        );
}

void CProjectile::fireLink( int iFromUnit, const CPoint& rToPoint )
{

}

void CProjectile::fireLink( const CPoint& rFromPoint, int iToUnit )
{

}

void CProjectile::fireLink( const CPoint& rFromPoint, const CPoint& rToPoint )
{

}

void CProjectile::fireStraight( const CPoint& rFromPoint, const CPoint& rToPoint, float fDuration, float fMaxHeightDelta )
{

}

void CProjectile::fire()
{
    CWorld* w = getWorld();

    switch (getFireType())
    {
    case kFireFollow:
        {
            assert(getFromToType() == kUnitToUnit || getFromToType() == kPointToUnit);

            if (getFromToType() == kUnitToUnit)
            {
                CUnit* u = w->getUnit(getFromUnit());
                CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

                setFromPoint(CPoint(d->getPosition() + CPoint(d->isFixed() ? -d->getFirePoint().x : d->getFirePoint().x, d->getFirePoint().y)));
            }

            CUnit* t = w->getUnit(getToUnit());
            CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);

            float fDis = getFromPoint().getDistance(td->getPosition() + CPoint(0.0f, td->getHalfOfHeight()));
            fireFollow(getFromPoint(), getToUnit(), fDis / max(FLT_EPSILON, getMoveSpeed()), getMaxHeightDelta());
        }
        

        break;

    case kFireLink:
        {
            switch (getFromToType())
            {
            case kUnitToUnit:
                fireLink(getFromUnit(), getToUnit());
                break;
                
            default:
                assert(false);
            }
            
        }
        break;

    case kFireStraight:
        break;
    }
}
