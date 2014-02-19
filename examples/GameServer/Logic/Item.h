/* 
 * File:   Item.h
 * Author: thunderliu
 *
 * Created on 2014��2��11��, ����1:08
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
    // uMaxStackSize����ȡINFINITE�����޶ѵ���eItemTypeΪkConsumableʱ��uMaxStackSize����ȡ0������ʰ����������
    CItem(const char* pRootId, const char* pName, ITEM_TYPE eItemType, unsigned int uMaxStackSize);
    virtual ~CItem();

    virtual const char* getDbgTag() const;

    const char* getRootId() const;
    M_SYNTHESIZE_STR(Name);
    
    M_SYNTHESIZE(ITEM_TYPE, m_eItemType, ItemType);
    
    M_SYNTHESIZE_BOOL(Equipped);
    M_SYNTHESIZE(unsigned int, m_uStackCount, StackCount);
    M_SYNTHESIZE(unsigned int, m_uMaxStackSize, MaxStackSize);
    unsigned int getFreeStackSize() const;
    unsigned int incStackCount(unsigned int uIncrease);
    unsigned int decStatckCount(unsigned int uDecrease);
    
protected:
    typedef vector<int> VEC_SKILLIDS;
    VEC_SKILLIDS m_vecActSkillIds;
    VEC_SKILLIDS m_vecPasSkillIds;
    
public:
    typedef CMultiRefVec<CActiveSkill*> VEC_ACT_SKILLS;
    typedef CMultiRefVec<CPassiveSkill*> VEC_PAS_SKILLS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ACT_SKILLS, m_vecActSkills, ActiveSkills);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_PAS_SKILLS, m_vecPasSkills, PassiveSkills);
    void addActiveSkill(int id);
    void addPassiveSkill(int id);
    M_SYNTHESIZE(CUnit*, m_pOwner, Owner);
    
    // @overide
    virtual void onUnitAddItem();
    virtual void onUnitDelItem();
    
    // ������֮��
    virtual bool use();
    virtual bool checkConditions();
    virtual void onUnitUseItem();
    
    // �ⲿ����
    void onAddToNewSlot(CUnit* pOwner);
    void onDelFromSlot();
    
protected:
    void addSkillToOwner(CUnit* pOwner, bool bNotify = false);
    void delSkillFromOwner(bool bNotify = false);
    
};

class CPackage : public CMultiRefObject
{
public:
};

#endif	/* ITEM_H */

