/* 
 * File:   Unit.h
 * Author: thunderliu
 *
 * Created on 2013��12��8��, ����10:55
 */

#ifndef __UNIT_H__
#define __UNIT_H__

#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"
#include "CommDef.h"
// ��ֹ�ڴ˴�����Unit.h�ļ�


class CTypeValue
{
public:
    CTypeValue(int type, float value);
    M_SYNTHESIZE(int, m_iType, Type);
    M_SYNTHESIZE(float, m_fValue, Value);
    
    void set(int type, float value);
};

class CAttackValue : public CTypeValue
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
    
    CAttackValue(ATTACK_TYPE type = kPhysical, float value = 0.0f);
};

class CArmorValue : public CTypeValue
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
    
    CArmorValue(ARMOR_TYPE type = kNormal, float value = 0.0f);
};


// ������ֵ���ɶ������͵Ĺ�����϶���
class CMultiAttackValue
{
public:
    static const char* CONST_ARR_NAME[CAttackValue::CONST_MAX_ATTACK_TYPE][CAttackValue::CONST_MAX_NAME_INDEX];
    typedef float ARR_ATTACK_VALUES[CAttackValue::CONST_MAX_ATTACK_TYPE];
    
public:
    CMultiAttackValue();
    CMultiAttackValue(int iCount, CAttackValue::ATTACK_TYPE eType1, float fValue1, ...);
    
    float getValue(CAttackValue::ATTACK_TYPE eType) const;
    void setValue(CAttackValue::ATTACK_TYPE eType, float fValue);
    void setAttackValue(const CMultiAttackValue& roAttackValue);
    void setValueZero();
    
    static const char* getName(CAttackValue::ATTACK_TYPE eType, int iIndex = 0);
    
    const CMultiAttackValue& operator=(const CMultiAttackValue& roAttackValue);
    
    ARR_ATTACK_VALUES m_afValues;
};

// ������ֵ���ɶ������͵Ļ�����϶���
class CMultiArmorValue
{
public:
    static const char* CONST_ARR_NAME[CArmorValue::CONST_MAX_ARMOR_TYPE][CArmorValue::CONST_MAX_NAME_INDEX];
    typedef float ARR_ARMOR_VALUES[CArmorValue::CONST_MAX_ARMOR_TYPE];
    
public:
    CMultiArmorValue();
    CMultiArmorValue(int iCount, CArmorValue::ARMOR_TYPE eType1, float fValue1, ...);
    
    float getValue(CArmorValue::ARMOR_TYPE eType) const;
    void setValue(CArmorValue::ARMOR_TYPE eType, float fValue);
    void setArmorValue(const CMultiArmorValue& roArmorValue);
    void setValueZero();
    
    static const char* getName(CArmorValue::ARMOR_TYPE eType, int iIndex = 0);
    
    const CMultiArmorValue& operator=(const CMultiArmorValue& roArmorValue);
    
    ARR_ARMOR_VALUES m_afValues;
};

// ����-���׼���ϵ��
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

// f = ax + b��xΪ��׼ֵ��a��b��Ϊ��������ͼ�����Ӧ�����Σ������ػ����������������2�������⸽��30����˺�����������20%�Ļ�������ֵ
// Ӧ�øýӽṹ�������׽��װ������/����BUFF��ж������/ɾ��BUFF�������һ��
class CExtraCoeff
{
public:
    CExtraCoeff(float fMulriple = 1.0f, float fAddend = 0.0f);
    
    float getValue(float fBase) const;
    
    M_SYNTHESIZE(float, m_fMulriple, Mulriple);
    M_SYNTHESIZE(float, m_fAddend, Addend);
};

class CUnitForce
{
public:
    enum EFFECTIVE_TARGET_FLAG_BIT
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
    bool isEffective(const CUnitForce* pForce, uint32_t dwEffectiveTypeFlags) const;
    
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
class CAbility;
class CPassiveAbility;
class CBuffAbility;
class CActiveAbility;
class CWorld;
class CItem;

class CCommandTarget
{
public:
    enum TARGET_TYPE
    {
        kNoTarget = 0,
        kUnitTarget,
        kPointTarget
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
    
    inline virtual void onUnitChangeLevel(int iChanged) {}
    inline virtual void onUnitRevive() {}
    inline virtual void onUnitDying() {}
    inline virtual void onUnitDead() {}
    inline virtual void onUnitChangeHp(float fChanged) {}
    inline virtual void onUnitTick(float dt) {}
    inline virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget) { return pAttack; }
    inline virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource) { return pAttack; }
    inline virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource) {}
    inline virtual void onUnitDamagedDone(float fDamage, CUnit* pSource) {}
    inline virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget) {}
    inline virtual void onUnitProjectileEffect(CProjectile* pProjectile) {}
    inline virtual void onUnitAddActiveAbility(CActiveAbility* pAbility) {}
    inline virtual void onUnitDelActiveAbility(CActiveAbility* pAbility) {}
    inline virtual void onUnitAddPassiveAbility(CPassiveAbility* pAbility) {}
    inline virtual void onUnitDelPassiveAbility(CPassiveAbility* pAbility) {}
    inline virtual void onUnitAddBuffAbility(CBuffAbility* pAbility) {}
    inline virtual void onUnitDelBuffAbility(CBuffAbility* pAbility) {}
    inline virtual void onUnitAbilityReady(CAbility* pAbility) {}
    inline virtual void onUnitAddItem(int iIndex) {}
    inline virtual void onUnitDelItem(int iIndex) {}
    //inline virtual void onUnitChangeItemStackCount(CItem* pItem, int iChange) {}
    
    M_SYNTHESIZE(CUnit*, m_pNotifyUnit, NotifyUnit);
};

class CDefaultAI : public CUnitEventAdapter
{
public:
    virtual void onUnitTick(float dt);
    virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource);
};

class CUnitDraw;

class CUnit : public CMultiRefObject, public CUnitForce, public CLevelExp
{
protected:
    const string CONST_ROOT_ID;

public:
    CUnit(const char* pRootId);
    virtual ~CUnit();

    virtual const char* getDbgTag() const;

    M_SYNTHESIZE(CWorld*, m_pWorld, World);
    
    CUnit* getUnit(int id);
    void abilityCD(CAbility* pAbility);
    void updateAbilityCD(int id);
    
    M_SYNTHESIZE_STR(Name);
    
    bool revive(float fHp);
    bool setHp(float fHp);
    void setMaxHp(float fMaxHp);
    bool isDead() const;
    
    M_SYNTHESIZE_READONLY(float, m_fHp, Hp);
    M_SYNTHESIZE_READONLY(float, m_fMaxHp, MaxHp);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMaxHp, ExMaxHp);
    
    // @override
    
    // �ȼ��仯ʱ��֪ͨ����ͨ��addExp������ʱ��ͨ������iChanged����Ϊ1�����ܾ�����ʱ���㹻������������2��
    virtual void onChangeLevel(int iChanged);
    // ����ʱ��֪ͨ
    virtual void onRevive();
    // ����ʱ��֪ͨ
    virtual void onDying();
    // ������֪ͨ
    virtual void onDead();
    // Ѫ���仯ʱ��֪ͨ
    virtual void onChangeHp(float fChanged);
    // ÿ����Ϸ�̱�֪ͨ
    virtual void step(float dt);
    virtual void onTick(float dt);
    // ��������ʱ�������߱�֪ͨ
    virtual CAttackData* onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask);
    // �����ִ�ʱ���ܺ��߱�֪ͨ
    virtual CAttackData* onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);
    // ��������ʱ���ܺ��߱�֪ͨ
    virtual void onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);
    // ��������ʱ���ܺ��߱�֪ͨ
    virtual void onDamagedDone(float fDamage, CUnit* pSource, uint32_t dwTriggerMask);
    // ��������ʱ�������߱�֪ͨ
    virtual void onDamageTargetDone(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask);
    // ������������ʱ��֪ͨ��ͨ����Ͷ����Я���������ݣ�����������һ��
    virtual void onProjectileEffect(CProjectile* pProjectile);
    
    virtual void onAddActiveAbility(CActiveAbility* pAbility);
    virtual void onDelActiveAbility(CActiveAbility* pAbility);
    virtual void onAddPassiveAbility(CPassiveAbility* pAbility);
    virtual void onDelPassiveAbility(CPassiveAbility* pAbility);
    virtual void onAddBuffAbility(CBuffAbility* pAbility);
    virtual void onDelBuffAbility(CBuffAbility* pAbility);
    
    // ����CD����ʱ��֪ͨ
    virtual void onAbilityReady(CAbility* pAbility);  // �Ժ����ֳ�onItemReady
    
    virtual void onAddItem(int iIndex);
    virtual void onDelItem(int iIndex);
    //virtual void onChangeItemStackCount(CItem* pItem, int iChange);
protected:
    CUnitEventAdapter* m_pAI;
    
public:
    template <typename ADAPTER>
    void setAI(const ADAPTER&);
    void delAI();
    

    ////////////////////////  trigger /////////////////    
    enum TRIGGER_FLAG_BIT
    {
        kOnReviveTrigger = 1 << 0,
        kOnDyingTrigger = 1 << 1,
        kOnDeadTrigger = 1 << 2,
        kOnChangeHpTrigger = 1 << 3,
        kOnTickTrigger = 1 << 4,
        kOnAttackTargetTrigger = 1 << 5,
        kOnAttackedTrigger = 1 << 6,
        kOnDamagedSurfaceTrigger = 1 << 7,
        kOnDamagedInnerTrigger = 1 << 8,
        kOnDamagedDoneTrigger = 1 << 9,
        kOnDamageTargetDoneTrigger = 1 << 10,
        kOnProjectileEffectTrigger = 1 << 11
    };
    
    enum TRIGGER_MASK
    {
        kNoMasked = 0,
        kMaskAll = 0xFFFFFFFF,
        kMaskActiveTrigger = kOnAttackTargetTrigger | kOnDamageTargetDoneTrigger
    };
    
    
    //////////////////// attack & damaged ////////////////////////
    
    // �߲㹥������������������ɹ������ݣ�һ�������������ɵĹ������ݣ�һ����øú���
    // �����������ɶ�Ŀ����ɶ���������ͨ���������ܵ�
    // �������ݣ�������ι����������壬��� CAttackData ����
    // �ڲ������е����в㡢�ײ㹥���������Թ������ݽ��д��ݲ�����ͨ�����ش����Ĺ������ݣ�Ҳ���Է��� NULL
    // �ڲ�������������ԶԹ������ݽ���һ�α任���������ӳɵ�
    CAttackData* attackAdv(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // �в㹥������
    // ���� onAttackTarget��
    CAttackData* attackMid(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // �ײ㹥��������Ŀǰ���߼���ֻ�ǽ����ݹ����Ĺ������ݷ��ظ��ϲ�
    CAttackData* attackBot(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // �߲��˺����������������ɵĹ�������Ŀ���Ŀ�꽫���øú��������������˺�
    // �ڲ���Թ������ݽ������´���
    // ���� onAttacked�����onAttacked���� NULL���˺�������������²㺯�����ݣ���������false������˵�����ܳɹ����˺������������
    bool damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    // �в��˺����������������Ѿ����������������Ըı��˺����ݣ���һ��ȫ���˺��ĵֵ�����Ȼ����Ͽ�HPû�����𣬵���Ȼ�����һ��0�˺��ж�
    // ���� onDamaged
    // ������������Я����BUFF�������ݸ��Ÿ��ʶԵ�λ�������BUFF����
    // ���ݵ�λ���ԣ����й������ݱ任���翹�ԶԹ������ݵ�Ӱ��
    // ���ݵ�λ���ף����й��������еĹ�����ֵ�任
    void damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    // �ײ��˺�������ֱ�ӿ۳�ָ������HPֵ
    // �����˺�Դ�� onDamaeTarget
    // ���� setHp���Ӷ��ᴥ�� onChangeHp�����ܻᴥ��onDying
    void damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    float calcDamage(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue, CArmorValue::ARMOR_TYPE eArmorType, float fArmorValue);
    
    
    typedef CMultiRefMap<CActiveAbility*> MAP_ACTIVE_ABILITYS;
    typedef CMultiRefMap<CPassiveAbility*> MAP_PASSIVE_ABILITYS;
    typedef CMultiRefMap<CBuffAbility*> MAP_BUFF_ABILITYS;
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ACTIVE_ABILITYS, m_mapActAbilitys, ActiveAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PASSIVE_ABILITYS, m_mapPasAbilitys, PassiveAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_BUFF_ABILITYS, m_mapBuffAbilitys, BuffAbilitys);


    M_SYNTHESIZE(int, m_iAttackAbilityId, AttackAbilityId);
    // ���к�������ȫ����ɾ������
    
    void addActiveAbility(CActiveAbility* pAbility, bool bNotify = true);
    void addActiveAbility(int id, int iLevel = 1);
    void delActiveAbility(int id, bool bNotify = true);
    CActiveAbility* getActiveAbility(int id);
    
    void addPassiveAbility(CPassiveAbility* pAbility, bool bNotify = true);
    void addPassiveAbility(int id, int iLevel = 1);
    void delPassiveAbility(int id, bool bNotify = true);
    CPassiveAbility* getPassiveAbility(int id);
    
    void addBuffAbility(CBuffAbility* pAbility, bool bNotify = true);
    void addBuffAbility(int id, int iSrcUnit, int iLevel = 1);
    void delBuffAbility(int id, bool bNotify = true);
    CBuffAbility* getBuffAbility(int id);
    
protected:
    void updateBuffAbilityElapsed(float dt);
    
public:
    typedef CMultiRefMap<CAbility*> MAP_TRIGGER_ABILITYS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnAttackTargetTriggerAbilitys, OnAttackTargetTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnAttackedTriggerAbilitys, OnAttackedTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnDamagedSurfaceTriggerAbilitys, OnDamagedSurfaceTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnDamagedInnerTriggerAbilitys, OnDamagedInnerTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnDamagedDoneTriggerAbilitys, OnDamagedDoneTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnDamageTargetDoneTriggerAbilitys, OnDamageTargetDoneTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnChangeHpTriggerAbilitys, OnChangeHpTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnReviveTriggerAbilitys, OnReviveTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnDyingTriggerAbilitys, OnDyingTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnDeadTriggerAbilitys, OnDeadTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnTickTriggerAbilitys, OnTickTriggerAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapOnProjectileEffectTriggerAbilitys, OnProjectileEffectTriggerAbilitys);
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapTriggerAbilitysToAdd, TriggerAbilitysToAdd);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITYS, m_mapTriggerAbilitysToDel, TriggerAbilitysToDel);
    
public:
    // ��Ӵ�����
    void addAbilityToTriggers(CAbility* pAbility);
    
    // ɾ��������
    void delAbilityFromTriggers(CAbility* pAbility);
    
protected:
    // ֻ����triggerFree��ʱ�����
    void updateTriggerAbilitysWhenTriggerFree();
    
    // trigger֮�����п��ܴ���Ƕ�׹�ϵ��
    // Ϊ�˰�ȫ��ɾtrigger����Ҫά��һ�����ü���
    int m_iTriggerRefCount;
    void beginTrigger();
    void endTrigger();
    bool isTriggerFree() const;
    
    // ���������Ĵ������ڲ�����
    void triggerOnRevive();
    void triggerOnDying();
    void triggerOnDead();
    void triggerOnChangeHp(float fChanged);
    void triggerOnTick(float dt);
    CAttackData* triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    CAttackData* triggerOnAttacked(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedInner(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedDone(float fDamage, CUnit* pSource);
    void triggerOnDamageTargetDone(float fDamage, CUnit* pTarget);
    void triggerOnProjectileEffect(CProjectile* pProjectile);
    
    // Ϊ��λ���/ɾ������
    //void addAbility(CAbility* pAbility);
    //void delAbility(CAbility* pAbility);
    
    // Ϊ��λ���/ɾ��/����ɾ��BUFF
    //void addBuff(CBuffAbility* pBuff, bool bForce = false);
    //void delBuff(CBuffAbility* pBuff, bool bAfterTriggerLoop = true);
    //void coverBuff(CBuffAbility* pBuff);
        
public:
    M_SYNTHESIZE_READONLY(int, m_iSuspendRef, SuspendRef);
    virtual bool isSuspended() const;
    virtual void suspend();
    virtual void resume();
    
    M_SYNTHESIZE_PASS_BY_REF(CArmorValue, m_oBaseArmor, BaseArmor);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExArmorValue, ExArmorValue)
    float getRealArmorValue() const;
    
    M_SYNTHESIZE_BOOL(Revivable);
    
    ///////////////////////// item //////////////////////
    typedef CMultiRefVec<CItem*> VEC_ITEMS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ITEMS, m_vecItems, Items);
    void setPackageSize(int iSize);
    bool addItem(CItem* pItem);
    void delItem(int iIndex);
    CItem* getItem(int iIndex);
    
    virtual int useItem(int iIndex);
    /////////////////////// doing - begin ////////////////////////////
    enum DOING_FLAG_BIT
    {
        kDying = 1 << 16,
        kMoving = 1 << 17,
        kObstinate = 1 << 18,
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
    
    // --------------- Action ----------------

    M_SYNTHESIZE_READONLY(CUnitDraw*, m_pDraw, Draw);
    virtual void setDraw(CUnitDraw* pDraw);

protected:
    CActionManager m_oActMgr;
    
public:
    enum ACTION_TAG
    {
        kActMoveTo,
        kActMove,
        kActAttack,
        kActAttackEffect,
        kActDying,
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

class CWorld : public CMultiRefObject
{
public:
    CWorld();
    virtual ~CWorld();
    
    M_SYNTHESIZE(int, m_iScriptHandler, ScriptHandler);
    static lua_State* getLuaHandle();
    void addScriptSearchPath(const char* pPath);
    bool loadScript(const char* pName);

    virtual bool onInit();
    virtual void onTick(float dt);
    virtual void onAddUnit(CUnit* pUnit);
    virtual void onDelUnit(CUnit* pUnit);
    virtual void onAddProjectile(CProjectile* pProjectile);
    virtual void onDelProjectile(CProjectile* pProjectile);

    bool init();

    M_SYNTHESIZE(int, m_iControlUnit, ControlUnit);
    
    typedef CMultiRefMap<CUnit*> MAP_UNITS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnits, Units);
    void addUnit(CUnit* pUnit);
    void delUnit(int id, bool bRevivable = false);
    CUnit* getUnit(int id) const;
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnitsToRevive, UnitsToRevive);
    void reviveUnit(int id, float fHp);
    CUnit* getUnitToRevive(int id);
    
    typedef CMultiRefMap<CAbility*> MAP_ABILITYS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ABILITYS, m_mapAbilitysCD, AbilitysCD);
    void addAbilityCD(CAbility* pAbility);
    void delAbilityCD(int id);
    bool isAbilityCD(int id) const;
    CAbility* getAbilityCD(int id) const;
    void updateAbilityCD(int id);
    
protected:
    void cleanAbilitysCD(CUnit* pUnit);
    void abilityReady(CAbility* pAbility);
    
public:
    virtual void step(float dt);

    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ABILITYS, m_mapTemplateAbilitys, TemplateAbilitys);
    int addTemplateAbility(CAbility* pAbility);
    void loadTemplateAbilitys();
    CAbility* copyAbility(int id) const;

    typedef CMultiRefMap<CProjectile*> MAP_PROJECTILES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PROJECTILES, m_mapTemplateProjectiles, TemplateProjectiles);
    int addTemplateProjectile(CProjectile* pProjectile);
    CProjectile* copyProjectile(int id) const;

    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PROJECTILES, m_mapProjectiles, Projectiles);
    void addProjectile(CProjectile* pProjectile);
    void delProjectile(int id);

};



// ----------- Inline Implementation--------------

// CUnit
template <typename ADAPTER>
inline void CUnit::setAI(const ADAPTER&)
{
    ADAPTER* pAdapter = new ADAPTER;
    CUnitEventAdapter* pAI = DCAST(pAdapter, CUnitEventAdapter*);
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

