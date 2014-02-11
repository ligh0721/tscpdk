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
        u->castSkill(u->getAttackSkillId());
    }

    virtual void onUnitTick(float dt)
    {
        CUnit* u = getNotifyUnit();

        if (m_iTargetId != 0)
        {
            u->setCastTarget(CCommandTarget(m_iTargetId));
            u->castSkill(u->getAttackSkillId());
        }
    }

    int m_iTargetId;

};

// CMyWorld

CMyWorld::CMyWorld()
: m_iUnitId(0)
, m_iUnitId2(0)
{
    CUnit* u = NULL;
    CSkill* pSkill = NULL;
    int id = 0;

    CAttackAct* atk = new CAttackAct("NormalAttack",
                                     "普通攻击",
                                     1.25,
                                     CAttackValue(1,
                                                  CAttackValue::kPhysical,
                                                  30.0),
                                     0.5);

    u = new CUnit("CUnit");
    m_iUnitId = u->getId();
    u->setName("Slime");
    u->setMaxHp(1000.0001);
    u->addActiveSkill(atk);
    u->setAI<CMyAI>();
    this->addUnit(u);
    u->setForceByIndex(1);

    // hero init
    u = new CUnit("CUnit");
    m_iUnitId2 = u->getId();
    u->setName("【Sw0rd】");
    u->setMaxHp(1000.0001);
    atk = new CAttackAct("NormalAttack",
                         "普通攻击",
                         2.0,
                         CAttackValue(1,
                                      CAttackValue::kPhysical,
                                      10.0),
                         0.5);
    u->addActiveSkill(atk);
    this->addUnit(u);
    u->setForceByIndex(0);

    pSkill = new CHpChangeBuff("Bleed",
                               "出血",
                               5.0f,
                               false,
                               0.5f,
                               -5.0f,
                               false,
                               -1.0);
    id = addTemplateSkill(pSkill);

    pSkill = new CAttackBuffMakerPas("AttackBuffMaker",
                                     "利刃",
                                     0.2f,
                                     id,
                                     CExtraCoeff());
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(dynamic_cast<CPassiveSkill*>(pSkill->copy()));

    pSkill = new CSpeedBuff("SlowDown",
                            "霜冻",
                            2.0f,
                            false,
                            CExtraCoeff(0.0f, 0.0f),
                            CExtraCoeff(-0.1f, 0.0f));
    id = addTemplateSkill(pSkill);

    pSkill = new CAuraPas("SlowDownAura",
                          "霜冻光环",
                          1.0f,
                          id,
                          500.0f,
                          CUnitForce::kEnemy);
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(dynamic_cast<CPassiveSkill*>(pSkill->copy()));
    
    pSkill = new CAttackBuffMakerPas("AttackBuffMaker",
                                     "暴击",
                                     0.2f,
                                     0,
                                     CExtraCoeff(5.0, 0.0f));
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(dynamic_cast<CPassiveSkill*>(pSkill->copy()));

    //u->setRevivable(true);

    //u->setAI<CUnitEventAdapter>();

    u->setCastTarget(CCommandTarget(m_iUnitId));
    u->castSkill(u->getAttackSkillId());
    //atk->setExAttackSpeed(CExtraCoeff(1.0, 0.0));

}

void CMyWorld::onTick(float dt)
{
    if (getUnits().size() <= 1 && this->getSkillsCD().empty())
    {
        CDbgMultiRefObjectManager::sharedDbgMultiRefObjectManager()->printDbgInfo();

        if (getUnits().size() == 1)
        {
            CUnit* winner = getUnits().begin()->second;
            LOG("%s(%d/%d)胜出\n", winner->getName(), (int)round(winner->getHp()), (int)round(winner->getMaxHp()));
            this->delUnit(getUnits().begin()->first);
        }

        CDbgMultiRefObjectManager::sharedDbgMultiRefObjectManager()->printDbgInfo();
        exit(EXIT_SUCCESS);

        return;
    }

    CWorld::onTick(dt);

    CUnit* u = this->getUnit(m_iUnitId2);
    if (u)
    {
        u->setCastTarget(CCommandTarget(m_iUnitId));
        u->castSkill(u->getAttackSkillId());
    }
}

void CMyWorld::onActEnd(CUnit* pUnit, void* Data)
{
}

// CMyApp

bool CMyApp::applicationDidFinishLaunching()
{
    setAnimationInterval(1.0 / 60);

    m_pG = new CMyWorld();
    m_pG->retain();

    setBursting(true);
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
