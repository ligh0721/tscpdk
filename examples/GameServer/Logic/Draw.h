/* 
 * File:   UnitDraw.h
 * Author: thunderliu
 *
 * Created on 2014��2��13��, ����6:34
 */

#ifndef __DRAW_H__
#define	__DRAW_H__

#include "Base.h"
#include "MultiRefObject.h"
#include "Unit.h"


class CCallFuncData : public CMultiRefObject
{
public:
    CCallFuncData(CMultiRefObject* pSel, FUNC_CALLFUNC_ND pCallFunc);

    M_SYNTHESIZE(CMultiRefObject*, m_pSelector, Selector);
    M_SYNTHESIZE(FUNC_CALLFUNC_ND, m_pCallFunc, CallFunc);
};

class CUnitDraw : public CMultiRefObject
{
public:
    CUnitDraw(const char* pName);
    virtual ~CUnitDraw();

    M_SYNTHESIZE_STR(Name);

    M_SYNTHESIZE(CUnit*, m_pUnit, Unit);

    enum ANI_ID
    {
        kAniMove,
        kAniDie,
        kAniAct1,
        kAniAct2,
        kAniAct3,
        kAniAct4,
        kAniAct5,
        kAniAct6,
        kAniAct7,
        kAniAct8
    };

    // ����actionTag
    virtual int doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();
    
    virtual void setVisible(bool bVisible = true);

    virtual void onUnitDying();
    virtual void onUnitTick(float dt);

    enum FRM_ID
    {
        kFrmDefault
    };
    
    virtual void setFrame(FRM_ID id);
    virtual void setFlipX(bool bFlipX);
    
    virtual void loadAnimation(int id, const char* pName, int iNotifyFrameIndex, float fDelay);
    virtual void loadFrame(int id, const char* pName);

};

class CUnitDraw2D : public CUnitDraw
{
public:
    CUnitDraw2D(const char* pName);
    virtual ~CUnitDraw2D();

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oPosition, Position);

    M_SYNTHESIZE(float, m_fHalfOfWidth, HalfOfWidth);
    M_SYNTHESIZE(float, m_fHalfOfHeight, HalfOfHeight);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oFirePoint, FirePoint);

    virtual void onUnitDying();
    virtual void onUnitTick(float dt);

    M_SYNTHESIZE(float, m_fHeight, Height);

    /////////////////////// move //////////////////////////////
    virtual int doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed = 1.0f);
    
    virtual void setFlipX(bool bFlipX = true);
    virtual bool isFlipX() const;

    struct UNIT_MOVE_PARAMS
    {
        UNIT_MOVE_PARAMS(
            //bool bObstinate_ = true,
            bool bAutoFlipX_ = true,
            float fMaxOffsetY_ = 0.0f
            //bool bCancelCast_ = true
            )
            //: bObstinate(bObstinate_)
            : bAutoFlipX(bAutoFlipX_)
            , fMaxOffsetY(fMaxOffsetY_)
            //, bCancelCast(bCancelCast_)
        {}
        //bool bObstinate;
        bool bAutoFlipX;
        float fMaxOffsetY;
        //bool bCancelCast;
    };

    static const UNIT_MOVE_PARAMS CONST_DEFAULT_MOVE_PARAMS;
    static const float CONST_MIN_MOVE_SPEED;
    static const float CONST_MAX_MOVE_SPEED;
    static const float CONST_MIN_MOVE_SPEED_MULRIPLE;

    M_SYNTHESIZE_READONLY(float, m_fBaseMoveSpeed, BaseMoveSpeed);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeed, ExMoveSpeed);
    M_SYNTHESIZE_BOOL(Fixed);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oMoveTarget, MoveTarget);

    void setBaseMoveSpeed(float fMoveSpeed);
    void setExMoveSpeed(const CExtraCoeff& roExMoveSpeed);

    float getRealMoveSpeed() const;

    void updateMoveSpeedDelta();


    M_SYNTHESIZE(int, m_iMoveToActionId, MoveToActionId);
    M_SYNTHESIZE(int, m_iMoveActionId, MoveActionId);
    void cmdMove(const CPoint& roPos, bool bObstinate = true);
    void move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    void follow(int iTargetUnit, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    //virtual void moveAlongPath(CUnitPath* pPath, bool bObstinate = true, bool bRestart = false, float fBufArrive = 5.0);
    void stopMove();
    void onMoveDone(CMultiRefObject* pUnit, CCallFuncData* pData);

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oLastMoveToTarget, LastMoveToTarget);
    //M_SYNTHESIZE(uint32_t, m_dwPathCurPos, PathCurPos);
    //virtual void setPathObstinate(bool bPathObstinate = true);
    //virtual bool isPathObstinate() const;

    
    // /////////////////////// cast /////////////////////////////
    M_SYNTHESIZE_PASS_BY_REF(CCommandTarget, m_oCastTarget, CastTarget);
    M_SYNTHESIZE(int, m_iCastActionId, CastActionId);
    M_SYNTHESIZE(int, m_iCastActiveAbilityId, CastActiveAbilityId);
    int cmdCastSpell(int iActiveAbilityId, bool bObstinate = true);  // ������ʩ��ʧ�ܣ�ʩ���У�ʩ��׷���У����Է�������Ϊint
    int castSpell(CActiveAbility* pAbility);
    bool checkCastTargetDistance(CActiveAbility* pAbility, const CPoint& roPos, CUnitDraw2D* td);
    void moveToCastPosition(CActiveAbility* pAbility, CUnitDraw2D* td);

    void stopCast();
    void onCastEffect(CMultiRefObject* pDraw, CCallFuncData* pData);
    void onCastDone(CMultiRefObject* pDraw, CCallFuncData* pData);

    void cmdStop();
    void stop(bool bDefaultFrame = true);

    void die();
    void onDyingDone(CMultiRefObject* pDraw, CCallFuncData* pData);
};

typedef bool (*FUNC_UNIT_CONDITION)(CUnit* pUnit, void* pParam);

class CUnitGroup : public CMultiRefObject
{
public:
    typedef CMultiRefVec<CUnit*> VEC_UNITS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_UNITS, m_vecUnits, Units);

public:
    CUnitGroup();
    CUnitGroup(CWorld* pWorld, const CPoint& roPos, float fRadius, int iMaxCount = INFINITE, FUNC_UNIT_CONDITION pBoolFunc = NULL, void* pParam = NULL);
    CUnitGroup(CWorld* pWorld, int iMaxCount = INFINITE, FUNC_UNIT_CONDITION pBoolFunc = NULL, void* pParam = NULL);

    CUnit* getUnitByIndex(int iIndex);
    CUnit* getRandomUnit();
    CUnit* getNearestUnitInRange(const CPoint& roPos, float fRadius, FUNC_UNIT_CONDITION pBoolFunc = NULL, void* pParam = NULL);
    void addUnit(CUnit* pUnit);

    static CUnit* getNearestUnitInRange(CWorld* pWorld, const CPoint& roPos, float fRadius, FUNC_UNIT_CONDITION pBoolFunc = NULL, void* pParam = NULL);

    void cleanUnits();
    int getUnitsCount();

    //virtual void setRangePosition(const CPoint& roPos, float fRadius);
    //virtual void turnTo(bool bLeft);
    //virtual void move(const CPoint& roPos, const CUnit::UNIT_MOVE_PARAMS& roMoveParams = CUnit::CONST_DEFAULT_MOVE_PARAMS);
    //virtual void followTo(int iTargetKey, const CUnit::UNIT_MOVE_PARAMS& roMoveParams = CUnit::CONST_DEFAULT_MOVE_PARAMS);
    //virtual void stopMove();
    //virtual void attack(int iTargetKey, bool bObstinate = true);
    //virtual void stopAttack();
    //virtual void moveAlongPath(CUnitPath* pPath, bool bObstinate = true, bool bRestart = false, float fBufArrive = 5.0);

    //void damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);
    //void damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);
    //void damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask = CUnit::kNoMasked);
    //void addActiveAbility(CAbility* pAbility);
    //void addBuff(CBuffAbility* pBuff, bool bForce = false);

    static bool isLivingAllyOf(CUnit* pUnit, CUnitForce* pParam);
    static bool isLivingEnemyOf(CUnit* pUnit, CUnitForce* pParam);

};

class CProjectile : public CMultiRefObject
{
public:
    CProjectile(const char* pName);
    virtual ~CProjectile();
    virtual CMultiRefObject* copy() const;

    M_SYNTHESIZE_STR(Name);

    M_SYNTHESIZE(CWorld*, m_pWorld, World);

    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oPosition, Position);
    M_SYNTHESIZE(float, m_fHeight, Height);

    M_SYNTHESIZE(float, m_fMoveSpeed, MoveSpeed);
    M_SYNTHESIZE(float, m_fMaxHeightDelta, MaxHeightDelta);

    enum ANI_ID
    {
        kAniMove,
        kAniDie
    };

    virtual int doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone);
    virtual int doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, CCallFuncData* pOnMoveToDone);
    virtual int doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone);
    virtual int doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone);
    virtual void stopAction(int tag);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();

    virtual void setVisible(bool bVisible = true);

    enum FRM_ID
    {
        kFrmDefault
    };

    void die();

    virtual void step(float dt);
    virtual void onTick(float dt);
    void onMoveDone(CMultiRefObject* pProjectile, CCallFuncData* pData);
    void onEffect(CMultiRefObject* pProjectile, CCallFuncData* pData);
    void onDyingDone(CMultiRefObject* pProjectile, CCallFuncData* pData);

    // ��λ��������ǽ�����ϵ������λ�����������ﲻһ�����ͷţ����Ա���ͨ��ID����
    M_SYNTHESIZE(int, m_iSourceUnit, SourceUnit);  // ������������λ
    M_SYNTHESIZE(int, m_iFromUnit, FromUnit);  // ���䵥λ
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oFromPoint, FromPoint);
    M_SYNTHESIZE(int, m_iToUnit, ToUnit);  // Ŀ��λ��
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oToPoint, ToPoint);

    M_SYNTHESIZE_READONLY(CAttackData*, m_pAttackData, AttackData);
    void setAttackData(CAttackData* pAttackData);

    enum PENALTY_FLAG_BIT
    {
        kOnDying = 1 << 0,
        kOnContact = 1 << 1
    };

    M_SYNTHESIZE(uint32_t, m_dwPenaltyFlags, PenaltyFlags);
    bool hasPenaltyType(PENALTY_FLAG_BIT ePenaltyType) const;

    enum FROM_TO_TYPE
    {
        kPointToUnit,
        kPointToPoint,
        kUnitToUnit,
        kUnitToPoint
    };

    M_SYNTHESIZE(FROM_TO_TYPE, m_eFromToType, FromToType);

    enum FIRE_TYPE
    {
        kFireFollow,
        kFireLink,
        kFireStraight
    };

    M_SYNTHESIZE(FIRE_TYPE, m_eFireType, FireType);

    void fireFollow(const CPoint& rFromPoint, int iToUnit, float fDuration, float fMaxHeightDelta);
    
    void fireLink(const CPoint& rFromPoint, int iToUnit);  // ��-��λ
    void fireLink(const CPoint& rFromPoint, const CPoint& rToPoint);  // ��-��
    void fireLink(int iFromUnit, int iToUnit);  // ��λ-��λ
    void fireLink(int iFromUnit, const CPoint& rToPoint);  // ��λ-��

    void fireStraight(const CPoint& rFromPoint, const CPoint& rToPoint, float fDuration, float fMaxHeightDelta);

    void fire();

protected:

};


#endif	/* __DRAW_H__ */

