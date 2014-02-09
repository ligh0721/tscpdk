/* 
 * File:   TSProtoType.h
 * Author: thunderliu
 *
 * Created on 2013年12月2日, 上午3:53
 */

#ifndef __TSPROTOTYPE_H__
#define	__TSPROTOTYPE_H__


#include <vector>
#include <map>
#include <string>
#include <string.h>
#include <stdint.h>
using namespace std;

NS_TSCPDK_BEGIN


class CProtoTypeBase;
class CProtoTypeArray;
class CProtoTypeStruct;

class TSCPDK_API CProtoTypeDump
{
public:
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CProtoTypeBase
{
public:
    CProtoTypeBase();
    CProtoTypeBase(bool bIsAnInteger);
    virtual ~CProtoTypeBase();
    
    void Init(const char* pName, const char* pTypeName);
    void Release();
    void Retain();
    void ReleaseIfNotRef();
    
    const char* GetName() const;
    const char* GetTypeName() const;
    
    virtual void Dump(CProtoTypeDump* pDump);
    
    void SetProtected(bool bProtected = true);
    bool IsProtected() const;
    
    virtual bool IsAnInteger() const;
    virtual size_t GetSize() const;
    
protected:
    int m_iRef;
    string m_sName;
    string m_sTypeName;
    bool m_bProtected;
    const bool CONST_IS_AN_INTEGER;
};

typedef vector<CProtoTypeBase*> PROTOTYPE_LIST;
typedef map<string, CProtoTypeBase*> PROTOTYPE_SYMBOLS;

#define DEF_PROTO_INTEGER_CLASS(name, type) \
class TSCPDK_API CProtoType ## name : public CProtoTypeBase \
{ \
public: \
    inline CProtoType ## name(): CProtoTypeBase(true), m_tValue(type()) {} \
    inline virtual ~CProtoType ## name () {} \
    \
    inline static CProtoType ## name* Create(const char* pName, const char* pTypeName = #type) \
    { \
        CProtoType ## name* pRet = new CProtoType ## name(); \
        pRet->Init(pName, pTypeName); \
        return pRet; \
    } \
    \
    virtual size_t GetSize() const \
    { \
        return sizeof(type); \
    } \
    \
protected: \
    type m_tValue; \
}

DEF_PROTO_INTEGER_CLASS(Int8, int8_t);
DEF_PROTO_INTEGER_CLASS(Uint8, uint8_t);

DEF_PROTO_INTEGER_CLASS(Int16, int16_t);
DEF_PROTO_INTEGER_CLASS(Uint16, uint16_t);

DEF_PROTO_INTEGER_CLASS(Int32, int32_t);
DEF_PROTO_INTEGER_CLASS(Uint32, uint32_t);

DEF_PROTO_INTEGER_CLASS(Int64, int64_t);
DEF_PROTO_INTEGER_CLASS(Uint64, uint64_t);

class TSCPDK_API CProtoTypeStruct : public CProtoTypeBase
{
public:
    CProtoTypeStruct();
    virtual ~CProtoTypeStruct();
    
    static CProtoTypeStruct* Create(const char* pName, const char* pTypeName);
    
    bool AppendMember(CProtoTypeBase* pMember);
    
    PROTOTYPE_SYMBOLS& GetSymbols();
    int GetNumberOfMembers() const;
    CProtoTypeBase* GetMember(int iIndex) const;
    CProtoTypeBase* GetMember(const char* pName) const;
    
    bool SetSwitchSymbol(const char* pSymbol, CProtoTypeStruct* pLocalSymbols);
    
    virtual void Dump(CProtoTypeDump* pDump);
    
    CProtoTypeBase* GetSymbol(const char* pSymbol);
        
protected:
    PROTOTYPE_LIST m_vecMembers;
    PROTOTYPE_SYMBOLS m_mapSymbols;
    CProtoTypeBase* m_pSwitchSymbol;
};

class TSCPDK_API CProtoTypeArray : public CProtoTypeBase
{
public:
    CProtoTypeArray(CProtoTypeBase* pSimpleElement);
    virtual ~CProtoTypeArray();
    
    static CProtoTypeArray* Create(const char* pName, const char* pTypeName, CProtoTypeBase* pSimpleElement);
    
    void AppendElement(CProtoTypeBase* pElement);
    
    void Dump(CProtoTypeDump* pDump);
    
    bool SetArraySizeSymbol(const char* pSymbol, CProtoTypeStruct* pLocalSymbols);
    const char* GetSizeSymbol() const;
    void SetArraySize(size_t uSize);
    size_t GetArraySize() const;
    
    bool IsArraySizeConst() const;
    
    const CProtoTypeBase* GetSimpleElement() const;
    
protected:
    CProtoTypeBase* const CONST_SIMPLE_ELEMENT;
    vector<CProtoTypeBase*> m_vecElements;
    bool m_bIsConst;
    
    // union
    size_t m_uSize;
    string m_sArraySizeSymbol;
    CProtoTypeBase* m_pArraySizeSymbol;
    
};

class TSCPDK_API CProtoTypeGlobal : public CProtoTypeStruct
{
public:
    CProtoTypeGlobal();
    virtual ~CProtoTypeGlobal();
    
    static CProtoTypeGlobal* Create(const char* pName = "", const char* pTypeName = "");
    
    void ParseLine(const char* pLine);
    CProtoTypeBase* ParseMember(const char* pMember, CProtoTypeStruct* pLocalSymbols);
    CProtoTypeBase* ParseArray(const char* pArray, CProtoTypeStruct* pLocalSymbols);
    
};

class TSCPDK_API CSourceHeaderMemberDataDump : public CProtoTypeDump
{
public:
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceHeaderMemberFunctionDump : public CProtoTypeDump
{
public:
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceHeaderStructDefineDump : public CProtoTypeDump
{
public:
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceHeaderGlobalDump : public CProtoTypeDump
{
public:
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceInlineMemberDataDump : public CProtoTypeDump
{
public:
    CSourceInlineMemberDataDump(int iCount);
    
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
    
protected:
    int m_iIndex;
    int m_iCount;
};

class TSCPDK_API CSourceInlineMemberDataInitDump : public CProtoTypeDump
{
public:
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceInlineMemberFunctionDump : public CProtoTypeDump
{
public:
    CSourceInlineMemberFunctionDump(const char* pStructName);
    
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
    
    static void ChangeSymbol(const char* pSymbol, char* pOutSymbol);
    
protected:
    string m_sStructName;
};

class TSCPDK_API CSourceInlineStructDefineDump : public CProtoTypeDump
{
public:
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceInlineSerializeDump : public CProtoTypeDump
{
public:
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceInlineParseDump : public CProtoTypeDump
{
public:
    virtual void OnDumpDefault(CProtoTypeBase* pProtoType);
    virtual void OnDumpArray(CProtoTypeArray* pProtoType);
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSourceInlineGlobalDump : public CProtoTypeDump
{
public:
    virtual void OnDumpStruct(CProtoTypeStruct* pProtoType);
};

class TSCPDK_API CSimpleProtoTypeCompiler
{
public:
    static bool Compile(const char* pInput, const char* pOutput);
    static char* MakeHeaderProtector(const char* pPath, char* pBuf, char* pFileName);
};

NS_TSCPDK_END

#include "TSProtoType.inl"

#endif	/* __TSPROTOTYPE_H__ */

