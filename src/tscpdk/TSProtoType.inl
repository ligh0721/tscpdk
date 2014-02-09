/* 
 * File:   TSProtoType.inl
 * Author: thunderliu
 *
 * Created on 2013年12月2日, 上午3:55
 */

#ifndef __TSPROTOTYPE_INL__
#define	__TSPROTOTYPE_INL__

#include "TSProtoType.h"



NS_TSCPDK_BEGIN

// CProtoTypeDump

inline void CProtoTypeDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
}

inline void CProtoTypeDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
}

inline void CProtoTypeDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
}

// CProtoTypeBase

inline CProtoTypeBase::CProtoTypeBase()
: m_iRef(0)
, m_bProtected(false)
, CONST_IS_AN_INTEGER(false)
{
}

inline CProtoTypeBase::CProtoTypeBase(bool bIsAnInteger)
: m_iRef(0)
, m_bProtected(false)
, CONST_IS_AN_INTEGER(bIsAnInteger)
{
}

inline CProtoTypeBase::~CProtoTypeBase()
{
}

inline void CProtoTypeBase::Init(const char* pName, const char* pTypeName)
{
    m_sName = pName;
    m_sTypeName = pTypeName;
}

inline void CProtoTypeBase::Release()
{
    if (m_iRef)
    {
        --m_iRef;
    }
    else
    {
        delete this;
    }
}

inline void CProtoTypeBase::Retain()
{
    ++m_iRef;
}

inline void CProtoTypeBase::ReleaseIfNotRef()
{
    if (!m_iRef)
    {
        delete this;
    }
}

inline const char* CProtoTypeBase::GetName() const
{
    return m_sName.c_str();
}

inline const char* CProtoTypeBase::GetTypeName() const
{
    return m_sTypeName.c_str();
}

inline void  CProtoTypeBase::Dump(CProtoTypeDump* pDump)
{
    pDump->OnDumpDefault(this);
}

inline void CProtoTypeBase::SetProtected(bool bProtected)
{
    m_bProtected = bProtected;
}

inline bool CProtoTypeBase::IsProtected() const
{
    return m_bProtected;
}

inline bool CProtoTypeBase::IsAnInteger() const
{
    return CONST_IS_AN_INTEGER;
}

inline size_t CProtoTypeBase::GetSize() const
{
    return 0;
}

// CProtoTypeStruct

inline CProtoTypeStruct::CProtoTypeStruct()
: m_pSwitchSymbol(NULL)
{
}

inline CProtoTypeStruct::~CProtoTypeStruct()
{
    for (PROTOTYPE_LIST::iterator it = m_vecMembers.begin(); it != m_vecMembers.end(); ++it)
    {
        (*it)->Release();
    }
}

inline CProtoTypeStruct* CProtoTypeStruct::Create(const char* pName, const char* pTypeName)
{
    CProtoTypeStruct* pRet = new CProtoTypeStruct();
    pRet->Init(pName, pTypeName);
    return pRet;
}

inline bool CProtoTypeStruct::AppendMember(CProtoTypeBase* pMember)
{
    unlikely (m_mapSymbols.find(pMember->GetName()) != m_mapSymbols.end())
    {
        fprintf(stderr, "ERR | append member err, member(%s) has been appended\n", pMember->GetName());
        return false;
    }
    
    pMember->Retain();
    m_mapSymbols.insert(make_pair(pMember->GetName(), pMember));
    m_vecMembers.push_back(pMember);
    
    return true;
}

inline PROTOTYPE_SYMBOLS& CProtoTypeStruct::GetSymbols()
{
    return m_mapSymbols;
}

inline int CProtoTypeStruct::GetNumberOfMembers() const
{
    return (int)m_vecMembers.size();
}

inline CProtoTypeBase* CProtoTypeStruct::GetMember(int iIndex) const
{
    return m_vecMembers[iIndex];
}

inline CProtoTypeBase* CProtoTypeStruct::GetMember(const char* pName) const
{
    PROTOTYPE_SYMBOLS::const_iterator it = m_mapSymbols.find(pName);
    likely (it != m_mapSymbols.end())
    {
        return it->second;
    }
    return NULL;
}

inline bool CProtoTypeStruct::SetSwitchSymbol(const char* pSymbol, CProtoTypeStruct* pLocalSymbols)
{
    if (m_pSwitchSymbol)
    {
        //m_pSwitchSymbol->SetProtected(false);
        m_pSwitchSymbol->Release();
    }
    
    m_pSwitchSymbol = pLocalSymbols->GetSymbol(pSymbol);
    if (m_pSwitchSymbol)
    {
        m_pSwitchSymbol->Retain();
        //m_pSwitchSymbol->SetProtected();
        return true;
    }
    
    fprintf(stderr, "ERR | struct switch symbol(%s) was not found\n", pSymbol);
    return false;
}

inline void CProtoTypeStruct::Dump(CProtoTypeDump* pDump)
{
    pDump->OnDumpStruct(this);
}

inline CProtoTypeBase* CProtoTypeStruct::GetSymbol(const char* pSymbol)
{
    // wLen
    char* pNext = const_cast<char*>(pSymbol);
    char* pStr = NULL;
    do
    {
        pStr = pNext;
        pNext = strchr(pStr, '.');
        if (pNext)
        {
            char szSymbol[128];
            memcpy(szSymbol, pStr, pNext - pStr);
            szSymbol[pNext - pStr] = 0; // stBody.stInfo.wLen
            ++pNext;
            return dynamic_cast<CProtoTypeStruct*>(GetMember(szSymbol))->GetSymbol(pNext);
        }
    } while (pNext);
    
    return GetMember(pSymbol);
}

// CProtoTypeArray

inline CProtoTypeArray::CProtoTypeArray(CProtoTypeBase* pSimpleElement)
: CONST_SIMPLE_ELEMENT(pSimpleElement)
, m_bIsConst(false)
, m_uSize(0)
, m_pArraySizeSymbol(NULL)
{
    CONST_SIMPLE_ELEMENT->Retain();
}

inline CProtoTypeArray::~CProtoTypeArray()
{
    CONST_SIMPLE_ELEMENT->Release();
    for (PROTOTYPE_LIST::iterator it = m_vecElements.begin(); it != m_vecElements.end(); ++it)
    {
        (*it)->Release();
    }
    if (m_pArraySizeSymbol)
    {
        m_pArraySizeSymbol->Release();
    }
}

inline CProtoTypeArray* CProtoTypeArray::Create(const char* pName, const char* pTypeName, CProtoTypeBase* pSimpleElement)
{
    CProtoTypeArray* pRet = new CProtoTypeArray(pSimpleElement);
    pRet->Init(pName, pTypeName);
    return pRet;
}

inline void CProtoTypeArray::AppendElement(CProtoTypeBase* pElement)
{
    pElement->Retain();
    m_vecElements.push_back(pElement);
}

inline void CProtoTypeArray::Dump(CProtoTypeDump* pDump)
{
    pDump->OnDumpArray(this);
}

inline bool CProtoTypeArray::SetArraySizeSymbol(const char* pSymbol, CProtoTypeStruct* pLocalSymbols)
{
    if (m_pArraySizeSymbol)
    {
        m_pArraySizeSymbol->SetProtected(false);
        m_pArraySizeSymbol->Release();
    }
    
    m_pArraySizeSymbol = pLocalSymbols->GetSymbol(pSymbol);
    if (m_pArraySizeSymbol)
    {
        m_pArraySizeSymbol->Retain();
        m_pArraySizeSymbol->SetProtected();
        m_sArraySizeSymbol = pSymbol;
        m_bIsConst = false;
        return true;
    }
    
    fprintf(stderr, "ERR | array size symbol(%s) was not found\n", pSymbol);
    return false;
}

inline const char* CProtoTypeArray::GetSizeSymbol() const
{
    return m_sArraySizeSymbol.c_str();
}

inline void CProtoTypeArray::SetArraySize(size_t uSize)
{
    m_uSize = uSize;
    m_bIsConst = true;
}

inline size_t CProtoTypeArray::GetArraySize() const
{
    if (m_bIsConst)
    {
        return m_uSize;
    }
    return m_vecElements.size();
}

inline bool CProtoTypeArray::IsArraySizeConst() const
{
    return m_bIsConst;
}

inline const CProtoTypeBase* CProtoTypeArray::GetSimpleElement() const
{
    return CONST_SIMPLE_ELEMENT;
}

// CProtoTypeGlobal

inline CProtoTypeGlobal::CProtoTypeGlobal()
{
}

inline CProtoTypeGlobal::~CProtoTypeGlobal()
{
}

inline CProtoTypeGlobal* CProtoTypeGlobal::Create(const char* pName, const char* pTypeName)
{
    CProtoTypeGlobal* pRet = new CProtoTypeGlobal();
    pRet->Init(pName, pTypeName);
    return pRet;
}


NS_TSCPDK_END


#endif	/* __TSPROTOTYPE_INL__ */

