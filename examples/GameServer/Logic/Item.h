/* 
 * File:   Item.h
 * Author: thunderliu
 *
 * Created on 2014年2月11日, 上午1:08
 */

#ifndef ITEM_H
#define	ITEM_H


#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"
#include "Skill.h"


class CPackage;
class CSkill;
class CPassSkill;
class CActiveSkill;

class CItem : public CMultiRefObject
{
protected:
    const string CONST_ROOT_ID;
    
public:
    enum ITEM_TYPE
    {
        kNormal,
        kEquipment,
        kConsumable
    };
    
public:
    // iMaxStackSize可以取INFINITE，无限堆叠；eItemType为kConsumable时，iMaxStackSize可以取0，代表拾起立即消耗
    CItem(const char* pRootId, const char* pName, ITEM_TYPE eItemType, int iMaxStackSize);
    virtual ~CItem();
    
    const char* getRootId() const;
    M_SYNTHESIZE_STR(Name);
    
    M_SYNTHESIZE(ITEM_TYPE, m_eItemType, ItemType);
    
    M_SYNTHESIZE_BOOL(Equipped);
    M_SYNTHESIZE(int, m_iStackCount, StackCount);
    M_SYNTHESIZE(int, m_iMaxStackSize, MaxStackSize);
    void addStackCount();
    
protected:
    typedef vector<int> VEC_PAS_SKILLIDS;
    int m_iActSkillId;
    VEC_PAS_SKILLIDS m_vecPasSkillIds;
    
public:
    typedef CMultiRefVec<CPassiveSkill*> VEC_PAS_SKILLS;
    M_SYNTHESIZE_READONLY(CActiveSkill*, m_pActSkill, ActiveSkill);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_PAS_SKILLS, m_vecPasSkills, PassiveSkills);
    void addPassiveSkill(int id);
    M_SYNTHESIZE(CUnit*, m_pOwner, Owner);
    
    // @overide
    virtual void onUnitAddItem();
    virtual void onUnitDelItem();
    
    // 调用完之后
    virtual bool use();
    virtual void onUnitUseItem();
    
    // 外部调用
    void onAddToNewSlot(CUnit* pOwner, bool bNotify = true);
    void onDelFromSlot(bool  bNotify = true);
    
protected:
    void addSkillToOwner(CUnit* pOwner);
    void delSkillFromOwner();
    
};

class CPackage : public CMultiRefObject
{
public:
};

#endif	/* ITEM_H */

