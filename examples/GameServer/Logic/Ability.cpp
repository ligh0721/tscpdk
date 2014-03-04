/* 
 * File:   Ability.cpp
 * Author: thunderliu
 * 
 * Created on 2013��12��8��, ����11:45
 */

#include "CommInc.h"
#include "Unit.h"
#include "Ability.h"
#include "MultiRefObject.h"
#include "Application.h"
#include "Draw.h"
#include "LuaBinding.h"


#ifdef DEBUG_FOR_CC
// for cocos2d
#include "../CommHeader.h"
#include "../DrawForCC.h"
#endif


// CAbility
CAbility::CAbility(const char* pRootId, const char* pName, float fCoolDown)
: CONST_ROOT_ID(pRootId)
, m_iScriptHandler(0)
, m_sName(pName)
, m_pOwner(NULL)
, m_fCoolDown(fCoolDown)
, m_fCoolingDownElapsed(FLT_MAX)
, m_fInterval(0.0f)
, m_fIntervalElapsed(0.0f)
, m_dwTriggerFlags(0)
{
    setDbgClassName("CAbility");
}

CAbility::~CAbility()
{
    if (getScriptHandler() != 0)
    {
        lua_State* L = CWorld::getLuaHandle();
        luaL_unref(L, LUA_REGISTRYINDEX, getScriptHandler());
    }
}

const char* CAbility::getDbgTag() const
{
    return getName();
}

const char* CAbility::getRootId() const
{
    return CONST_ROOT_ID.c_str();
}

bool CAbility::isCoolingDown() const
{
    return m_fCoolingDownElapsed < getCoolDown();
}

void CAbility::resetCD()
{
    m_fCoolingDownElapsed = FLT_MAX;
    getOwner()->updateAbilityCD(getId());
}

void CAbility::coolDown()
{
    setCoolingDownElapsed(0.0f);
    getOwner()->abilityCD(this);
}

void CAbility::setInterval(float fInterval)
{
    if (fInterval <= FLT_EPSILON)
    {
        m_fInterval = 0.0f;
        return;
    }
    
    setTriggerFlags(CUnit::kOnTickTrigger);
    m_fInterval = fInterval;
}

void CAbility::onUnitAddAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAddAbility");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitDelAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());
    
    lua_getfield(L, a, "onUnitDelAbility");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitAbilityReady()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAbilityReady");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitRevive()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitRevive");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitDying()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDying");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitDead()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDead");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitChangeHp(float fChanged)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitChangeHp");
    luaL_getcopy(L, a);
    lua_pushnumber(L, fChanged);
    lua_call(L, 2, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitTick(float dt)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitTick");
    luaL_getcopy(L, a);
    lua_pushnumber(L, dt);
    lua_call(L, 2, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitInterval()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitInterval");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);
}

CAttackData* CAbility::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return pAttack;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAttackTarget");
    luaL_getcopy(L, a);
    lua_pushnil(L);
    lua_pushnil(L);
    lua_call(L, 3, 1);
    // pAttack = 
    lua_pop(L, 1);

    return pAttack;
}

CAttackData* CAbility::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return pAttack;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAttacked");
    luaL_getcopy(L, a);
    lua_pushnil(L);
    lua_pushnil(L);
    lua_call(L, 3, 1);
    // pAttack = 
    lua_pop(L, 1);

    return pAttack;
}

void CAbility::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamaged");
    luaL_getcopy(L, a);
    lua_pushnil(L);
    lua_pushnil(L);
    lua_call(L, 3, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitDamagedDone(float fDamage, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamagedDone");
    luaL_getcopy(L, a);
    lua_pushnil(L);
    lua_pushnil(L);
    lua_call(L, 3, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamageTargetDone");
    luaL_getcopy(L, a);
    lua_pushnil(L);
    lua_pushnil(L);
    lua_call(L, 3, 0);
    
    lua_pop(L, 1);
}

void CAbility::onUnitProjectileEffect(CProjectile* pProjectile)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitProjectileEffect");
    luaL_getcopy(L, a);
    lua_pushnil(L);
    lua_call(L, 2, 0);
    
    lua_pop(L, 1);
}

void CAbility::onAddToUnit(CUnit* pOwner)
{
    setOwner(pOwner);
    onUnitAddAbility();
}

void CAbility::onDelFromUnit()
{
    onUnitDelAbility();
    setOwner(NULL);
}

void CAbility::setTriggerFlags(uint32_t dwTriggerFlags)
{
    m_dwTriggerFlags |= dwTriggerFlags;
}

void CAbility::unsetTriggerFlags(uint32_t dwTriggerFlags)
{
    m_dwTriggerFlags &= ~dwTriggerFlags;
}

// CActiveAbility
CActiveAbility::CActiveAbility(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags)
: CAbility(pRootId, pName, fCoolDown)
, m_eCastTargetType(eCastType)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
, m_fCastMinRange(0.0f)
, m_fCastRange(0.0f)
, m_fCastTargetRadius(0.0f)
, m_iTemplateProjectile(0)
, m_bCastHorizontal(false)
{
    setDbgClassName("CActiveAbility");
}

CActiveAbility::~CActiveAbility()
{
}

const float CActiveAbility::CONST_MAX_CAST_BUFFER_RANGE = 50.0f;
const float CActiveAbility::CONST_MAX_HOR_CAST_Y_RANGE = 5.0f;

bool CActiveAbility::cast()
{
    if (isCoolingDown() == true)
    {
        return false;
    }

    if (checkConditions() == false)
    {
        return false;
    }

    CUnit* o = getOwner();
    LOG("%s%s%s..", o->getName(), o->getAttackAbilityId() == getId() ? "��" : "ʩ����", getName());
    coolDown();
    onUnitCastAbility();  // onCastAbility��cd�仯���棬���Կ����������cd���߼�
    
    return true;
}

bool CActiveAbility::checkConditions()
{
    if (getScriptHandler() == 0)
    {
        return true;
    }

    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "checkConditions");
    luaL_getcopy(L, a);
    lua_call(L, 1, 1);
    bool res = lua_toboolean(L, -1) != 0;
    lua_pop(L, 1);

    return res;
}

void CActiveAbility::onUnitCastAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitCastAbility");
    luaL_getcopy(L, a);
    lua_call(L, 1, 0);

    lua_pop(L, 1);
}

void CActiveAbility::fireProjectile(CAttackData* pAttackData)
{
    assert(getTemplateProjectile() != 0);

    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    CWorld* w = o->getWorld();
    CProjectile* p = NULL;
    w->copyProjectile(getTemplateProjectile())->dcast(p);
    w->addProjectile(p);
    p->setSourceUnit(o->getId());
    p->setAttackData(pAttackData);

    if (getCastTargetType() == CCommandTarget::kUnitTarget)
    {
        CUnit* t = w->getUnit(d->getCastTarget().getTargetUnit());
        CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
        assert(td != NULL);

        p->setFromToType(CProjectile::kUnitToUnit);
        p->setFromUnit(o->getId());
        p->setToUnit(t->getId());

        p->fire();
    }
}

void CActiveAbility::addCastAnimation( int id )
{
    m_vecCastAnis.push_back(id);
}

int CActiveAbility::getCastRandomAnimation() const
{
    if (m_vecCastAnis.empty())
    {
        return -1;
    }

    return m_vecCastAnis[rand() % m_vecCastAnis.size()];
}

// CPassiveAbility
CPassiveAbility::CPassiveAbility(const char* pRootId, const char* pName, float fCoolDown)
: CAbility(pRootId, pName, fCoolDown)
{
    setDbgClassName("CPassiveAbility");
}

CPassiveAbility::~CPassiveAbility()
{
}

// CBuffAbility
CBuffAbility::CBuffAbility(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CAbility(pRootId, pName, 0.0f)
, m_fDuration(fDuration)
, m_fElapsed(0.0f)
, m_bStackable(bStackable)
, m_iSrcUnit(0)
{
    setDbgClassName("CBuffAbility");
}

CBuffAbility::~CBuffAbility()
{
}

bool CBuffAbility::isDone() const
{
    return m_fElapsed >= m_fDuration;
}

// CAttackAct
const float CAttackAct::CONST_MIN_ATTACK_SPEED_INTERVAL = 0.1f; // 0.1s
const float CAttackAct::CONST_MIN_ATTACK_SPEED_MULRIPLE = 0.2f; // 20%
const float CAttackAct::CONST_MAX_ATTACK_SPEED_MULRIPLE = 5.0f; // 500%

CAttackAct::CAttackAct(const char* pRootId, const char* pName, float fCoolDown, const CAttackValue& rAttackValue, float fAttackValueRandomRange)
: CActiveAbility(pRootId, pName, fCoolDown, CCommandTarget::kUnitTarget, CUnitForce::kEnemy)
, m_oAttackValue(rAttackValue)
, m_fAttackValueRandomRange(fAttackValueRandomRange)
{
    setDbgClassName("CAttackAct");
    setTriggerFlags(CUnit::kOnProjectileEffectTrigger);
}

CMultiRefObject* CAttackAct::copy() const
{
    CAttackAct* pRet = new CAttackAct(getRootId(), getName(), m_fCoolDown, m_oAttackValue, m_fAttackValueRandomRange);
    pRet->setCastTargetType(getCastTargetType());
    pRet->setEffectiveTypeFlags(getEffectiveTypeFlags());
    pRet->setCastMinRange(getCastMinRange());
    pRet->setCastRange(getCastRange());
    pRet->setCastTargetRadius(getCastTargetRadius());
    pRet->setTemplateProjectile(getTemplateProjectile());
    pRet->setCastHorizontal(isCastHorizontal());
    pRet->m_vecCastAnis = m_vecCastAnis;
    return pRet;
}

void CAttackAct::onUnitAddAbility()
{
    getOwner()->setAttackAbilityId(getId());
}

void CAttackAct::onUnitDelAbility()
{
    getOwner()->setAttackAbilityId(0);
}

bool CAttackAct::checkConditions()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != NULL);

    CUnit* t = o->getUnit(d->getCastTarget().getTargetUnit());
    if (t == NULL || t->isDead())
    {
        return false;
    }
    
    return true;
}

void CAttackAct::onUnitCastAbility()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(getCastTargetType() == CCommandTarget::kUnitTarget);
    CUnit* t = o->getUnit(d->getCastTarget().getTargetUnit());
    
    if (t == NULL || t->isDead())
    {
        return;
    }
    
    CAttackData* ad = new CAttackData();
    ad->setAttackValue((CAttackValue::ATTACK_TYPE)getBaseAttack().getType(), getRealAttackValue());
    
    ad = o->attackAdv(ad, t);
    if (ad == NULL)
    {
        return;
    }

#if 1
    if (getTemplateProjectile() == 0)
    {
        t->damagedAdv(ad, o);
    }
    else
    {
        fireProjectile(ad);
    }
#else
    ad->retain();
    TEST_ATTACK_INFO* pAi = new TEST_ATTACK_INFO;
    pAi->iTarget = t->getId();
    pAi->pAttackData = ad;
    o->runAction(new CCallFunc(this, (FUNC_CALLFUNC_ND)&CAttackAct::onTestAttackEffect, pAi));
#endif
}

float CAttackAct::getBaseAttackValue() const
{
    return m_oAttackValue.getValue();
}

float CAttackAct::getRealAttackValue(bool bUseRandomRange) const
{
    if (bUseRandomRange)
    {
        float fAttackValueRandomRange = m_oExAttackValueRandomRange.getValue(m_fAttackValueRandomRange);
        if (fAttackValueRandomRange > 0.001)
        {
            return m_aoExAttackValue.getValue(m_oAttackValue.getValue()) * (1 - fAttackValueRandomRange * 0.5 + (rand() % (int)(fAttackValueRandomRange * 1000)) * 0.001);
        }
    }
    return m_aoExAttackValue.getValue(m_oAttackValue.getValue());
}

float CAttackAct::getCoolDown() const
{
    return getRealAttackInterval();
}

void CAttackAct::setBaseAttackInterval(float fAttackInterval)
{
    m_fCoolDown = max(fAttackInterval, CONST_MIN_ATTACK_SPEED_INTERVAL);
}

float CAttackAct::getBaseAttackInterval() const
{
    return m_fCoolDown;
}

float CAttackAct::getBaseAttackSpeed() const
{
    return 1 / max(getBaseAttackInterval(), FLT_EPSILON);
}

float CAttackAct::getRealAttackInterval() const
{
    float fAttackInterval = getBaseAttackInterval();
    // ȡ�����ٶ�ϵ��������С����Сֵ
    float fMulriple = m_oExAttackSpeed.getMulriple();
    fMulriple = max(fMulriple, CONST_MIN_ATTACK_SPEED_MULRIPLE);
    // ����������̹�������е����ֵ��Ϊ��̹������
    float fMinAttackSpeedInterval = fAttackInterval / CONST_MAX_ATTACK_SPEED_MULRIPLE;
    fMinAttackSpeedInterval = max(CONST_MIN_ATTACK_SPEED_INTERVAL, fMinAttackSpeedInterval);
    // ����ʵ�ʹ������������С��������̹������
    float fRealAttackInterval = fAttackInterval / fMulriple;
    return max(fRealAttackInterval, fMinAttackSpeedInterval);
}

float CAttackAct::getRealAttackSpeed() const
{
    return 1 / getRealAttackInterval();
}

void CAttackAct::setExAttackSpeed(const CExtraCoeff& roExAttackSpeed)
{
    m_oExAttackSpeed = roExAttackSpeed;  // ���뱣��ԭֵ�������п��ܳ�����Χ��������ɾ�����޷��ָ�
    updateAttackSpeed();
}

const CExtraCoeff& CAttackAct::getExAttackSpeed() const
{
    return m_oExAttackSpeed;
}

void CAttackAct::updateAttackSpeed()
{
    CUnit* o = getOwner();
    assert(o != NULL);
    o->updateAbilityCD(getId());
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    if (d->getCastActiveAbilityId() == o->getAttackAbilityId())
    {
        float spd = getBaseAttackInterval() / max(FLT_EPSILON, getRealAttackInterval());
        LOG("ATK SPD: %.1f", spd);
        d->setActionSpeed(d->getCastActionId(), spd);
    }
    
}

void CAttackAct::onTestAttackEffect(CMultiRefObject* pObj, void* pData)
{
    TEST_ATTACK_INFO* pAi = (TEST_ATTACK_INFO*)pData;
    
    CUnit* o = getOwner();
    CUnit* t = o->getUnit(pAi->iTarget);
    CAttackData* ad = pAi->pAttackData;
    
    if (t != NULL && t->isDead() == false)
    {
        t->damagedAdv(ad, o);
    }
    
    ad->release();
    
    delete pAi;
}

void CAttackAct::onUnitProjectileEffect( CProjectile* pProjectile )
{
    CUnit* o = getOwner();
    if (pProjectile->getFromToType() == CProjectile::kUnitToUnit || pProjectile->getFromToType() == CProjectile::kPointToUnit)
    {
        CUnit* t = o->getUnit(pProjectile->getToUnit());
        //CUnit* s = o->getUnit(pProjectile->getSourceUnit());
        if (t != NULL)
        {
            t->damagedAdv(pProjectile->getAttackData(), o);
        }
    }
}

// CBuffMakerAct
CBuffMakerAct::CBuffMakerAct(const char* pRootId, const char* pName, float fCoolDown, int iTemplateBuff, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags)
: CActiveAbility(pRootId, pName, fCoolDown, eCastType, dwEffectiveTypeFlags)
, m_iTemplateBuff(iTemplateBuff)
, m_pTarget(NULL)
{
    setDbgClassName("CBuffMakerAct");
}

CMultiRefObject* CBuffMakerAct::copy() const
{
    CBuffMakerAct* pRet = new CBuffMakerAct(getRootId(), getName(), getCoolDown(), getTemplateBuff(), getCastTargetType(), getEffectiveTypeFlags());
    pRet->setCastRange(getCastRange());
    pRet->setCastTargetRadius(getCastTargetRadius());
    pRet->setTemplateProjectile(getTemplateProjectile());
    return pRet;
}

bool CBuffMakerAct::checkConditions()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != NULL);

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        m_pTarget = o;
        break;
        
    case CCommandTarget::kUnitTarget:
        m_pTarget = o->getUnit(d->getCastTarget().getTargetUnit());
        if (m_pTarget != NULL && m_pTarget->isDead())
        {
            m_pTarget = NULL;
        }
        break;
        
    default:
        ;
    }
    
    if (m_pTarget != NULL &&
        !o->isEffective(DCAST(m_pTarget, CUnitForce*), getEffectiveTypeFlags()))
    {
        // ����д�ѡĿ��(���������Ŀ��)�������޷�����
        if (getCastTargetRadius() <= FLT_EPSILON ||
            getEffectiveTypeFlags() == CUnitForce::kSelf)
        {
            // ���������չ��Χ��λ
            return false;
        }
    }
    
    return true;
}

void CBuffMakerAct::onUnitCastAbility()
{
    CUnit* o = getOwner();
    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
    case CCommandTarget::kUnitTarget:
        if (o->isEffective(DCAST(m_pTarget, CUnitForce*), getEffectiveTypeFlags()))
        {
            m_pTarget->addBuffAbility(getTemplateBuff(), o->getId(), getLevel());
        }
        break;
        
    default:
        ;
    }
    
    if (getCastTargetRadius() <= FLT_EPSILON)
    {
        return;
    }
    
    CWorld* w = o->getWorld();
    CUnitDraw2D* od  = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != NULL);
    CBuffAbility* pBuff = NULL;
    CWorld::MAP_UNITS& mapUnits = w->getUnits();
    M_MAP_FOREACH(mapUnits)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;
        
        if (u == NULL || u->isDead())
        {
            continue;
        }
        
        if (!o->isEffective(DCAST(u, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            continue;
        }

        CUnitDraw2D* ud  = DCAST(u->getDraw(), CUnitDraw2D*);
        assert(ud != NULL);
        if (ud->getPosition().getDistance(od->getPosition()) > getCastTargetRadius())
        {
            continue;
        }
        
        if (pBuff == NULL)
        {
            w->copyAbility(getTemplateBuff())->dcast(pBuff);
        }
        else
        {
            pBuff->copy()->dcast(pBuff);
        }
        
        u->addBuffAbility(pBuff);
    }
}

// CLuaAbilityPas
CLuaAbilityPas::CLuaAbilityPas(const char* pRootId, const char* pName, float fCoolDown)
: CPassiveAbility(pRootId, pName, fCoolDown)
{
    setDbgClassName("CLuaAbilityPas");
}

CMultiRefObject* CLuaAbilityPas::copy() const
{
    CLuaAbilityPas* ret = new CLuaAbilityPas(getRootId(), getName(), m_fCoolDown);
    ret->setScriptHandler(getScriptHandler());
    return ret;
}

// CAuraPas
CAuraPas::CAuraPas(const char* pRootId, const char* pName, float fInterval, int iTemplateBuff, float fRange, uint32_t dwEffectiveTypeFlags)
: CPassiveAbility(pRootId, pName)
, m_iTemplateBuff(iTemplateBuff)
, m_fRange(fRange)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
{
    setDbgClassName("CAuraPas");
    setInterval(fInterval);
}

CAuraPas::~CAuraPas()
{
}

CMultiRefObject* CAuraPas::copy() const
{
    return new CAuraPas(getRootId(), getName(), m_fInterval, m_iTemplateBuff, m_fRange, m_dwEffectiveTypeFlags);
}

void CAuraPas::onUnitInterval()
{
    CUnit* o = getOwner();
    CWorld* w = o->getWorld();
    CBuffAbility* pBuff = NULL;
    
    CUnitDraw2D* od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != NULL);
    CWorld::MAP_UNITS& mapUnits = w->getUnits();
    M_MAP_FOREACH(mapUnits)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;
        
        if (u == NULL || u->isDead())
        {
            continue;
        }
        
        if (!o->isEffective(DCAST(u, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            continue;
        }

        CUnitDraw2D* ud = DCAST(u->getDraw(), CUnitDraw2D*);
        assert(ud != NULL);
        if (ud->getPosition().getDistance(od->getPosition()) > m_fRange)
        {
            continue;
        }
        
        if (pBuff == NULL)
        {
            w->copyAbility(m_iTemplateBuff)->dcast(pBuff);
        }
        else
        {
            pBuff->copy()->dcast(pBuff);
        }
        
        u->addBuffAbility(pBuff);
    }
}

// CAttackBuffMakerPas
CAttackBuffMakerPas::CAttackBuffMakerPas(const char* pRootId, const char* pName, float fProbability, int iTemplateBuff, bool bToSelf, const CExtraCoeff& roExAttackValue)
: CPassiveAbility(pRootId, pName)
, m_fProbability(fProbability)
, m_iTemplateBuff(iTemplateBuff)
, m_bToSelf(bToSelf)
, m_oExAttackValue(roExAttackValue)
{
    setDbgClassName("CAttackBuffMakerPas");
    setTriggerFlags(CUnit::kOnAttackTargetTrigger);
}

CMultiRefObject* CAttackBuffMakerPas::copy() const
{
    CAttackBuffMakerPas* ret = new CAttackBuffMakerPas(getRootId(), getName(), m_fProbability, m_iTemplateBuff, m_bToSelf, m_oExAttackValue);
    ret->setCoolDown(getCoolDown());
    return ret;
}

CAttackData* CAttackBuffMakerPas::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    if (isCoolingDown())
    {
        return pAttack;
    }

    coolDown();

    if (M_RAND_HIT(m_fProbability) == false)
    {
        return pAttack;
    }
    
    pAttack->getAttackValue().setValue(m_oExAttackValue.getValue(pAttack->getAttackValue().getValue()));
    
    if (m_iTemplateBuff != 0)
    {
        if (isToSelf())
        {
            CUnit* o = getOwner();
            o->addBuffAbility(m_iTemplateBuff, o->getId(), getLevel());
        }
        else
        {
            pAttack->addAttackBuff(CAttackBuff(m_iTemplateBuff, getLevel()));
        }
    }
    
    return pAttack;
}

// CVampirePas
CVampirePas::CVampirePas(const char* pRootId, const char* pName, float fPercentConversion)
: CPassiveAbility(pRootId, pName)
, m_fPercentConversion(fPercentConversion)
{
    setDbgClassName("CVampirePas");
    setTriggerFlags(CUnit::kOnDamageTargetDoneTrigger);
}

CMultiRefObject* CVampirePas::copy() const
{
    return new CVampirePas(getRootId(), getName(), getPercentConversion());
}

void CVampirePas::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    CUnit* o = getOwner();
    float fDtHp = fDamage * getPercentConversion();
    o->setHp(o->getHp() + fDtHp);
    LOG("%s�ָ�%d��HP", o->getName(), toInt(fDtHp));

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnit* u = getOwner();
    CUnitDrawForCC* d = NULL;
    u->getDraw()->dcast(d);

    if (d != NULL)
    {
        char sz[64];
        sprintf(sz, "+%d", toInt(fDtHp));
        d->addBattleTip(sz, "Comic Book", 18, ccc3(113, 205, 44));
    }
#endif
}

// CStunBuff
CStunBuff::CStunBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
{
    setDbgClassName("CStunBuff");
}

CMultiRefObject* CStunBuff::copy() const
{
    return new CStunBuff(getRootId(), getName(), m_fDuration, m_bStackable);
}

void CStunBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != NULL);
    d->cmdStop();
    o->suspend();
    
    LOG("%s%s��", o->getName(), getName());

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = NULL;
    o->getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(250, 104, 16));
    }
#endif
}

void CStunBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    o->resume();
    
    if (!o->isSuspended())
    {
        LOG("%s����%s", o->getName(), getName());
    }
}

// CDoubleAttackBuff
CDoubleAttackBuff::CDoubleAttackBuff(const char* pRootId, const char* pName)
: CBuffAbility(pRootId, pName, 0.0f, true)
{
    setDbgClassName("CDoubleAttackBuff");
}

CMultiRefObject* CDoubleAttackBuff::copy() const
{
    return new CDoubleAttackBuff(getRootId(), getName());
}

void CDoubleAttackBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    if (o->getAttackAbilityId() == 0)
    {
        return;
    }
    
    CAttackAct* pAtk = NULL;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtk);
    
    pAtk->resetCD();
    
    LOG("%s������%s", o->getName(), getName());
}

// CSpeedBuff
CSpeedBuff::CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_oExMoveSpeedDelta(roExMoveSpeedDelta)
, m_oExAttackSpeedDelta(roExAttackSpeedDelta)
{
    setDbgClassName("CSpeedBuff");
}

CMultiRefObject* CSpeedBuff::copy() const
{
    return new CSpeedBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_oExMoveSpeedDelta, m_oExAttackSpeedDelta);
}

void CSpeedBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    
    CUnitDraw2D* od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != NULL);
    const CExtraCoeff& rExMs = od->getExMoveSpeed();
    od->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() + m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() + m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = NULL;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtkAct);
    if (pAtkAct == NULL)
    {
        return;
    }
    
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() + m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() + m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s�����ٶȱ���(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = NULL;
    o->getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(72, 130, 200));
    }
#endif
}

void CSpeedBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    const CExtraCoeff& rExMs = d->getExMoveSpeed();
    d->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() - m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() - m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = NULL;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtkAct);
    if (pAtkAct == NULL)
    {
        return;
    }
    
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() - m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() - m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s�����ٶȻָ�(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());
}

// CChangeHpBuff
CChangeHpBuff::CChangeHpBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_oChangeHp(roChangeHp)
, m_oMinHp(roMinHp)
{
    setDbgClassName("CChangeHpBuff");
    setInterval(fInterval);
}

CMultiRefObject* CChangeHpBuff::copy() const
{
    return new CChangeHpBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_fInterval, m_oChangeHp, m_oMinHp);
}

void CChangeHpBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    LOG("%s���%s״̬(%.1f/s)\n", o->getName(), getName(), getChangeHp().getValue(o->getMaxHp()));
}

void CChangeHpBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    LOG("%sʧȥ%s״̬\n", o->getName(), getName());
}

void CChangeHpBuff::onUnitInterval()
{
    CUnit* o = getOwner();
    float fNewHp = o->getHp();
    float fChangeHp = getChangeHp().getValue(o->getMaxHp());
    fNewHp += fChangeHp;

    float fMinHp = getMinHp().getValue(o->getMaxHp());
    if (fNewHp < fMinHp)
    {
        fNewHp = fMinHp;
    }

    o->setHp(fNewHp);
}

// CRebirthPas
CRebirthPas::CRebirthPas( const char* pRootId, const char* pName, float fCoolDown, const CExtraCoeff& rExMaxHp )
    : CPassiveAbility(pRootId, pName, fCoolDown)
    , m_oExMaxHp(rExMaxHp)
    , m_bRevivableBefore(false)
{
    setDbgClassName("CRebirthPas");
    setTriggerFlags(CUnit::kOnDeadTrigger);
}

CMultiRefObject* CRebirthPas::copy() const
{
    return new CRebirthPas(getRootId(), getName(), getCoolDown(), m_oExMaxHp);
}

void CRebirthPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    setRevivableBefore(o->isRevivable());
    o->setRevivable();
}

void CRebirthPas::onUnitDelAbility()
{
    CUnit* o = getOwner();
    o->setRevivable(isRevivableBefore());
}

void CRebirthPas::onUnitDead()
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    CUnit* o = getOwner();
    CWorld* w = o->getWorld();

    CUnit* oo = w->getUnit(o->getId());
    float fHp = getExMaxHp().getValue(o->getMaxHp());
    if (oo != NULL)
    {
        oo->revive(fHp);
    }
    else
    {
        w->reviveUnit(o->getId(), fHp);
    }

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = NULL;
    o->getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(217, 47, 111));
    }
#endif

    LOG("Doing: %u", o->getDoingFlags());
}

// CEvadePas
CEvadePas::CEvadePas( const char* pRootId, const char* pName, float fChance, int iTemplateBuff )
    : CPassiveAbility(pRootId, pName)
    , m_fChance(fChance)
    , m_iTemplateBuff(iTemplateBuff)
{
    setDbgClassName("CEvadePas");
    setTriggerFlags(CUnit::kOnAttackedTrigger);
}

CMultiRefObject* CEvadePas::copy() const
{
    return new CEvadePas(getRootId(), getName(), m_fChance, m_iTemplateBuff);
}

CAttackData* CEvadePas::onUnitAttacked( CAttackData* pAttack, CUnit* pSource )
{
    if (M_RAND_HIT(getChance()))
    {
        CUnit* o = getOwner();
        if (getTemplateBuff() != 0)
        {
            o->addBuffAbility(getTemplateBuff(), o->getId(), getLevel());
        }

        LOG("%s%s��%s�Ĺ���", getOwner()->getName(), getName(), pSource->getName());

#ifdef DEBUG_FOR_CC
        // for cocos2d
        CUnitDrawForCC* ccd = NULL;
        o->getDraw()->dcast(ccd);

        if (ccd != NULL)
        {
            char sz[64];
            sprintf(sz, "%s!", getName());
            ccd->addBattleTip(sz, "Comic Book", 18, ccc3(250, 104, 16));
        }
#endif
        return NULL;
    }

    return pAttack;
}

// CEvadeBuff
CEvadeBuff::CEvadeBuff( const char* pRootId, const char* pName, float fDuration, bool bStackable, float fChance )
    : CBuffAbility(pRootId, pName, fDuration, bStackable)
    , m_fChance(fChance)
{
    setDbgClassName("CEvadeBuff");
    setTriggerFlags(CUnit::kOnAttackedTrigger);
}

CMultiRefObject* CEvadeBuff::copy() const
{
    return new CEvadeBuff(getRootId(), getName(), getDuration(), isStackable(), m_fChance);
}

CAttackData* CEvadeBuff::onUnitAttacked( CAttackData* pAttack, CUnit* pSource )
{
    if (M_RAND_HIT(getChance()))
    {
        LOG("%s%s��%s�Ĺ���", getOwner()->getName(), getName(), pSource->getName());

#ifdef DEBUG_FOR_CC
        // for cocos2d
        CUnit* o = getOwner();
        CUnitDrawForCC* ccd = NULL;
        o->getDraw()->dcast(ccd);

        if (ccd != NULL)
        {
            char sz[64];
            sprintf(sz, "%s!", getName());
            ccd->addBattleTip(sz, "Comic Book", 18, ccc3(250, 104, 16));
        }
        return NULL;
#endif
    }
    
    return pAttack;
}
