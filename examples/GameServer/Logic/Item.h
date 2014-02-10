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


class CItem : public CMultiRefObject
{
protected:
    const string CONST_ROOT_ID;
    
public:
    CItem(const char* pRootId, const char* pName);
    virtual ~CItem();
    
    M_SYNTHESIZE_STR(Name);
};

#endif	/* ITEM_H */

