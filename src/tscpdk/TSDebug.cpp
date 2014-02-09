/* 
 * File:   TSDebug.cpp
 * Author: thunderliu
 * 
 * Created on 2011年12月29日, 上午11:32
 */

#define LIBTSCPDK_SRC

#include "TSPlatform.h"
#include <stdio.h>
#include <stdint.h>
#include "TSDebug.h"

NS_TSCPDK_BEGIN

// CDebug

void CDebug::SimpleDump(const void* pBuf, size_t uSize, uint32_t dwDumpFlags)
{
    if (dwDumpFlags & kShowOffset)
    {
        fprintf(CLog::m_pFile, "%08zX  ", (size_t)0);
    }
    
    size_t uEnd = ((uSize + 0x0F) / 0x10) * 0x10;
    
    for (size_t i = 0; i < uEnd; ++i)
    {
        if (i < uSize)
        {
            fprintf(CLog::m_pFile, "%02X ", uint32_t(*((uint8_t*)pBuf + i)));
        }
        else
        {
            fprintf(CLog::m_pFile, "   ");
        }
        
        
        if (i % 0x10 == 0x0F)
        {
            if ((dwDumpFlags & kShowChar) && i)
            {
                fprintf(CLog::m_pFile, " |");
                for (size_t j = i - 0x0F ; j <= i; ++j)
                {
                    if (j >= uSize)
                    {
                        break;
                    }
                    fprintf(CLog::m_pFile, "%c", (((char*)pBuf)[j] < 32 || ((char*)pBuf)[j] > 126) ? '.' : ((char*)pBuf)[j]);
                }
                fprintf(CLog::m_pFile, "|");
            }
            
            if (i < uSize - 1)
            {
                fprintf(CLog::m_pFile, "\n");
                if ((dwDumpFlags & kShowOffset))
                {
                    fprintf(CLog::m_pFile, "%08zX  ", i + 1);
                }
            }

        }
        else if (i % 0x08 == 0x07)
        {
            fprintf(CLog::m_pFile, " ");
        }
    }
    
    fprintf(CLog::m_pFile, "\n");
    if (dwDumpFlags & kShowOffset)
    {
        fprintf(CLog::m_pFile, "%08zX\n", uSize);
    }
}

// CLog

E_LOG_LVL CLog::m_eLogLv = E_LL_ERR;
FILE* CLog::m_pFile = stderr;
//const char* CLog::m_pFuncName = __FUNCTION__;

NS_TSCPDK_END
