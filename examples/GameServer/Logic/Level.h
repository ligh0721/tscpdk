/* 
 * File:   Level.h
 * Author: thunderliu
 *
 * Created on 2013��12��8��, ����10:31
 */

#ifndef __LEVEL_H__
#define	__LEVEL_H__


class CLevelExp;

class CLevelUpdate
{
public:
    virtual void updateMaxExp(CLevelExp* pLevel) = 0; // @override
    virtual void onLevelChange(CLevelExp* pLevel, int iChanged) = 0; // @override
};

// �ȼ�����ֵ���������ȼ�����ֵ����
// ��Ҫ���� updateMaxExp���ṩ�ȼ��仯ʱ�������ֵ�����ʽ
// �ȼ��仯�󴥷� onLevelChange
class CLevelExp
{
public:
    CLevelExp();
    virtual ~CLevelExp();
    
    virtual void updateMaxExp(); // @override
    virtual void onChangeLevel(int iChanged); // @override
    void addExp(int iExp);
    void addLevel(int iLvl);
    virtual void setLevel(int iLvl);
    virtual void setMaxLevel(int iMaxLvl);
    
    int getLevel() const;
    int getMaxLevel() const;
    int getExp() const;
    int getMaxExp() const;
    
    void setLevelUpdate(CLevelUpdate* pUpdate);
    
public:
    int m_iLvl;
    int m_iMaxLvl;
    int m_iExp;
    int m_iMaxExp;
    CLevelUpdate* m_pUpdate;
    
};


#endif	/* __LEVEL_H__ */

