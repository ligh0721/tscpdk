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
#include "Ability.h"
#include "Item.h"
#include "Draw.h"


USING_NS_TSCPDK;


// CMyWorld
CMyWorld::CMyWorld()
: m_iUnitId(0)
, m_iHero(0)
{
}

bool CMyWorld::onInit()
{
    if (loadScript("../examples/GameServer/world.lua") == false)
    {
        exit(0);
    }
    
    CUnit* u = NULL;
    CAbility* pAbility = NULL;
    CItem* pItem = NULL;
    int id = 0;

    CAttackAct* atk = new CAttackAct("NormalAttack",
                                     "¹¥»÷",
                                     1.25,
                                     CAttackValue(CAttackValue::kPhysical, 30.0),
                                     0.5);

    u = new CUnit("CUnit");
    m_iUnitId = u->getId();
    u->setDraw(new CUnitDraw2D("Slime"));
    u->setName("Slime");
    u->setMaxHp(1000.0001);
    u->setForceByIndex(1);
    u->addActiveAbility(atk);
    this->addUnit(u);

    pAbility = new CChangeHpBuff("HP+10%/0.5s|2s",
                               "»Ö¸´",
                               2.0,
                               false,
                               0.5,
                               0.10,
                               true);
    id = addTemplateAbility(pAbility);

    pAbility = new CBuffMakerAct("Treat.[HP+10%/0.5s|2s]",
                               "»Ö¸´Êõ",
                               30.0,
                               id,
                               CCommandTarget::kNoTarget,
                               CUnitForce::kSelf);
    id = addTemplateAbility(pAbility);
    u->addActiveAbility(id);


    // hero init
    u = new CUnit("CUnit");
    u->setDraw(new CUnitDraw2D("Sw0rd"));
    m_iHero = u->getId();
    u->setName("¡¾Sw0rd¡¿");
    u->setMaxHp(1000.0001);
    u->setForceByIndex(0);
    this->addUnit(u);

    // Add Abilitys
    atk = new CAttackAct("NormalAttack",
                         "¹¥»÷",
                         2.0,
                         CAttackValue(CAttackValue::kPhysical, 10.0),
                         0.5);
    u->addActiveAbility(atk);
    
    pAbility = new CSpeedBuff("SlowDown",
                            "¼õËÙ",
                            2.0f,
                            false,
                            CExtraCoeff(0.0f, 0.0f),
                            CExtraCoeff(-0.1f, 0.0f));
    id = addTemplateAbility(pAbility);

    pAbility = new CAuraPas("Aura.[SlowDown]",
                          "¼õËÙ¹â»·",
                          1.0f,
                          id,
                          500.0f,
                          CUnitForce::kEnemy);
    id = addTemplateAbility(pAbility);
    u->addPassiveAbility(id);

    pAbility = new CStunBuff("Stun3s",
                           "»èÃÔ",
                           3.0,
                           false);
    id = addTemplateAbility(pAbility);

    pAbility = new CAttackBuffMakerPas("AttackBuffMaker.[Stun3s]",
                                     "ÖØ»÷",
                                     0.2f,
                                     id,
                                     false,
                                     CExtraCoeff(1.0, 50.0));
    id = addTemplateAbility(pAbility);
    u->addPassiveAbility(id);

    pAbility = new CDoubleAttackBuff("DoubleAttack",
                                   "Á¬»÷");
    id = addTemplateAbility(pAbility);

    pAbility = new CAttackBuffMakerPas("AttackBuffMaker.[DoubleAttack]",
                                     "Á¬»÷",
                                     0.3f,
                                     id,
                                     true,
                                     CExtraCoeff(0.5, 0.0));
    id = addTemplateAbility(pAbility);
    u->addPassiveAbility(id);

    pAbility = new CVampirePas("Vampire",
                             "20%ÎüÑª",
                             0.20f);
    id = addTemplateAbility(pAbility);
    u->addPassiveAbility(id);
    
    

    // Add Items
    u->setPackageSize(10);

    pAbility = new CChangeHpBuff("Bleed",
                               "³öÑª",
                               5.0f,
                               false,
                               0.5f,
                               CExtraCoeff(0.0f, -5.0f),
                               CExtraCoeff(0.0f, -1.0f));
    id = addTemplateAbility(pAbility);

    pAbility = new CAttackBuffMakerPas("AttackBuffMaker.[Bleed]",
                                     "ÖØÉË",
                                     0.2f,
                                     id,
                                     false,
                                     CExtraCoeff(2.0));
    id = addTemplateAbility(pAbility);

    pItem = new CItem("BleedSword", "ÖØÉË½£ÈÐ", CItem::kNormal, 1);
    pItem->addPassiveAbility(id);
    u->addItem(pItem);

    pAbility = new CChangeHpBuff("HP+10/0.5s|10s",
                               "HP»Ö¸´",
                               10.0,
                               false,
                               0.5,
                               CExtraCoeff(0.0f, 10.0f));
    id = addTemplateAbility(pAbility);

    pAbility = new CBuffMakerAct("Treat.[HP+10/0.5s|10s]",
                               "HP»Ö¸´",
                               10.0,
                               id,
                               CCommandTarget::kNoTarget,
                               CUnitForce::kSelf);
    id = addTemplateAbility(pAbility);

    pItem = new CItem("RedElixir", "HP»Ö¸´Ò©¼Á", CItem::kConsumable, 1000);
    pItem->addActiveAbility(id);
    pItem->setStackCount(2);
    u->addItem(pItem);

    OBJS_INFO;

    //u->setRevivable(true);
    //u->setCastTarget(CCommandTarget(m_iUnitId));
    //u->cmdCastSpell(u->getAttackAbilityId());
    //atk->setExAttackSpeed(CExtraCoeff(1.0, 0.0));
    return true;
}

void CMyWorld::step(float dt)
{
    if (getUnits().size() <= 1 && this->getAbilitysCD().empty())
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
