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
CItem::CItem(const char* pRootId, const char* pName)
: CONST_ROOT_ID(pRootId)
, m_sName(pName)
{
}

CItem::~CItem()
{
}

