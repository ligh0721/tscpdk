/* 
 * File:   Item.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月11日, 上午1:08
 */

#include "CommInc.h"
#include "Item.h"
#include "Unit.h"
#include "Skill.h"
#include "Application.h"


// CItem
CItem::CItem(const char* pRootId, const char* pName, ITEM_TYPE eItemType, int iMaxStackSize)
: CONST_ROOT_ID(pRootId)
, m_sName(pName)
, m_eItemType(kNormal)
, m_bEquipped(false)
, m_iStackCount(1)
, m_iMaxStackSize(iMaxStackSize)
, m_iActSkillId(0)
, m_pActSkill(NULL)
, m_pOwner(NULL)
{
}

CItem::~CItem()
{
    if (m_pActSkill != NULL)
    {
        m_pActSkill->release();
    }
}

const char* CItem::getRootId() const
{
    return CONST_ROOT_ID.c_str();
}

void CItem::addStackCount()
{
    if (m_iStackCount >= m_iMaxStackSize)
    {
        return;
    }
    ++m_iStackCount;
}

void CItem::addPassiveSkill(int id)
{
    m_vecPasSkillIds.push_back(id);
}

void CItem::onUnitAddItem()
{
}

void CItem::onUnitDelItem()
{
}

bool CItem::use()
{
    CUnit* o = getOwner();
    if (o == NULL)
    {
        return NULL;
    }
    
    if (m_eItemType == kEquipment)
    {
        // 属于装备，使用代表进行装备
        // TODO: 装备上
    }
    else
    {
        // 直接可以使用
        if (m_pActSkill == NULL)
        {
            // 无主动技能，直接返回
            return false;
        }
        
        if (m_pActSkill->cast() == false)
        {
            return false;
        }
        
        onUnitUseItem();
        
        if (m_eItemType == kConsumable)
        {
            // 是消耗品
            --m_iStackCount;
            if (m_iStackCount == 0)
            {
                // 应该删除该物品
                onDelFromSlot();
            }
        }
    }
    
    return true;
}

void CItem::onUnitUseItem()
{
}

void CItem::onAddToNewSlot(CUnit* pOwner, bool bNotify)
{
    setOwner(pOwner);
    
    if (m_eItemType != kEquipment)
    {
        addSkillToOwner(pOwner);
    }
    
    if (bNotify)
    {
        onUnitAddItem();
    }
}

void CItem::onDelFromSlot(bool bNotify)
{
    if (m_eItemType != kEquipment)
    {
        delSkillFromOwner();
    }
    
    if (bNotify)
    {
        onUnitDelItem();
    }
    
    setOwner(NULL);
}

void CItem::addSkillToOwner(CUnit* pOwner)
{
    CWorld* w = pOwner->getWorld();

    if (m_iActSkillId != 0)
    {
        if (m_pActSkill == NULL)
        {
            m_pActSkill = dynamic_cast<CActiveSkill*>(w->copySkill(m_iActSkillId));
            if (m_pActSkill != NULL)
            {
                m_pActSkill->retain();
            }
            
        }
        
        pOwner->addActiveSkill(m_pActSkill, false);
    }
    
    if (m_vecPasSkills.empty())
    {
        for (auto it = m_vecPasSkillIds.begin(); it != m_vecPasSkillIds.end(); ++it)
        {
            CPassiveSkill* pPasSkill = dynamic_cast<CPassiveSkill*>(w->copySkill(*it));
            if (pPasSkill != NULL)
            {
                m_vecPasSkills.addObject(pPasSkill);
            }
        }
    }
    
    M_VEC_FOREACH(m_vecPasSkills)
    {
        CPassiveSkill* pPasSkill = M_VEC_EACH;
        pOwner->addPassiveSkill(pPasSkill, false);
        M_VEC_NEXT;
    }
}

void CItem::delSkillFromOwner()
{
    CUnit* o = getOwner();
    if (m_pActSkill != NULL)
    {
        o->delActiveSkill(m_iActSkillId, false);
    }
    
    M_VEC_FOREACH(m_vecPasSkills)
    {
        CPassiveSkill* pPasSkill = M_VEC_EACH;
        o->delPassiveSkill(pPasSkill->getId(), false);
        M_VEC_NEXT;
    }
}

