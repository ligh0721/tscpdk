/* 
 * File:   GameServer.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午7:39
 */

#include <tscpdk/TSCPDK.h>

#include "CommInc.h"
#include "GameServer.h"
#include "Logic/Action.h"
#include "Logic/Skill.h"

USING_NS_TSCPDK;

class CMyAI : public CUnitEventAdapter
{
public:
    CMyAI()
    : m_iTargetId(0)
    {
    }
    
    virtual void onUnitDamagedDone(float fDamaged, CUnit* pSource)
    {
        CUnit* u = getNotifyUnit();
        
        m_iTargetId = pSource->getId();
        u->setCastTarget(CCommandTarget(m_iTargetId));
        u->cast(u->getAttackSkillId());
    }

    virtual void onUnitTick(float dt)
    {
        CUnit* u = getNotifyUnit();
        
        if (m_iTargetId != 0)
        {
            u->setCastTarget(CCommandTarget(m_iTargetId));
            u->cast(u->getAttackSkillId());
        }
    }
    
    int m_iTargetId;

};

// CMyWorld
CMyWorld::CMyWorld()
: m_iUnitId(0)
, m_iUnitId2(0)
, m_bExit(false)
{
    CAttackAct* atk = new CAttackAct("CAttackAct",
                                     "普通攻击",
                                     0.5,
                                     CAttackValue(1,
                                                  CAttackValue::kPhysical,
                                                  5.0),
                                     0.5);
    
    CUnit* u = new CUnit("CUnit");
    m_iUnitId = u->getId();
    u->setName("Slime");
    u->setMaxHp(75.0001);
    u->addActiveSkill(atk);
    u->setAI<CMyAI>();
    this->addUnit(u);
    
    // hero init
    u = new CUnit("CUnit");
    m_iUnitId2 = u->getId();
    u->setName("Sw0rd");
    u->setMaxHp(100.0001);
    atk = dynamic_cast<CAttackAct*>(atk->copy());
    atk->setBaseAttackInterval(2.0);
    u->addActiveSkill(atk);
    this->addUnit(u);
    
    CSkill* pSkill = new CSpeedBuff("CSpeedBuff", "减速", 10.0f, true, CExtraCoeff(0.0f, 0.0f), CExtraCoeff(-0.05f, 0.0f));
    int id = addTemplateSkill(pSkill);
    
    pSkill = new CAttackBuffMakerPas("CAttackBuffMakerPas", "霜冻攻击", 1.0f, id, CExtraCoeff());
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(dynamic_cast<CPassiveSkill*>(pSkill->copy()));
    //u->setRevivable(true);
    
    //u->setAI<CUnitEventAdapter>();
    
    u->setCastTarget(CCommandTarget(m_iUnitId));
    u->cast(u->getAttackSkillId());
    //atk->setExAttackSpeed(CExtraCoeff(1.0, 0.0));
    
    
#if 0
    m_bExit = false;
    u->runAction(new CSpeed(new CSequence(new CMoveTo(5, CPoint(100, 200)),
                                                new CCallFunc(this, (FUNC_CALLFUNC_ND)&CMyWorld::onActEnd)),
                                  0.5));
#endif
}

void CMyWorld::onTick(float dt)
{
    if (getUnits().size() <= 1 && this->getSkillsCD().empty())
    {
        CDbgMultiRefObjectManager::sharedDbgMultiRefObjectManager()->printDbgInfo();
        
        if (getUnits().size() == 1)
        {
            LOG("%s胜出\n", getUnits().begin()->second->getName());
            this->delUnit(getUnits().begin()->first);
        }
        
        CDbgMultiRefObjectManager::sharedDbgMultiRefObjectManager()->printDbgInfo();
        exit(EXIT_SUCCESS);
        
        return;
    }
    
    CWorld::onTick(dt);
    
    if (m_bExit)
    {
        return;
    }
    
    CUnit* u = this->getUnit(m_iUnitId2);
    if (u)
    {
        u->setCastTarget(CCommandTarget(m_iUnitId));
        u->cast(u->getAttackSkillId());
    }
}

void CMyWorld::onActEnd(CUnit* pUnit, void* Data)
{
    m_bExit = true;
}

// CMyApp
bool CMyApp::applicationDidFinishLaunching()
{
    setAnimationInterval(1.0 / 60);
    
    m_pG = new CMyWorld();
    m_pG->retain();
    
    return true;
}

void CMyApp::applicationTick(float dt)
{
    m_pG->onTick(dt);
}


int main(int argc, char* argv[])
{
    CMyApp oApp;
    oApp.run();
    
    
    return 0;
}
