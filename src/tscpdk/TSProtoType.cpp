/* 
 * File:   TSProtoType.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月2日, 上午3:53
 */

#define LIBTSCPDK_SRC

#include <stdio.h>
#include "TSPlatform.h"
#include "TSProtoType.h"
#include "TSString.h"
#include "TSSocket.h"

NS_TSCPDK_BEGIN


// CProtoTypeGlobal

void CProtoTypeGlobal::ParseLine(const char* pLine)
{
    CProtoTypeStruct* pRet = NULL;
    const char* pStr = pLine;
    size_t uLen = 0;
    char szWord[128];
    //int i = 0;
    while (pStr)
    {
        pStr = CStringHelper::PickOutWord(pStr, 0, 0, " \t\r\n=+", " \t\r\n=+", NULL, &uLen);
        if (pStr)
        {
            memcpy(szWord, pStr, uLen);
            szWord[uLen] = 0;
            //fprintf(TS_STDOUT, "%d %d %s\n", i++, dwLen, szWord);
            pStr += uLen;

            if (!pRet)
            {
                // create struct
                char szTypeName[128];
                sprintf(szTypeName, "CType_%s", szWord);
                pRet = CProtoTypeStruct::Create(szWord, szTypeName);
            }
            else
            {
                // append member
                CProtoTypeBase* pMember = ParseMember(szWord, pRet);
                pRet->AppendMember(pMember);
            }
        }
    }

    AppendMember(pRet);
}

CProtoTypeBase* CProtoTypeGlobal::ParseMember(const char* pMember, CProtoTypeStruct* pLocalSymbols)
{
    // stHdr wLen cBuf[wLen] stInfo[2]

    // have [] or not ?
    if (strchr((char*)pMember, '['))
    {
        // cBuf[11] stInfo[wLen]
        return ParseArray(pMember, pLocalSymbols);
    }

    CProtoTypeBase* pRet = NULL;
    // stHdr wLen
    switch (pMember[0])
    {
    case 'c':
        pRet = CProtoTypeUint8::Create(pMember);
        break;

    case 'w':
        pRet = CProtoTypeUint16::Create(pMember);
        break;

    case 'd':
        if (pMember[1] == 'w')
        {
            pRet = CProtoTypeUint32::Create(pMember);
        }
        else if (pMember[1] == 'd' && pMember[2] == 'w')
        {
            pRet = CProtoTypeUint64::Create(pMember);
        }
        break;

    case 'q':
        likely(pMember[1] == 'w')
        {
            pRet = CProtoTypeUint64::Create(pMember);
        }
        break;

    case 's':
        likely(pMember[1] == 't')
        {
            if (strchr((char*)pMember, '('))
            {
                
            }
            else
            {
                pRet = GetMember(pMember);
                if (pRet) // found symbol in global
                {
                    // stInfo = wLen + stStr + stStr + stHdr + 
                    // found symbol
                }
                else
                {
                    fprintf(stderr, "ERR | parse member err, struct symbol(%s) was not found in the global\n", pMember);
                    return NULL;
                }
            }
        }
        break;
        
    default:
        fprintf(stderr, "ERR | parse member err, unknown symbol(%s) type\n", pMember);
        return NULL;
    }

    return pRet;
}

CProtoTypeBase* CProtoTypeGlobal::ParseArray(const char* pArray, CProtoTypeStruct* pLocalSymbols)
{
    // cBuf[wLen] stInfo[1]
    CProtoTypeArray* pRet = NULL;
    char szWord[128];
    size_t uLen = 0;
    const char* pStr = pArray;

    while (pStr)
    {
        char cSeq = 0;
        pStr = CStringHelper::PickOutWord(pStr, '[', ']', " \t[]", " \t[]", &cSeq, &uLen);
        if (pStr)
        {
            memcpy(szWord, pStr, uLen);
            szWord[uLen] = 0;
            pStr += uLen;
            if (cSeq != ']')
            {
                CProtoTypeBase* pTmp = ParseMember(szWord, pLocalSymbols);
                pRet = CProtoTypeArray::Create(szWord, pTmp->GetTypeName(), pTmp);
                pTmp->ReleaseIfNotRef();
            }
            else
            {
                // size
                memmove(szWord, szWord + 1, uLen);
                szWord[uLen - 2] = 0;
                int iLen = atoi(szWord);
                //pRet->SetArraySizeSymbol(szWord, pLocalSymbols);
                if (iLen)
                {
                    // const
                    pRet->SetArraySize(iLen);
                }
                else if (szWord[0] != '0')
                {
                    // var  wLen  <not impl ???!!!!!!!stInfo.wCount>
                    pRet->SetArraySizeSymbol(szWord, pLocalSymbols);
                }
                else
                {
                    // ERR
                }
            }
        }
    }

    return pRet;
}

// CSourceHeaderMemberDataDump

void CSourceHeaderMemberDataDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
    fprintf(TS_STDOUT, "    %s m_%s;\n", pProtoType->GetTypeName(), pProtoType->GetName());
}

void CSourceHeaderMemberDataDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    if (pProtoType->IsArraySizeConst())
    {
        fprintf(TS_STDOUT, "    %s m_a%s[%zu];\n", pProtoType->GetTypeName(), pProtoType->GetName(), pProtoType->GetArraySize());
    }
    else
    {
        fprintf(TS_STDOUT, "    std::vector<%s> m_a%s;\n", pProtoType->GetTypeName(), pProtoType->GetName());
    }

}

void CSourceHeaderMemberDataDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    OnDumpDefault(pProtoType);
}

// CSourceHeaderMemberFunctionDump

void CSourceHeaderMemberFunctionDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
    if (pProtoType->IsProtected())
    {
        fprintf(TS_STDOUT, "    const %s& %s() const;\n", pProtoType->GetTypeName(), pProtoType->GetName());
    }
    else
    {
        fprintf(TS_STDOUT, "    %s& %s();\n", pProtoType->GetTypeName(), pProtoType->GetName());
    }
}

void CSourceHeaderMemberFunctionDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    fprintf(TS_STDOUT, "    size_t GetNumberOf_a%s() const;\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "    %s& a%s(size_t uIndex);\n", pProtoType->GetTypeName(), pProtoType->GetName());
    fprintf(TS_STDOUT, "    %s* a%s();\n", pProtoType->GetTypeName(), pProtoType->GetName());
    if (pProtoType->IsArraySizeConst())
    {
        fprintf(TS_STDOUT, "    void Assign_a%s(const %s a%s[%zu]);\n", pProtoType->GetName(), pProtoType->GetTypeName(), pProtoType->GetName(), pProtoType->GetArraySize());
    }
    else
    {
        fprintf(TS_STDOUT, "    void Assign_a%s(const %s a%s[], size_t uNumberOfElements);\n", pProtoType->GetName(), pProtoType->GetTypeName(), pProtoType->GetName());
        fprintf(TS_STDOUT, "    %s& Add_%s();\n", pProtoType->GetTypeName(), pProtoType->GetName());
    }
}

void CSourceHeaderMemberFunctionDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    OnDumpDefault(pProtoType);
}

// CSourceHeaderStructDefineDump

void CSourceHeaderStructDefineDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    fprintf(TS_STDOUT, "class %s\n", pProtoType->GetTypeName());
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "public:\n");
    fprintf(TS_STDOUT, "    %s();\n", pProtoType->GetTypeName());
    fprintf(TS_STDOUT, "    ~%s();\n", pProtoType->GetTypeName());
    fprintf(TS_STDOUT, "\n");

    CSourceHeaderMemberFunctionDump oDumpFuncion;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpFuncion);
    }
    fprintf(TS_STDOUT, "\n");
    fprintf(TS_STDOUT, "    size_t SerializeToArray(void* pBuf, size_t uBufSize);\n");
    fprintf(TS_STDOUT, "    size_t ParseFromArray(const void* pBuf, size_t uBufSize);\n");
    fprintf(TS_STDOUT, "\n");
    fprintf(TS_STDOUT, "public:\n");
    CSourceHeaderMemberDataDump oDumpData;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpData);
    }
    string st;

    fprintf(TS_STDOUT, "};\n\n");
}

void CSourceHeaderGlobalDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    if (pProtoType->GetName()[0] != 0)
    {
        fprintf(TS_STDOUT, "namespace %s\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "{\n\n");
    }

    CSourceHeaderStructDefineDump oDumpDef;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpDef);
    }

    if (pProtoType->GetName()[0] != 0)
    {
        fprintf(TS_STDOUT, "};\n\n");
    }
}

// CSourceInlineMemberDataDump

CSourceInlineMemberDataDump::CSourceInlineMemberDataDump(int iCount)
: m_iIndex(0)
, m_iCount(iCount)
{
}

void CSourceInlineMemberDataDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
    if (!m_iIndex)
    {
        fprintf(TS_STDOUT, ": ");
    }
    else
    {
        fprintf(TS_STDOUT, ", ");
    }

    fprintf(TS_STDOUT, "m_%s(%s())", pProtoType->GetName(), pProtoType->GetTypeName());

    if (m_iIndex == m_iCount - 1)
    {
        fprintf(TS_STDOUT, "\n");
    }
    else
    {
        ++m_iIndex;
    }
}

void CSourceInlineMemberDataDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    if (m_iIndex == m_iCount - 1)
    {
        fprintf(TS_STDOUT, "\n");
    }
    else
    {
        ++m_iIndex;
    }
}

void CSourceInlineMemberDataDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    if (m_iIndex == m_iCount - 1)
    {
        fprintf(TS_STDOUT, "\n");
    }
    else
    {
        ++m_iIndex;
    }
}

// CSourceInlineMemberDataInitDump

void CSourceInlineMemberDataInitDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
}

void CSourceInlineMemberDataInitDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    if (pProtoType->IsArraySizeConst())
    {
        fprintf(TS_STDOUT, "    memset(m_a%s, 0, sizeof(m_a%s));\n", pProtoType->GetName(), pProtoType->GetName());
    }
}

void CSourceInlineMemberDataInitDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
}

// CSourceInlineMemberFunctionDump

inline CSourceInlineMemberFunctionDump::CSourceInlineMemberFunctionDump(const char* pStructName)
: m_sStructName(pStructName)
{
}

void CSourceInlineMemberFunctionDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
    if (pProtoType->IsProtected())
    {
        fprintf(TS_STDOUT, "inline const %s& %s::%s() const\n", pProtoType->GetTypeName(), m_sStructName.c_str(), pProtoType->GetName());
    }
    else
    {
        fprintf(TS_STDOUT, "inline %s& %s::%s()\n", pProtoType->GetTypeName(), m_sStructName.c_str(), pProtoType->GetName());
    }
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "    return m_%s;\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "}\n\n");
}

void CSourceInlineMemberFunctionDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    fprintf(TS_STDOUT, "inline size_t %s::GetNumberOf_a%s() const\n", m_sStructName.c_str(), pProtoType->GetName());
    fprintf(TS_STDOUT, "{\n");
    if (pProtoType->IsArraySizeConst())
    {
        fprintf(TS_STDOUT, "    return %zu;\n", pProtoType->GetArraySize());
    }
    else
    {
        fprintf(TS_STDOUT, "    return m_a%s.size();\n", pProtoType->GetName());
    }
    fprintf(TS_STDOUT, "}\n\n");

    fprintf(TS_STDOUT, "inline %s& %s::a%s(size_t uIndex)\n", pProtoType->GetTypeName(), m_sStructName.c_str(), pProtoType->GetName());
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "    assert(uIndex < GetNumberOf_a%s());\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "    return m_a%s[uIndex];\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "}\n\n");
    
    fprintf(TS_STDOUT, "inline %s* %s::a%s()\n", pProtoType->GetTypeName(), m_sStructName.c_str(), pProtoType->GetName());
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "    return &m_a%s[0];\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "}\n\n");

    if (pProtoType->IsArraySizeConst())
    {
        fprintf(TS_STDOUT, "inline void %s::Assign_a%s(const %s a%s[%zu])\n", m_sStructName.c_str(), pProtoType->GetName(), pProtoType->GetTypeName(), pProtoType->GetName(), pProtoType->GetArraySize());
        fprintf(TS_STDOUT, "{\n");
        if (pProtoType->GetName()[0] != 's' || pProtoType->GetName()[1] != 't')
        {
            fprintf(TS_STDOUT, "    memcpy(m_a%s, a%s, sizeof(m_a%s));\n", pProtoType->GetName(), pProtoType->GetName(), pProtoType->GetName());
        }
        else
        {
            fprintf(TS_STDOUT, "    for (size_t i = 0; i < %zu; ++i) \n", pProtoType->GetArraySize());
            fprintf(TS_STDOUT, "    {\n");
            fprintf(TS_STDOUT, "        m_a%s[i] = a%s[i];\n", pProtoType->GetName(), pProtoType->GetName());
            fprintf(TS_STDOUT, "    }\n");
        }
        
        fprintf(TS_STDOUT, "}\n\n");
    }
    else
    {
        char szOutSymbol[128];
        ChangeSymbol(pProtoType->GetSizeSymbol(), szOutSymbol);
        
        fprintf(TS_STDOUT, "inline void %s::Assign_a%s(const %s a%s[], size_t uNumberOfElements)\n", m_sStructName.c_str(), pProtoType->GetName(), pProtoType->GetTypeName(), pProtoType->GetName());
        fprintf(TS_STDOUT, "{\n");
        fprintf(TS_STDOUT, "    m_a%s.resize(uNumberOfElements);\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "    %s = m_a%s.size();\n", szOutSymbol, pProtoType->GetName()); // m_stBody.m_stInfo.wLen
        if (pProtoType->GetName()[0] != 's' || pProtoType->GetName()[1] != 't')
        {
            fprintf(TS_STDOUT, "    memcpy(&m_a%s[0], a%s, uNumberOfElements * sizeof(%s));\n", pProtoType->GetName(), pProtoType->GetName(), pProtoType->GetTypeName());
        }
        else
        {
            fprintf(TS_STDOUT, "    for (size_t i = 0; i < uNumberOfElements; ++i) \n");
            fprintf(TS_STDOUT, "    {\n");
            fprintf(TS_STDOUT, "        m_a%s[i] = a%s[i];\n", pProtoType->GetName(), pProtoType->GetName());
            fprintf(TS_STDOUT, "    }\n");
        }
        
        fprintf(TS_STDOUT, "}\n\n");
        
        fprintf(TS_STDOUT, "inline %s& %s::Add_%s()\n", pProtoType->GetTypeName(), m_sStructName.c_str(), pProtoType->GetName());
        fprintf(TS_STDOUT, "{\n");
        fprintf(TS_STDOUT, "    m_a%s.push_back(%s());\n", pProtoType->GetName(), pProtoType->GetTypeName());
        fprintf(TS_STDOUT, "    %s = m_a%s.size();\n", szOutSymbol, pProtoType->GetName()); // m_stBody.m_stInfo.wLen
        fprintf(TS_STDOUT, "    return m_a%s.back();\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "}\n\n");
    }

}

void CSourceInlineMemberFunctionDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    OnDumpDefault(pProtoType);
}

void CSourceInlineMemberFunctionDump::ChangeSymbol(const char* pSymbol, char* pOutSymbol)
{
    // stBody.stInfo.wLen
    memcpy(pOutSymbol, "m_", 2);
    pOutSymbol += 2;
    do
    {
        *pOutSymbol = *pSymbol;

        ++pOutSymbol;

        if (*pSymbol == '.')
        {
            memcpy(pOutSymbol, "m_", 2);
            pOutSymbol += 2;
        }

        ++pSymbol;

    }
    while (*pSymbol);
    *pOutSymbol = *pSymbol;
}

// CSourceInlineStructDefineDump

void CSourceInlineStructDefineDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    fprintf(TS_STDOUT, "inline %s::%s()\n", pProtoType->GetTypeName(), pProtoType->GetTypeName());

    CSourceInlineMemberDataDump oDumpData(pProtoType->GetNumberOfMembers());
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpData);
    }

    fprintf(TS_STDOUT, "{\n");
    CSourceInlineMemberDataInitDump oDumpDataInit;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpDataInit);
    }
    fprintf(TS_STDOUT, "}\n\n");

    fprintf(TS_STDOUT, "inline %s::~%s()\n", pProtoType->GetTypeName(), pProtoType->GetTypeName());
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "}\n\n");

    CSourceInlineMemberFunctionDump oDumpFuncion(pProtoType->GetTypeName());
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpFuncion);
    }

    fprintf(TS_STDOUT, "inline size_t %s::SerializeToArray(void* pBuf, size_t uBufSize)\n", pProtoType->GetTypeName());
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "    size_t uSize = 0;\n");
    fprintf(TS_STDOUT, "    char* pPos = (char*)pBuf + uSize;\n\n"); // for no warning

    CSourceInlineSerializeDump oDumpSerialize;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpSerialize);
    }

    fprintf(TS_STDOUT, "    return (size_t)pPos - (size_t)pBuf;\n");
    fprintf(TS_STDOUT, "}\n\n");

    fprintf(TS_STDOUT, "inline size_t %s::ParseFromArray(const void* pBuf, size_t uBufSize)\n", pProtoType->GetTypeName());
    fprintf(TS_STDOUT, "{\n");
    fprintf(TS_STDOUT, "    size_t uSize = 0;\n");
    fprintf(TS_STDOUT, "    const char* pPos = (const char*)pBuf + uSize;\n\n"); // for no warning

    CSourceInlineParseDump oDumpParse;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpParse);
    }

    fprintf(TS_STDOUT, "    return (size_t)pPos - (size_t)pBuf;\n");
    fprintf(TS_STDOUT, "}\n\n");
}

// CSourceInlineSerializeDump

void CSourceInlineSerializeDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
    if (pProtoType->IsAnInteger())
    {
        switch (pProtoType->GetSize())
        {
        case sizeof (uint8_t):
            fprintf(TS_STDOUT, "    *(%s*)pPos = m_%s;\n", pProtoType->GetTypeName(), pProtoType->GetName());
            break;

        case sizeof (uint16_t):
            fprintf(TS_STDOUT, "    *(%s*)pPos = htons(m_%s);\n", pProtoType->GetTypeName(), pProtoType->GetName());
            break;

        case sizeof (uint32_t):
            fprintf(TS_STDOUT, "    *(%s*)pPos = htonl(m_%s);\n", pProtoType->GetTypeName(), pProtoType->GetName());
            break;

        case sizeof (uint64_t):
            fprintf(TS_STDOUT, "    *(%s*)pPos = htonl64(m_%s);\n", pProtoType->GetTypeName(), pProtoType->GetName());
            break;

        default:
            fprintf(TS_STDOUT, "    memcpy(pPos, &m_%s, sizeof(m_%s));\n", pProtoType->GetName(), pProtoType->GetName());
        }
    }
    else
    {
        fprintf(TS_STDOUT, "    memcpy(pPos, &m_%s, sizeof(m_%s));\n", pProtoType->GetName(), pProtoType->GetName());
    }

    fprintf(TS_STDOUT, "    pPos += sizeof(m_%s);\n\n", pProtoType->GetName());
}

void CSourceInlineSerializeDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    if (pProtoType->GetName()[0] != 's' || pProtoType->GetName()[1] != 't')
    {
        if (pProtoType->GetSimpleElement()->IsAnInteger())
        {
            switch (pProtoType->GetSimpleElement()->GetSize())
            {
            case sizeof (uint16_t):
                fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    {\n");
                fprintf(TS_STDOUT, "        *(%s*)pPos = htons(m_a%s[i]);\n", pProtoType->GetTypeName(), pProtoType->GetName());
                fprintf(TS_STDOUT, "        pPos += sizeof(m_a%s[i]);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    }\n\n");
                break;

            case sizeof (uint32_t):
                fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    {\n");
                fprintf(TS_STDOUT, "        *(%s*)pPos = htonl(m_a%s[i]);\n", pProtoType->GetTypeName(), pProtoType->GetName());
                fprintf(TS_STDOUT, "        pPos += sizeof(m_a%s[i]);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    }\n\n");
                break;

            case sizeof (uint64_t):
                fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    {\n");
                fprintf(TS_STDOUT, "        *(%s*)pPos = htonl64(m_a%s[i]);\n", pProtoType->GetTypeName(), pProtoType->GetName());
                fprintf(TS_STDOUT, "        pPos += sizeof(m_a%s[i]);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    }\n\n");
                break;

            default:
                fprintf(TS_STDOUT, "    uSize = GetNumberOf_a%s() * sizeof(%s);\n", pProtoType->GetName(), pProtoType->GetTypeName());
                fprintf(TS_STDOUT, "    memcpy(pPos, &m_a%s[0], uSize);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    pPos += uSize;\n\n");
            }
        }
        else
        {
            fprintf(TS_STDOUT, "    uSize = GetNumberOf_a%s() * sizeof(%s);\n", pProtoType->GetName(), pProtoType->GetTypeName());
            fprintf(TS_STDOUT, "    memcpy(pPos, &m_a%s[0], uSize);\n", pProtoType->GetName());
            fprintf(TS_STDOUT, "    pPos += uSize;\n\n");
        }

    }
    else
    {
        fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "    {\n");
        fprintf(TS_STDOUT, "        uSize = m_a%s[i].SerializeToArray(pPos, uBufSize - (size_t)pPos + (size_t)pBuf);\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "        pPos += uSize;\n");
        fprintf(TS_STDOUT, "    }\n\n");
    }
}

void CSourceInlineSerializeDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    fprintf(TS_STDOUT, "    uSize = m_%s.SerializeToArray(pPos, uBufSize - (size_t)pPos + (size_t)pBuf);\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "    pPos += uSize;\n\n");
}

// CSourceInlineParseDump

void CSourceInlineParseDump::OnDumpDefault(CProtoTypeBase* pProtoType)
{
    if (pProtoType->IsAnInteger())
    {
        switch (pProtoType->GetSize())
        {
        case sizeof (uint8_t):
            fprintf(TS_STDOUT, "    m_%s = *(%s*)pPos;\n", pProtoType->GetName(), pProtoType->GetTypeName());
            break;

        case sizeof (uint16_t):
            fprintf(TS_STDOUT, "    m_%s = ntohs(*(%s*)pPos);\n", pProtoType->GetName(), pProtoType->GetTypeName());
            break;

        case sizeof (uint32_t):
            fprintf(TS_STDOUT, "    m_%s = ntohl(*(%s*)pPos);\n", pProtoType->GetName(), pProtoType->GetTypeName());
            break;

        case sizeof (uint64_t):
            fprintf(TS_STDOUT, "    m_%s = ntohl64(*(%s*)pPos);\n", pProtoType->GetName(), pProtoType->GetTypeName());
            break;

        default:
            fprintf(TS_STDOUT, "    memcpy(&m_%s, pPos, sizeof(m_%s));\n", pProtoType->GetName(), pProtoType->GetName());
        }
    }
    else
    {
        fprintf(TS_STDOUT, "    memcpy(&m_%s, pPos, sizeof(m_%s));\n", pProtoType->GetName(), pProtoType->GetName());
    }
    fprintf(TS_STDOUT, "    pPos += sizeof(m_%s);\n\n", pProtoType->GetName());
}

void CSourceInlineParseDump::OnDumpArray(CProtoTypeArray* pProtoType)
{
    char szOutSymbol[128];
    if (!pProtoType->IsArraySizeConst())
    {
        CSourceInlineMemberFunctionDump::ChangeSymbol(pProtoType->GetSizeSymbol(), szOutSymbol);
        fprintf(TS_STDOUT, "    m_a%s.resize(%s);\n", pProtoType->GetName(), szOutSymbol); // m_stBody.m_stInfo.wLen
    }

    if (pProtoType->GetName()[0] != 's' || pProtoType->GetName()[1] != 't')
    {
        if (pProtoType->GetSimpleElement()->IsAnInteger())
        {
            switch (pProtoType->GetSimpleElement()->GetSize())
            {
            case sizeof (uint16_t):
                fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    {\n");
                fprintf(TS_STDOUT, "        m_a%s[i] = ntohs(*(%s*)pPos);\n", pProtoType->GetName(), pProtoType->GetTypeName());
                fprintf(TS_STDOUT, "        pPos += sizeof(m_a%s[i]);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    }\n\n");
                break;

            case sizeof (uint32_t):
                fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    {\n");
                fprintf(TS_STDOUT, "        m_a%s[i] = ntohl(*(%s*)pPos);\n", pProtoType->GetName(), pProtoType->GetTypeName());
                fprintf(TS_STDOUT, "        pPos += sizeof(m_a%s[i]);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    }\n\n");
                break;

            case sizeof (uint64_t):
                fprintf(TS_STDOUT, "    for (size_t i = 0; i < GetNumberOf_a%s(); ++i)\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    {\n");
                fprintf(TS_STDOUT, "        m_a%s[i] = ntohl64(*(%s*)pPos);\n", pProtoType->GetName(), pProtoType->GetTypeName());
                fprintf(TS_STDOUT, "        pPos += sizeof(m_a%s[i]);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    }\n\n");
                break;

            default:
                fprintf(TS_STDOUT, "    uSize = GetNumberOf_a%s() * sizeof(%s);\n", pProtoType->GetName(), pProtoType->GetTypeName());
                fprintf(TS_STDOUT, "    memcpy(&m_a%s[0], pPos, uSize);\n", pProtoType->GetName());
                fprintf(TS_STDOUT, "    pPos += uSize;\n\n");
            }
        }
        else
        {
            fprintf(TS_STDOUT, "    uSize = GetNumberOf_a%s() * sizeof(%s);\n", pProtoType->GetName(), pProtoType->GetTypeName());
            fprintf(TS_STDOUT, "    memcpy(&m_a%s[0], pPos, uSize);\n", pProtoType->GetName());
            fprintf(TS_STDOUT, "    pPos += uSize;\n\n");
        }
    }
    else
    {
        fprintf(TS_STDOUT, "    for (size_t i = 0; i < ");
        if (pProtoType->IsArraySizeConst())
        {
            fprintf(TS_STDOUT, "%zu", pProtoType->GetArraySize());
        }
        else
        {
            fprintf(TS_STDOUT, "m_a%s.size()", pProtoType->GetName());
        }
        fprintf(TS_STDOUT, "; ++i)\n");
        fprintf(TS_STDOUT, "    {\n");
        fprintf(TS_STDOUT, "        uSize = m_a%s[i].ParseFromArray(pPos, uBufSize - (size_t)pPos + (size_t)pBuf);\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "        pPos += uSize;\n");
        fprintf(TS_STDOUT, "    }\n\n");
    }
}

void CSourceInlineParseDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    fprintf(TS_STDOUT, "    uSize = m_%s.ParseFromArray(pPos, uBufSize - (size_t)pPos + (size_t)pBuf);\n", pProtoType->GetName());
    fprintf(TS_STDOUT, "    pPos += uSize;\n\n");
}

// CSourceInlineGlobalDump

void CSourceInlineGlobalDump::OnDumpStruct(CProtoTypeStruct* pProtoType)
{
    if (pProtoType->GetName()[0] != 0)
    {
        fprintf(TS_STDOUT, "namespace %s\n", pProtoType->GetName());
        fprintf(TS_STDOUT, "{\n\n");
    }

    fprintf(TS_STDOUT,
            "#ifndef ___constant_swab64\n"
            "#define ___constant_swab64(x) \\\n"
            "    ((uint64_t)( \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \\\n"
            "    (uint64_t)(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56) ))\n"
            "#endif // ___constant_swab64\n"
            "\n"
            "inline uint64_t htonl64(uint64_t host)\n"
            "{\n"
            "#if __BYTE_ORDER == __LITTLE_ENDIAN\n"
            "    return ___constant_swab64(host);\n"
            "#else\n"
            "    return host;\n"
            "#endif\n"
            "}\n"
            "\n"
            "inline uint64_t ntohl64(uint64_t net)\n"
            "{\n"
            "#if __BYTE_ORDER == __LITTLE_ENDIAN\n"
            "    return ___constant_swab64(net);\n"
            "#else\n"
            "    return host;\n"
            "#endif\n"
            "}\n"
            "\n"
            );
    CSourceInlineStructDefineDump oDumpDef;
    for (int i = 0; i < pProtoType->GetNumberOfMembers(); ++i)
    {
        pProtoType->GetMember(i)->Dump(&oDumpDef);
    }

    if (pProtoType->GetName()[0] != 0)
    {
        fprintf(TS_STDOUT, "};\n\n");
    }
}

// CProtoTypeCompiler

bool CSimpleProtoTypeCompiler::Compile(const char* pInput, const char* pOutput)
{
    char szHdrPr[256] = {};
    char szFileName[256] = {};

    FILE* pInFile = fopen(pInput, "r");

    unlikely(!pInFile)
    {
        perror("ERR ");
        return false;
    }

    MakeHeaderProtector(pInput, szHdrPr, szFileName);
    char* pPos = strchr(szFileName, '.');
    if (pPos)
    {
        *pPos = 0;
    }
    CProtoTypeGlobal* pGlobal = CProtoTypeGlobal::Create(szFileName, "TSProtoTest.hpp");

    char szLine[1024];
    char* pStr = NULL;
    size_t uLen = 0;
    while (!feof(pInFile))
    {
        pStr = fgets(szLine, sizeof (szLine), pInFile);
        pStr = const_cast<char*>(CStringHelper::PickOutWord(pStr, 0, 0, " \t\r\n", " \t\r\n", NULL, &uLen));
        if (pStr)
        {
            pGlobal->ParseLine(pStr);
        }
    }
    fclose(pInFile);

    FILE* pOrgOut = TS_STDOUT;
    FILE* pOutFile = NULL;
    if (pOutput)
    {
        pOutFile = fopen(pOutput, "w");

        unlikely(!pOutFile)
        {
            perror("ERR ");
            return false;
        }
        TS_STDOUT = pOutFile;
    }

    MakeHeaderProtector(pOutput ? pOutput : pInput, szHdrPr, szFileName);
    fprintf(TS_STDOUT, "// Generated by the ProtoType compiler.  DO NOT EDIT!\n");
    fprintf(TS_STDOUT, "// File:      %s\n", szFileName);

    time_t uTm = time(NULL);
    struct tm stTm;
    localtime_r(&uTm, &stTm);
    char szTm[128];
    asctime_r(&stTm, szTm);

    fprintf(TS_STDOUT, "// Create on: %s", szTm);
    fprintf(TS_STDOUT, "//\n");
    fprintf(TS_STDOUT, "\n");
    fprintf(TS_STDOUT, "#ifndef %s\n", szHdrPr);
    fprintf(TS_STDOUT, "#define %s\n\n", szHdrPr);
    fprintf(TS_STDOUT, "#include <stdint.h>\n");
    fprintf(TS_STDOUT, "#include <string.h>\n");
    fprintf(TS_STDOUT, "#include <vector>\n");
    fprintf(TS_STDOUT, "\n");

    CSourceHeaderGlobalDump oDmpHdr;
    pGlobal->Dump(&oDmpHdr);

    fprintf(TS_STDOUT, "\n\n///////////////// - Inline Implementation - /////////////////\n\n");
    CSourceInlineGlobalDump oDmpInl;
    pGlobal->Dump(&oDmpInl);

    fprintf(TS_STDOUT, "#endif  //  %s\n", szHdrPr);

    if (pOutput)
    {
        TS_STDOUT = pOrgOut;
        fclose(pOutFile);
    }

    pGlobal->Release();

    return true;
}

char* CSimpleProtoTypeCompiler::MakeHeaderProtector(const char* pPath, char* pBuf, char* pFileName)
{
    // /home
    const char* pPos;
    for (pPos = pPath + strlen(pPath) - 1; *pPos != '/' && *pPos != '\\' && pPos != pPath; --pPos);
    if (*pPos == '/' || *pPos == '\\')
    {
        ++pPos;
    }

    if (pFileName)
    {
        strcpy(pFileName, pPos);
    }

    memcpy(pBuf, "__", 2);
    pBuf += 2;
    do
    {
        *pBuf = ((*pPos == '.' || *pPos == ' ') ? '_' : toupper(*pPos));

        ++pBuf;
        ++pPos;

    }
    while (*pPos);
    memcpy(pBuf, "__", 3);

    return pBuf;
}

NS_TSCPDK_END
