/* 
* File:   TSMemory.cpp
* Author: thunderliu
* 
* Created on 2012年4月16日, 下午4:29
*/

#define LIBTSCPDK_SRC

#include "TSPlatform.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "TSMemory.h"


NS_TSCPDK_BEGIN


    // CMemoryStream

    size_t CMemoryStream::Write(const void* pData, size_t uSize, size_t uCount)
{
    unlikely(!uSize)
    {
        return 0;
    }

    size_t uRet = (m_uSize - m_uPos) / uSize;
    likely(uCount < uRet)
    {
        uRet = uCount;
    }

    size_t uWriteSize = uSize * uRet;
    memmove((char*)m_pAddr + m_uPos, pData, uWriteSize);
    m_uPos += uWriteSize;

    return uRet;
}

size_t CMemoryStream::Read(void* pData, size_t uSize, size_t uCount)
{
    unlikely(!uSize)
    {
        return 0;
    }

    size_t uRet = (m_uSize - m_uPos) / uSize;
    likely(uCount < uRet)
    {
        uRet = uCount;
    }

    size_t uReadSize = uSize * uRet;
    memmove(pData, (char*)m_pAddr + m_uPos, uReadSize);
    m_uPos += uReadSize;

    return uRet;
}

// CShmStream

bool CShmStream::Init(const char* pName, size_t uSize)
{
#if !defined(TSCPDK_MSVC)
    int iFd = shm_open(pName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH); // 0664
    unlikely(iFd < 0)
    {
        return false;
    }

    ftruncate64(iFd, uSize);

    void* pAddr = mmap64(NULL, uSize, PROT_READ | PROT_WRITE, MAP_SHARED, iFd, 0);
    close(iFd);

    unlikely(pAddr == MAP_FAILED)
    {
        return false;
    }

#else
    char szName[1024];
    sprintf(szName, "Global\\shm_%s", pName);
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,   // Use paging file - shared memory
        NULL,                   // Default security attributes
        PAGE_READWRITE,         // Allow read and write access
        0,                      // High-order DWORD of file mapping max size
        uSize,                  // Low-order DWORD of file mapping max size
        szName                  // Name of the file mapping object
        );
    unlikely(!hMapFile)
    {
        return false;
    }

    void* pAddr = MapViewOfFile(
        hMapFile,                           // Handle of the map object
        FILE_MAP_ALL_ACCESS,                // Read and write access
        0,                                  // High-order DWORD of the file offset 
        0,                                  // Low-order DWORD of the file offset 
        uSize                               // The number of bytes to map to view
        );
    CloseHandle(hMapFile);

    unlikely(!pAddr)
    {
        return false;
    }

#endif
    CMemoryStream::Attach(pAddr, uSize);

    return true;
}

bool CShmStream::Attach(const char* pName, size_t uSize)
{
#if !defined(TSCPDK_MSVC)
    int iFd = shm_open(pName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH); // 0664
    unlikely(iFd < 0)
    {
        return false;
    }

    struct stat64 stFi = {};
    fstat64(iFd, &stFi);

    uSize = stFi.st_size;
    void* pAddr = mmap64(NULL, uSize, PROT_READ | PROT_WRITE, MAP_SHARED, iFd, 0);
    close(iFd);

    unlikely(pAddr == MAP_FAILED)
    {
        return false;
    }

#else
    char szName[1024];
    sprintf(szName, "Global\\shm_%s", pName);
    
    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,    // Allow read and write access
        FALSE,
        szName                  // Name of the file mapping object
        );

    unlikely(!hMapFile)
    {
        return false;
    }

    void* pAddr = MapViewOfFile(
        hMapFile,                           // Handle of the map object
        FILE_MAP_READ | FILE_MAP_WRITE,     // Read and write access
        0,                                  // High-order DWORD of the file offset 
        0,                                  // Low-order DWORD of the file offset 
        uSize                               // The number of bytes to map to view
        );
    CloseHandle(hMapFile);

    unlikely(!pAddr)
    {
        return false;
    }
#endif

    CMemoryStream::Attach(pAddr, uSize);

    return true;
}


NS_TSCPDK_END
