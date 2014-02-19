/* 
 * File:   GameServer.cpp
 * Author: thunderliu
 * 
 * Created on 2013Äê12ÔÂ8ÈÕ, ÏÂÎç7:39
 */

#include <tscpdk/TSCPDK.h>

#include "CommInc.h"
#include "GameServer.h"
#include "Action.h"
#include "Skill.h"
#include "Item.h"
#include "Draw.h"


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

class CHeroAI : public CUnitEventAdapter
{
public:

    CHeroAI()
    : m_iTargetId(0)
    , m_iTreat(0)
    , m_iRedElixir(0)
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

        if (u->getHp() / (u->getMaxHp() + FLT_EPSILON) < 0.5)
        {
            if (u->castSkill(m_iTreat) == false)
            {
            }
            else if (u->useItem(m_iRedElixir) == false)
            {
            }

        }

        if (m_iTargetId != 0)
        {
            u->setCastTarget(CCommandTarget(m_iTargetId));
            u->castSkill(u->getAttackSkillId());
        }
    }

    virtual void onUnitAddActiveSkill(CActiveSkill* pSkill)
    {
        if (strcmp(pSkill->getName(), "»Ö¸´Êõ") == 0)
        {
            m_iTreat = pSkill->getId();
        }
    }

    virtual void onUnitAddItem(int iIndex)
    {
        CUnit* u = getNotifyUnit();
        CItem* pItem = u->getItem(iIndex);
        if (pItem == NULL)
        {
            return;
        }

        if (strcmp(pItem->getName(), "HP»Ö¸´Ò©¼Á") == 0)
        {
            m_iRedElixir = iIndex;
        }
    }


    int m_iTargetId;
    int m_iTreat;
    int m_iRedElixir;

};

// CMyWorld

CMyWorld::CMyWorld()
: m_iUnitId(0)
, m_iHero(0)
{
}

void CMyWorld::onInit()
{
    CUnit* u = NULL;
    CSkill* pSkill = NULL;
    CItem* pItem = NULL;
    int id = 0;

    CAttackAct* atk = new CAttackAct("NormalAttack",
                                     "¹¥»÷",
                                     1.25,
                                     CAttackValue(1,
                                                  CAttackValue::kPhysical,
                                                  30.0),
                                     0.5);

    u = new CUnit("CUnit");
    m_iUnitId = u->getId();
    u->setDraw(new CUnitDraw2D("Slime"));
    u->setName("Slime");
    u->setMaxHp(1000.0001);
    u->setForceByIndex(1);
    u->addActiveSkill(atk);
    u->setAI(CHeroAI());
    this->addUnit(u);

    pSkill = new CHpChangeBuff("HP+10%/0.5s|2s",
                               "»Ö¸´",
                               2.0,
                               false,
                               0.5,
                               0.10,
                               true);
    id = addTemplateSkill(pSkill);

    pSkill = new CBuffMakerAct("Treat.[HP+10%/0.5s|2s]",
                               "»Ö¸´Êõ",
                               30.0,
                               id,
                               CCommandTarget::kNoTarget,
                               CUnitForce::kSelf);
    id = addTemplateSkill(pSkill);
    u->addActiveSkill(id);


    // hero init
    u = new CUnit("CUnit");
    u->setDraw(new CUnitDraw2D("Sw0rd"));
    m_iHero = u->getId();
    u->setName("¡¾Sw0rd¡¿");
    u->setMaxHp(1000.0001);
    u->setForceByIndex(0);
    u->setAI(CHeroAI());
    this->addUnit(u);

    // Add Skills
    atk = new CAttackAct("NormalAttack",
                         "¹¥»÷",
                         2.0,
                         CAttackValue(1,
                                      CAttackValue::kPhysical,
                                      10.0),
                         0.5);
    u->addActiveSkill(atk);

    pSkill = new CSpeedBuff("SlowDown",
                            "¼õËÙ",
                            2.0f,
                            false,
                            CExtraCoeff(0.0f, 0.0f),
                            CExtraCoeff(-0.1f, 0.0f));
    id = addTemplateSkill(pSkill);

    pSkill = new CAuraPas("Aura.[SlowDown]",
                          "¼õËÙ¹â»·",
                          1.0f,
                          id,
                          500.0f,
                          CUnitForce::kEnemy);
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(id);

    pSkill = new CStunBuff("Stun3s",
                           "»èÃÔ",
                           3.0,
                           false);
    id = addTemplateSkill(pSkill);

    pSkill = new CAttackBuffMakerPas("AttackBuffMaker.[Stun3s]",
                                     "ÖØ»÷",
                                     0.2f,
                                     id,
                                     false,
                                     CExtraCoeff(1.0, 50.0));
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(id);

    pSkill = new CDoubleAttackBuff("DoubleAttack",
                                   "Á¬»÷");
    id = addTemplateSkill(pSkill);

    pSkill = new CAttackBuffMakerPas("AttackBuffMaker.[DoubleAttack]",
                                     "Á¬»÷",
                                     0.3f,
                                     id,
                                     true,
                                     CExtraCoeff(0.5, 0.0));
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(id);

    pSkill = new CVampirePas("Vampire",
                             "20%ÎüÑª",
                             0.20f);
    id = addTemplateSkill(pSkill);
    u->addPassiveSkill(id);
    
    

    // Add Items
    u->setPackageSize(10);

    pSkill = new CHpChangeBuff("Bleed",
                               "³öÑª",
                               5.0f,
                               false,
                               0.5f,
                               -5.0f,
                               false,
                               -1.0);
    id = addTemplateSkill(pSkill);

    pSkill = new CAttackBuffMakerPas("AttackBuffMaker.[Bleed]",
                                     "ÖØÉË",
                                     0.2f,
                                     id,
                                     false,
                                     CExtraCoeff(2.0));
    id = addTemplateSkill(pSkill);

    pItem = new CItem("BleedSword", "ÖØÉË½£ÈÐ", CItem::kNormal, 1);
    pItem->addPassiveSkill(id);
    u->addItem(pItem);

    pSkill = new CHpChangeBuff("HP+10/0.5s|10s",
                               "HP»Ö¸´",
                               10.0,
                               false,
                               0.5,
                               10.0,
                               false);
    id = addTemplateSkill(pSkill);

    pSkill = new CBuffMakerAct("Treat.[HP+10/0.5s|10s]",
                               "HP»Ö¸´",
                               10.0,
                               id,
                               CCommandTarget::kNoTarget,
                               CUnitForce::kSelf);
    id = addTemplateSkill(pSkill);

    pItem = new CItem("RedElixir", "HP»Ö¸´Ò©¼Á", CItem::kConsumable, 1000);
    pItem->addActiveSkill(id);
    pItem->setStackCount(2);
    u->addItem(pItem);

    OBJS_INFO;

    //u->setRevivable(true);
    u->setCastTarget(CCommandTarget(m_iUnitId));
    u->castSkill(u->getAttackSkillId());
    //atk->setExAttackSpeed(CExtraCoeff(1.0, 0.0));
}

void CMyWorld::step(float dt)
{
    if (getUnits().size() <= 1 && this->getSkillsCD().empty())
    {
        OBJS_INFO;

        if (getUnits().size() == 1)
        {
            CUnit* winner = getUnits().begin()->second;
            LOG("%s(%d/%d)Ê¤³ö\n", winner->getName(), (int)round(winner->getHp()), (int)round(winner->getMaxHp()));
            this->delUnit(getUnits().begin()->first);
        }

        OBJS_INFO;
        exit(EXIT_SUCCESS);

        return;
    }

    CWorld::step(dt);
}

void CMyWorld::onActEnd(CUnit* pUnit, void* Data)
{
}

// CMyApp

bool CMyApp::applicationDidFinishLaunching()
{
    setAnimationInterval(1.0 / 60);

    m_pWorld = new CMyWorld();
    m_pWorld->retain();
    m_pWorld->init();

    setBursting(true);
    return true;
}

void CMyApp::applicationTick(float dt)
{
    m_pWorld->step(dt);
}

long CMyApp::getCurMSec()
{
	struct timeval stCurrentTime;
	gettimeofday(&stCurrentTime, NULL);
	return stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec * 0.001; // millseconds
}

void CMyApp::SleepForMSec(unsigned int dwMSec)
{
    ::SleepForMSec(dwMSec);
}

int main(int argc, char* argv[])
{
    CMyApp oApp;
    oApp.run();

    return 0;
}
