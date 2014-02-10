/* 
 * File:   Unit.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午10:55
 */

#ifndef __UNIT_H__
#define __UNIT_H__

#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"


// 攻击数值，由多种类型的攻击组合而成
class CAttackValue
{
public:
    static const int CONST_MAX_ATTACK_TYPE = 4;
    enum ATTACK_TYPE
    {
        kPhysical = 0,
        kMagical = 1,
        kSiege = 2,
        kHoly = CONST_MAX_ATTACK_TYPE - 1
    };
    
    static const int CONST_MAX_NAME_INDEX = 2;
    enum NAME_INDEX
    {
        kEnName = 0,
        kCnName = CONST_MAX_NAME_INDEX - 1
    };
    
    static const char* CONST_ARR_NAME[CONST_MAX_ATTACK_TYPE][CONST_MAX_NAME_INDEX];
    typedef float ARR_ATTACK_VALUES[CONST_MAX_ATTACK_TYPE];
    
public:
    CAttackValue();
    CAttackValue(int iCount, ATTACK_TYPE eType1, float fValue1, ...);
    
    float getValue(ATTACK_TYPE eType) const;
    void setValue(ATTACK_TYPE eType, float fValue);
    void setAttackValue(const CAttackValue& roAttackValue);
    void setValueZero();
    
    static const char* getName(ATTACK_TYPE eType, int iIndex = 0);
    
    const CAttackValue& operator=(const CAttackValue& roAttackValue);
    
    ARR_ATTACK_VALUES m_afValues;
};

// 护甲数值，由多种类型的护甲组合而成
class CArmorValue
{
public:
    static const int CONST_MAX_ARMOR_TYPE = 6;
    enum ARMOR_TYPE
    {
        kNormal = 0,
        kHeavy = 1,
        kCrystal = 2,
        kWall = 3,
        kHero = 4,
        kHoly = CONST_MAX_ARMOR_TYPE - 1
    };
    
    static const int CONST_MAX_NAME_INDEX = 2;
    enum NAME_INDEX
    {
        kEnName = 0,
        kCnName = CONST_MAX_NAME_INDEX - 1
    };
    
    static const char* CONST_ARR_NAME[CONST_MAX_ARMOR_TYPE][CONST_MAX_NAME_INDEX];
    typedef float ARR_ARMOR_VALUES[CONST_MAX_ARMOR_TYPE];
    
public:
    CArmorValue();
    CArmorValue(int iCount, ARMOR_TYPE eType1, float fValue1, ...);
    
    float getValue(ARMOR_TYPE eType) const;
    void setValue(ARMOR_TYPE eType, float fValue);
    void setArmorValue(const CArmorValue& roArmorValue);
    void setValueZero();
    
    static const char* getName(ARMOR_TYPE eType, int iIndex = 0);
    
    const CArmorValue& operator=(const CArmorValue& roArmorValue);
    
    ARR_ARMOR_VALUES m_afValues;
};

// 攻击-护甲计算系数
extern float g_afAttackArmorTable[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_ATTACK_TYPE];

class CAttackBuff
{
public:
    CAttackBuff(int iTemplateBuff, int iBuffLevel);
    
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE(int, m_iBuffLevel, BuffLevel);
};

class CAttackData : public CMultiRefObject
{
public:
    typedef vector<CAttackBuff> VEC_ATTACK_BUFF;
    
public:
    CAttackData();

    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oAtkVal, AttackValue);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ATTACK_BUFF, m_vecAtkBuffs, AttackBuffs);
    
    void setAttackValue(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue);
    void addAttackBuff(const CAttackBuff& rAttackBuff);
};

// f = ax + b，x为基准值，a和b分为计算乘数和加数，应用情形：触发重击将造成自身攻击力的2倍并额外附加30点的伤害；提升自身20%的基础力量值
// 应用该接结构，可轻易解决装备武器/新增BUFF，卸载武器/删除BUFF后的属性一致
class CExtraCoeff
{
public:
    CExtraCoeff();
    CExtraCoeff(float fMulriple, float fAddend);
    
    float getValue(float fBase) const;
    
    M_SYNTHESIZE(float, m_fMulriple, Mulriple);
    M_SYNTHESIZE(float, m_fAddend, Addend);
};

class CUnitForce
{
public:
    enum DAMAGE_TARGET_FLAG_BIT
    {
        kSelf = 1 << 0,
        kOwn = 1 << 1,
        kAlly = 1 << 2,
        kEnemy = 1 << 3,
    };
    
public:
    CUnitForce();
    
    bool isAllyOf(const CUnitForce* pForce) const;
    bool isEnemyOf(const CUnitForce* pForce) const;
    
    void setForceByIndex(int iForceIndex);
    
    M_SYNTHESIZE(uint32_t, m_dwForceFlag, Force);
    M_SYNTHESIZE(uint32_t, m_dwAllyMaskFlag, Ally);
};


class CAction;
class CUnit;

class CActionManager
{
public:
    typedef CMultiRefVec<CAction*> VEC_ACTIONS;
    
public:
    CActionManager();
    
    void addAction(CAction* pAction, CUnit* pTarget);
    CAction* getActionByTag(int iTag);
    
    virtual void onTick(float dt);
    
protected:
    VEC_ACTIONS m_vecActions;
};


class CProjectile;
class CSkill;
class CPassiveSkill;
class CBuffSkill;
class CActiveSkill;
class CWorld;

class CCommandTarget
{
public:
    enum TARGET_TYPE
    {
        kNoTarget = 0,
        kPointTarget,
        kUnitTarget
    };
    
public:
    CCommandTarget();
    CCommandTarget(const CPoint& rTargetPoint);
    CCommandTarget(int iTargetUnit);
    
    M_SYNTHESIZE(TARGET_TYPE, m_eTargetType, TargetType);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oTargetPoint, TargetPoint);
    M_SYNTHESIZE(int, m_iTargetUnit, TargetUnit);
};

class CUnitEventAdapter
{
public:
    CUnitEventAdapter();
    virtual ~CUnitEventAdapter();
    
    inline virtual void onUnitLevelChange(int iChanged) {}
    inline virtual void onUnitRevive() {}
    inline virtual void onUnitDie() {}
    inline virtual void onUnitHpChange(float fChanged) {}
    inline virtual void onUnitTick(float dt) {}
    inline virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget) { return pAttack; }
    inline virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource) { return pAttack; }
    inline virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource) {}
    inline virtual void onUnitDamagedDone(float fDamage, CUnit* pSource) {}
    inline virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget) {}
    inline virtual void onUnitDestroyProjectile(CProjectile* pProjectile) {}
    inline virtual void onUnitSkillReady(CSkill* pSkill) {}
    
    M_SYNTHESIZE(CUnit*, m_pNotifyUnit, NotifyUnit);
};

class CUnit : public CMultiRefObject, public CUnitForce, public CLevelExp
{
protected:
    const string CONST_ROOT_ID;

public:
    CUnit(const char* pRootId);
    virtual ~CUnit();
    
    M_SYNTHESIZE(CWorld*, m_pWorld, World);
    
    CUnit* getUnit(int id);
    void skillCD(CSkill* pSkill);
    void updateSkillCD(int id);
    
    M_SYNTHESIZE_STR(Name);
    
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oPosition, Position);
    M_SYNTHESIZE(float, m_fHalfWidth, HalfWidth);
    M_SYNTHESIZE(float, m_fHalfHeight, HalfHeight);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oFirePoint, FirePoint);
    
    bool revive(float fHp);
    bool setHp(float fHp);
    void setMaxHp(float fMaxHp);
    bool isDead() const;
    
    M_SYNTHESIZE_READONLY(float, m_fHp, Hp);
    M_SYNTHESIZE_READONLY(float, m_fMaxHp, MaxHp);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMaxHp, ExMaxHp);
    
    // @override
    
    // 等级变化时被通知，在通过addExp升级的时候，通常来讲iChanged总是为1，尽管经验有时会足够多以至于连升2级
    virtual void onLevelChange(int iChanged);
    // 复活时被通知
    virtual void onRevive();
    // 死亡时被通知
    virtual void onDie();
    // 血量变化时被通知
    virtual void onHpChange(float fChanged);
    // 每个游戏刻被通知
    virtual void onTick(float dt);
    // 攻击发出时，攻击者被通知
    virtual CAttackData* onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask);
    // 攻击抵达时，受害者被通知
    virtual CAttackData* onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);
    // 攻击命中时，受害者被通知
    virtual void onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);
    // 攻击命中时，受害者被通知
    virtual void onDamagedDone(float fDamage, CUnit* pSource, uint32_t dwTriggerMask);
    // 攻击命中时，攻击者被通知
    virtual void onDamageTargetDone(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask);
    // 攻击数据消除时被通知，通常由投射物携带攻击数据，二者生存期一致
    virtual void onDestroyProjectile(CProjectile* pProjectile);
    // 技能CD结束时被通知
    virtual void onSkillReady(CSkill* pSkill);
    
protected:
    CUnitEventAdapter* m_pAI;
    
public:
    template <typename ADAPTER>
    void setAI();
    void delAI();
    

    ////////////////////////  trigger /////////////////    
    enum TRIGGER_FLAG_BIT
    {
        kReviveTrigger = 1 << 0,
        kDieTrigger = 1 << 1,
        kHpChangeTrigger = 1 << 2,
        kTickTrigger = 1 << 3,
        kAttackTargetTrigger = 1 << 4,
        kAttackedTrigger = 1 << 5,
        kDamagedSurfaceTrigger = 1 << 6,
        kDamagedInnerTrigger = 1 << 7,
        kDamagedDoneTrigger = 1 << 8,
        kDamageTargetDoneTrigger = 1 << 9,
        kDestroyProjectileTrigger = 1 << 10
    };
    
    enum TRIGGER_MASK
    {
        kNoMasked = 0,
        kMaskAll = 0xFFFFFFFF,
        kMaskActiveTrigger = kAttackTargetTrigger | kDamageTargetDoneTrigger
    };
    
    
    //////////////////// attack & damaged ////////////////////////
    
    // 高层攻击函数，用于最初生成攻击数据，一个攻击动作生成的攻击数据，一般调用该函数
    // 攻击动作，可对目标造成动作，如普通攻击、技能等
    // 攻击数据，描述这次攻击的数据体，详见 CAttackData 定义
    // 内部会自行调用中层、底层攻击函数，对攻击数据进行传递并处理，通常返回处理后的攻击数据，也可以返回 NULL
    // 内部会根据人物属性对攻击数据进行一次变换，如力量加成等
    CAttackData* attackAdv(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // 中层攻击函数
    // 触发 onAttackTarget，
    CAttackData* attackMid(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // 底层攻击函数，目前无逻辑，只是将传递过来的攻击数据返回给上层
    CAttackData* attackBot(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // 高层伤害函数，攻击者生成的攻击到达目标后，目标将调用该函数，计算自身伤害
    // 内部会对攻击数据进行向下传递
    // 触发 onAttacked，如果onAttacked返回 NULL，伤害将不会继续向下层函数传递，函数返回false。比如说，闪避成功，伤害无需继续计算
    bool damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    // 中层伤害函数，攻击数据已经不可消除，但可以改变伤害数据，如一次全额伤害的抵挡，虽然结果上看HP没有受损，但仍然会进行一次0伤害判定
    // 触发 onDamaged
    // 遍历攻击数据携带的BUFF链，根据附着概率对单位自身进行BUFF附加
    // 根据单位属性，进行攻击数据变换，如抗性对攻击数据的影响
    // 根据单位护甲，进行攻击数据中的攻击数值变换
    void damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    // 底层伤害函数，直接扣除指定量的HP值
    // 触发伤害源的 onDamaeTarget
    // 调用 setHp，从而会触发 onHpChange，可能会触发onDie
    void damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    float calcDamage(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue, CArmorValue::ARMOR_TYPE eArmorType, float fArmorValue);
    
    
    typedef CMultiRefMap<CActiveSkill*> MAP_ACTIVE_SKILLS;
    typedef CMultiRefMap<CPassiveSkill*> MAP_PASSIVE_SKILLS;
    typedef CMultiRefMap<CBuffSkill*> MAP_BUFF_SKILLS;
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ACTIVE_SKILLS, m_mapActSkills, ActiveSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PASSIVE_SKILLS, m_mapPasSkills, PassiveSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_BUFF_SKILLS, m_mapBuffSkills, BuffSkills);


    // 下列函数将安全的增删触发器
    
    void addActiveSkill(CActiveSkill* pSkill);
    void delActiveSkill(int id);
    CActiveSkill* getActiveSkill(int id);
    
    void addPassiveSkill(CPassiveSkill* pSkill);
    void delPassiveSkill(int id);
    CPassiveSkill* getPassiveSkill(int id);
    
    void addBuffSkill(CBuffSkill* pSkill);
    void delBuffSkill(int id);
    CBuffSkill* getBuffSkill(int id);
    
protected:
    void updateBuffSkillElapsed(float dt);
    
public:
    typedef CMultiRefMap<CSkill*> MAP_TRIGGER_SKILLS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnAttackTargetTriggerSkills, OnAttackTargetTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnAttackedTriggerSkills, OnAttackedTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnDamagedSurfaceTriggerSkills, OnDamagedSurfaceTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnDamagedInnerTriggerSkills, OnDamagedInnerTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnDamagedDoneTriggerSkills, OnDamagedDoneTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnDamageTargetDoneTriggerSkills, OnDamageTargetDoneTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnHpChangeTriggerSkills, OnHpChangeTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnReviveTriggerSkills, OnReviveTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnDieTriggerSkills, OnDieTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnTickTriggerSkills, OnTickTriggerSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapOnDestroyProjectileTriggerSkills, OnDestroyProjectileTriggerSkills);
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapTriggerSkillsToAdd, TriggerSkillsToAdd);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_SKILLS, m_mapTriggerSkillsToDel, TriggerSkillsToDel);
    
public:
    // 添加触发器
    void addSkillToTriggers(CSkill* pSkill);
    
    // 删除触发器
    void delSkillFromTriggers(CSkill* pSkill);
    
protected:
    // 只能在triggerFree的时候调用
    void updateTriggerSkillsWhenTriggerFree();
    
    // trigger之间是有可能存在嵌套关系的
    // 为了安全增删trigger，需要维护一个引用计数
    int m_iTriggerRefCount;
    void beginTrigger();
    void endTrigger();
    bool isTriggerFree() const;
    
    // 触发器链的触发，内部调用
    void triggerOnRevive();
    void triggerOnDie();
    void triggerOnHpChange(float fChanged);
    void triggerOnTick(float dt);
    CAttackData* triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    CAttackData* triggerOnAttacked(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedInner(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedDone(float fDamage, CUnit* pSource);
    void triggerOnDamageTargetDone(float fDamage, CUnit* pTarget);
    void triggerOnDestroyProjectile(CProjectile* pProjectile);
    
    // 为单位添加/删除技能
    //void addSkill(CSkill* pSkill);
    //void delSkill(CSkill* pSkill);
    
    // 为单位添加/删除/覆盖删除BUFF
    //void addBuff(CBuffSkill* pBuff, bool bForce = false);
    //void delBuff(CBuffSkill* pBuff, bool bAfterTriggerLoop = true);
    //void coverBuff(CBuffSkill* pBuff);
    
protected:
    int m_iSuspendRef;
    
public:
    virtual bool isSuspended() const;
    virtual void suspend();
    virtual void resume();
    
    M_SYNTHESIZE(int, m_iAttackSkillId, AttackSkillId);
    
    M_SYNTHESIZE_PASS_BY_REF(CCommandTarget, m_oCastTarget, CastTarget);
    virtual int cast(int iActiveSkillId);  // 可能是施法失败，施法中，施法追逐中，所以返回类型为int
    
    M_SYNTHESIZE(CArmorValue::ARMOR_TYPE, m_eArmorType, ArmorType);
    M_SYNTHESIZE(float, m_fBaseArmorValue, BaseArmorValue);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExArmorValue, ExArmorValue)
    float getRealArmorValue() const;
    
    M_SYNTHESIZE_BOOL(Revivable);
    
    /////////////////////// doing - begin ////////////////////////////
    enum DOING_FLAG_BIT
    {
        kSuspended = 1 << 16,
        kMoving = 1 << 17,
        kIntended = 1 << 18,
        kAttacking = 1 << 19,
        kCasting = 1 << 20,
        kSpinning = 1 << 21
    };
    
    M_SYNTHESIZE_READONLY(uint32_t, m_dwDoingFlags, DoingFlags);
    virtual void startDoing(uint32_t dwFlags);
    virtual void endDoing(uint32_t dwFlags);
    virtual bool isDoingOr(uint32_t dwFlags) const;
    virtual bool isDoingAnd(uint32_t dwFlags) const;
    virtual bool isDoingNothing() const;
    
    /////////////////////// move - begin //////////////////////////////
    struct UNIT_MOVE_PARAMS
    {
        UNIT_MOVE_PARAMS(
                         bool bIntended_ = true,
                         bool bCancelAttack_ = true,
                         bool bAutoFlipX_ = true,
                         float fMaxOffsetY_ = 0.0f,
                         bool bCancelCast_ = true
                         )
        : bIntended(bIntended_)
        , bCancelAttack(bCancelAttack_)
        , bAutoFlipX(bAutoFlipX_)
        , fMaxOffsetY(fMaxOffsetY_)
        , bCancelCast(bCancelCast_)
        {}
        bool bIntended;
        bool bCancelAttack;
        bool bAutoFlipX;
        float fMaxOffsetY;
        bool bCancelCast;
    };
    
    static const UNIT_MOVE_PARAMS CONST_DEFAULT_MOVE_PARAMS;
    static const float CONST_MIN_MOVE_SPEED;
    static const float CONST_MAX_MOVE_SPEED;
    static const float CONST_MIN_MOVE_SPEED_MULRIPLE;
    
    M_SYNTHESIZE_READONLY(float, m_fBaseMoveSpeed, BaseMoveSpeed);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeed, ExMoveSpeed);
    M_SYNTHESIZE_BOOL(Fixed);
    M_SYNTHESIZE_BOOL(Flip); // default: false, toward right
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oMoveTarget, MoveTarget)
    
	void setBaseMoveSpeed(float fMoveSpeed);
    void setExMoveSpeed(const CExtraCoeff& roExMoveSpeed);
    
    float getRealMoveSpeed() const;
    
	virtual void updateMoveSpeedDelta();
    
    virtual void move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    virtual void follow(int iTargetUnit, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    //virtual void moveAlongPath(CUnitPath* pPath, bool bIntended = true, bool bRestart = false, float fBufArrive = 5.0);
    virtual void stopMove();
    virtual void onMoveEnd(CUnit* pUnit, void* pData);
    
    virtual void startMoveAct(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams = CONST_DEFAULT_MOVE_PARAMS);
    virtual void stopMoveAct();
    
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oLastMoveToTarget, LastMoveToTarget);
    //M_SYNTHESIZE(uint32_t, m_dwPathCurPos, PathCurPos);
    //virtual void setPathIntended(bool bPathIntended = true);
    //virtual bool isPathIntended() const;
    // --------------- Action ----------------
protected:
    CActionManager m_oActMgr;
    
public:
    enum ACTION_TAG
    {
        kActMoveTo,
        kActMove,
        kActAttack,
        kActAttackEffect,
        kActDie,
        kActCast,
        kActCastEffect,
        kActSpin
    };
    
    void runAction(CAction* pAction);
    CAction* getActionByTag(int iTag);
    
    enum UNIT_SPECIES
    {
        kUnknown = 0,
        kHuman,
        kBeast,
        kMechanical,
        kGod,
        kDemon
    };
    
    M_SYNTHESIZE(UNIT_SPECIES, m_eSpecies, Species);
    
};

class CProjectile : public CUnit
{
public:
    CProjectile(const char* pRootId);
    virtual ~CProjectile();

    // 单位和抛射物非紧密联系，即单位死亡后抛射物不一定会释放，所以必须通过ID引用
    M_SYNTHESIZE(int, m_iSourceUnit, SourceUnit);
    M_SYNTHESIZE(int, m_iStartUnit, StartUnit);
    M_SYNTHESIZE(int, m_iTargetUnit, TargetUnit);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oTargetPoint, TargetPoint);
    
    enum PENALTY_FLAG_BIT
    {
        kOnDie = 1 << 0,
        kOnContact = 1 << 1
    };
    
    M_SYNTHESIZE(uint32_t, m_dwPenaltyFlags, PenaltyFlags);
    bool hasPenaltyType(PENALTY_FLAG_BIT ePenaltyType) const;
    
    
protected:

};




class CWorld : public CMultiRefObject
{
public:
    CWorld();
    virtual ~CWorld();
    
    typedef CMultiRefMap<CUnit*> MAP_UNITS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnits, Units);
    void addUnit(CUnit* pUnit);
    void delUnit(int id);
    CUnit* getUnit(int id) const;
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnitsToRevive, UnitsToRevive);
    void reviveUnit(int id, float fHp);
    
    typedef CMultiRefMap<CSkill*> MAP_SKILLS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_SKILLS, m_mapSkillsCD, SkillsCD);
    void addSkillCD(CSkill* pSkill);
    bool isSkillCD(int id) const;
    CSkill* getSkillCD(int id) const;
    void updateSkillCD(int id);
    
protected:
    void cleanSkillsCD(CUnit* pUnit);
    void skillReady(CSkill* pSkill);
    
public:
    virtual void onTick(float dt);
        
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_SKILLS, m_mapTemplateSkills, TemplateSkills);
    int addTemplateSkill(CSkill* pSkill);
    void loadTemplateSkills();
    
    CSkill* copySkill(int id) const;

};





// ----------- Inline Implementation--------------
// CUnit
template <typename ADAPTER>
inline void CUnit::setAI()
{
    ADAPTER* pAdapter = new ADAPTER;
    CUnitEventAdapter* pAI = dynamic_cast<CUnitEventAdapter*>(pAdapter);
    if (pAI == NULL)
    {
        delete pAdapter;
        return;
    }
    
    if (m_pAI != NULL)
    {
        delete m_pAI;
    }
    
    m_pAI = pAI;
    m_pAI->setNotifyUnit(this);
}

#endif	/* __UNIT_H__ */

