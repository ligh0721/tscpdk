/* 
 * File:   Skill.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午11:45
 */

#include "CommInc.h"
#include "Unit.h"
#include "Skill.h"
#include "Application.h"


// CSkill
CSkill::CSkill(const char* pRootId, const char* pName, float fCoolDown)
: CONST_ROOT_ID(pRootId)
, m_sName(pName)
, m_pOwner(NULL)
, m_fCoolDown(fCoolDown)
, m_fCoolingDownElapsed(FLT_MAX)
, m_dwTriggerFlags(0)
{
    setDbgClassName("CSkill");
}

CSkill::~CSkill()
{
}

const char* CSkill::getRootId() const
{
    return CONST_ROOT_ID.c_str();
}

bool CSkill::isCoolingDown() const
{
    return m_fCoolingDownElapsed < getCoolDown();
}

void CSkill::resetCD()
{
    m_fCoolingDownElapsed = FLT_MAX;
}

void CSkill::coolDown()
{
    setCoolingDownElapsed(0.0f);
}

void CSkill::onUnitAddSkill()
{
}

void CSkill::onUnitDelSkill()
{
}

void CSkill::onUnitSkillReady()
{
}

void CSkill::onUnitRevive()
{
}

void CSkill::onUnitDie()
{
}

void CSkill::onUnitHpChange(float fChanged)
{
}

void CSkill::onUnitTick(float dt)
{
}

CAttackData* CSkill::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    return pAttack;
}

CAttackData* CSkill::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    return pAttack;
}

void CSkill::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
}

void CSkill::onUnitDamagedDone(float fDamage, CUnit* pSource)
{
}

void CSkill::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
}

void CSkill::onUnitDestroyProjectile(CProjectile* pProjectile)
{
}

void CSkill::onAddToUnit(CUnit* pOwner, bool bNotify)
{
    setOwner(pOwner);
    if (bNotify)
    {
        onUnitAddSkill();
    }
}

void CSkill::onDelFromUnit(bool bNotify)
{
    if (bNotify)
    {
        onUnitDelSkill();
    }
    
    setOwner(NULL);
}

//
/*
// CLevelLimitSkill
CLevelLimitSkill::CLevelLimitSkill()
{
}

CLevelLimitSkill::~CLevelLimitSkill()
{
}

bool CLevelLimitSkill::canBeLearning(uint32_t dwLvl) const
{
    return (m_dwLvl < m_dwMaxLvl) && (dwLvl >= m_dwLvl * m_dwLvlStp + m_dwLvlLmt);
}
*/

// CPassiveSkill
CPassiveSkill::CPassiveSkill(const char* pRootId, const char* pName, float fCoolDown)
: CSkill(pRootId, pName, fCoolDown)
{
    setDbgClassName("CPassiveSkill");
}

CPassiveSkill::~CPassiveSkill()
{
}

// CBuffSkill
CBuffSkill::CBuffSkill(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CPassiveSkill(pRootId, pName, 0.0f)
, m_fDuration(fDuration)
, m_fElapsed(0.0f)
, m_bStackable(bStackable)
, m_iSrcUnit(0)
{
    setDbgClassName("CBuffSkill");
}

CBuffSkill::~CBuffSkill()
{
}

bool CBuffSkill::isDone() const
{
    return m_fElapsed >= m_fDuration;
}

// CActiveSkill
CActiveSkill::CActiveSkill(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwDamageTypeFlags)
: CSkill(pRootId, pName, fCoolDown)
, m_eCastTargetType(eCastType)
, m_dwCastTargetDamageTypeFlags(dwDamageTypeFlags)
, m_fCastRange(0.0f)
, m_fCastTargetRadius(0.0f)
//, m_iCastTargetUnit(0)
, m_iTemplateProjectile(0)
{
    setDbgClassName("CActiveSkill");
}

CActiveSkill::~CActiveSkill()
{
}

bool CActiveSkill::cast()
{
    if (isCoolingDown())
    {
        return false;
    }
    
    if (checkCondition() == false)
    {
        return false;
    }
    
    coolDown();
    onUnitCastSkill();  // onCastSkill在cd变化下面，所以可以添加重置cd的逻辑
    
    return true;
}

void CActiveSkill::onUnitCastSkill()
{
}

bool CActiveSkill::checkCondition()
{
    return true;
}



// CAuraPas
CAuraPas::CAuraPas(const char* pRootId, const char* pName, float fCoolDown, int iTemplateBuff, float fRange, uint32_t dwTargetFlags)
: CPassiveSkill(pRootId, pName, fCoolDown)
, m_iTemplateBuff(iTemplateBuff)
, m_fRange(fRange)
, m_dwTargetFlags(dwTargetFlags)
{
    setDbgClassName("CAuraPas");
}

CAuraPas::~CAuraPas()
{
}

CMultiRefObject* CAuraPas::copy() const
{
    return new CAuraPas(CONST_ROOT_ID.c_str(), m_sName.c_str(), m_fCoolDown, m_iTemplateBuff, m_fRange, m_dwTargetFlags);
}

// CAttackAct
const float CAttackAct::CONST_MIN_ATTACK_SPEED_INTERVAL = 0.1; // 0.1s
const float CAttackAct::CONST_MIN_ATTACK_SPEED_MULRIPLE = 0.2; // 20%
const float CAttackAct::CONST_MAX_ATTACK_SPEED_MULRIPLE = 5.0; // 500%
const float CAttackAct::CONST_MAX_ATTACK_BUFFER_RANGE = 50.0;
const float CAttackAct::CONST_MAX_CLOSE_ATTACK_Y_RANGE = 5.0;

CAttackAct::CAttackAct(const char* pRootId, const char* pName, float fCoolDown, const CAttackValue& rAttackValue, float fAttackValueRandomRange)
: CActiveSkill(pRootId, pName, fCoolDown, CCommandTarget::kUnitTarget, CUnitForce::kEnemy)
, m_oAttackValue(rAttackValue)
, m_fAttackValueRandomRange(fAttackValueRandomRange)
{
    setDbgClassName("CAttackAct");
}

CMultiRefObject* CAttackAct::copy() const
{
    return new CAttackAct(CONST_ROOT_ID.c_str(), m_sName.c_str(), m_fCoolDown, m_oAttackValue, m_fAttackValueRandomRange);
}

void CAttackAct::onUnitAddSkill()
{
    getOwner()->setAttackSkillId(getId());
}

void CAttackAct::onUnitDelSkill()
{
    getOwner()->setAttackSkillId(0);
}

void CAttackAct::onUnitCastSkill()
{
    CUnit* o = getOwner();
    CUnit* t = o->getUnit(o->getCastTarget().getTargetUnit());
    
    if (t == NULL)
    {
        return;
    }
    
    CAttackData* ad = new CAttackData();
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        ad->setAttackValue((CAttackValue::ATTACK_TYPE)i, getRealAttackValue((CAttackValue::ATTACK_TYPE)i));
    }
    
    ad = o->attackAdv(ad, t);
    
    LOG("%s的攻击..", o->getName());
    
    // 这里模拟命中
    if (ad == NULL)
    {
        return;
    }
    
    ad->retain();
    TEST_ATTACK_INFO* pAi = new TEST_ATTACK_INFO;
    pAi->iTarget = t->getId();
    pAi->pAttackData = ad;
    o->runAction(new CCallFunc(this, (FUNC_CALLFUNC_ND)&CAttackAct::onTestAttackEffect, pAi));
}

bool CAttackAct::checkCondition()
{
    CUnit* o = getOwner();
    CUnit* t = o->getUnit(o->getCastTarget().getTargetUnit());
    if (t == NULL || t->isDead())
    {
        return false;
    }
    
    return true;
}

float CAttackAct::getBaseAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const
{
    return m_oAttackValue.getValue(eAttackType);
}

void CAttackAct::setExAttackValue(CAttackValue::ATTACK_TYPE eAttackType, const CExtraCoeff& roExAttackValue)
{
    m_aoExAttackValue[eAttackType] = roExAttackValue;
}

const CExtraCoeff& CAttackAct::getExAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const
{
    return m_aoExAttackValue[eAttackType];
}

float CAttackAct::getRealAttackValue(CAttackValue::ATTACK_TYPE eAttackType, bool bUseRandomRange) const
{
    if (bUseRandomRange)
    {
        float fAttackValueRandomRange = m_oExAttackValueRandomRange.getValue(m_fAttackValueRandomRange);
        if (fAttackValueRandomRange > 0.001)
        {
            return m_aoExAttackValue[eAttackType].getValue(m_oAttackValue.getValue(eAttackType)) * (1 - fAttackValueRandomRange * 0.5 + (rand() % (int)(fAttackValueRandomRange * 1000)) * 0.001);
        }
    }
    return m_aoExAttackValue[eAttackType].getValue(m_oAttackValue.getValue(eAttackType));
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
    // 取攻击速度系数，不得小于最小值
    float fMulriple = m_oExAttackSpeed.getMulriple();
    fMulriple = max(fMulriple, CONST_MIN_ATTACK_SPEED_MULRIPLE);
    // 计算两种最短攻击间隔中的最大值作为最短攻击间隔
    float fMinAttackSpeedInterval = fAttackInterval / CONST_MAX_ATTACK_SPEED_MULRIPLE;
    fMinAttackSpeedInterval = max(CONST_MIN_ATTACK_SPEED_INTERVAL, fMinAttackSpeedInterval);
    // 计算实际攻击间隔，不得小于上述最短攻击间隔
    float fRealAttackInterval = fAttackInterval / fMulriple;
    return max(fRealAttackInterval, fMinAttackSpeedInterval);
}

float CAttackAct::getRealAttackSpeed() const
{
    return 1 / getRealAttackInterval();
}

void CAttackAct::setExAttackSpeed(const CExtraCoeff& roExAttackSpeed)
{
    m_oExAttackSpeed = roExAttackSpeed;  // 必须保留原值，尽管有可能超出范围，否则技能删除后无法恢复
    updateAttackSpeed();
}

const CExtraCoeff& CAttackAct::getExAttackSpeed() const
{
    return m_oExAttackSpeed;
}

void CAttackAct::updateAttackSpeed()
{
    CUnit* o = getOwner();
    o->updateSkillCD(getId());
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

// CAttackBuffMakerPas
CAttackBuffMakerPas::CAttackBuffMakerPas(const char* pRootId, const char* pName, float fProbability, int iTemplateBuff, const CExtraCoeff& roExAttackValue)
: CPassiveSkill(pRootId, pName)
, m_fProbability(fProbability)
, m_iTemplateBuff(iTemplateBuff)
, m_oExAttackValue(roExAttackValue)
{
    setDbgClassName("CAttackBuffMakerPas");
    setTriggerFlags(CUnit::kAttackTargetTrigger);
}

CMultiRefObject* CAttackBuffMakerPas::copy() const
{
    return new CAttackBuffMakerPas(CONST_ROOT_ID.c_str(), m_sName.c_str(), m_fProbability, m_iTemplateBuff, m_oExAttackValue);
}

CAttackData* CAttackBuffMakerPas::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        const CAttackValue& rAtkVal = pAttack->getAttackValue();
        if (rAtkVal.getValue((CAttackValue::ATTACK_TYPE)i) > FLT_EPSILON)
        {
            pAttack->setAttackValue((CAttackValue::ATTACK_TYPE)i, m_oExAttackValue.getValue(rAtkVal.getValue((CAttackValue::ATTACK_TYPE)i)));
        }
    }
    
    pAttack->addAttackBuff(CAttackBuff(m_iTemplateBuff, getLevel(), m_fProbability));
    
    return pAttack;
}

// CSpeedBuff
CSpeedBuff::CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta)
: CBuffSkill(pRootId, pName, fDuration, bStackable)
, m_oExMoveSpeedDelta(roExMoveSpeedDelta)
, m_oExAttackSpeedDelta(roExAttackSpeedDelta)
{
    setDbgClassName("CSpeedBuff");
}

CMultiRefObject* CSpeedBuff::copy() const
{
    return new CSpeedBuff(CONST_ROOT_ID.c_str(), m_sName.c_str(), m_fDuration, m_bStackable, m_oExMoveSpeedDelta, m_oExAttackSpeedDelta);
}

void CSpeedBuff::onUnitAddSkill()
{
    CUnit* o = getOwner();
    
    const CExtraCoeff& rExMs = o->getExMoveSpeed();
    o->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() + m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() + m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = dynamic_cast<CAttackAct*>(o->getActiveSkill(o->getAttackSkillId()));
    if (pAtkAct == NULL)
    {
        return;
    }
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() + m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() + m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s攻击速度变慢(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());
}

void CSpeedBuff::onUnitDelSkill()
{
    CUnit* o = getOwner();
    
    const CExtraCoeff& rExMs = o->getExMoveSpeed();
    o->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() - m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() - m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = dynamic_cast<CAttackAct*>(o->getActiveSkill(o->getAttackSkillId()));
    if (pAtkAct == NULL)
    {
        return;
    }
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() - m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() - m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s攻击速度恢复(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());
}
