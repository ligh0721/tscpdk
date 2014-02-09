/* 
 * File:   TSMemory.inl
 * Author: thunderliu
 *
 * Created on 2012年4月16日, 下午4:30
 */

#ifndef __TSMEMORY_INL__
#define	__TSMEMORY_INL__

#include "TSMemory.h"


//#include "TSMemory.h"

NS_TSCPDK_BEGIN

// CBuffer

inline CBuffer::CBuffer(uint32_t dwSize)
: m_dwSize(dwSize)
{
    m_pBuf = (uint8_t*)malloc(dwSize);
}

inline CBuffer::~CBuffer()
{
    if (m_pBuf)
    {
        free(m_pBuf);
    }
}

inline void* CBuffer::GetBuffer(uint32_t dwPos)
{
    return m_pBuf + dwPos;
}

inline uint32_t CBuffer::GetBufferSize() const
{
    return m_dwSize;
}


// CMemoryStream

inline CMemoryStream::CMemoryStream()
{
}

inline CMemoryStream::CMemoryStream(void* pAddr, size_t uSize)
{
    Attach(pAddr, uSize);
}

inline void CMemoryStream::Attach(void* pAddr, size_t uSize)
{
    m_pAddr = pAddr;
    m_uPos = 0;
    m_uSize = uSize;
}

inline void* CMemoryStream::Read(size_t uSize)
{
    size_t uNewPos = m_uPos + uSize;
    unlikely(uNewPos > m_uSize)
    {
        return NULL;
    }
    
    m_uPos = uNewPos;
    return (char*)m_pAddr + m_uPos;
}

inline void* CMemoryStream::GetAddress(size_t uOffset)
{
    return (char*)m_pAddr + uOffset;
}

inline const void* CMemoryStream::GetAddress(size_t uOffset) const
{
    return (const char*)m_pAddr + uOffset;
}

inline bool CMemoryStream::Eof() const
{
    return m_uPos == m_uSize;
}

inline void CMemoryStream::Seek(long lOffset, STREAM_ORIGIN eOrigin)
{
    size_t uTmp = 0;
    switch (eOrigin)
    {
    case kBegin:
        uTmp = lOffset + 0;
        break;
        
    case kCur:
        uTmp = lOffset + m_uPos;
        break;
        
    case kEnd:
        uTmp = lOffset + m_uSize;
        break;
    }
    
    (uTmp < m_uSize) && (uTmp >= 0) && (m_uPos = uTmp);
}

inline size_t CMemoryStream::Tell() const
{
    return m_uPos;
}

inline size_t CMemoryStream::GetSize() const
{
    return m_uSize;
}

inline void CMemoryStream::Rewind()
{
    m_uPos = 0;
}

inline void CMemoryStream::Truncate()
{
    m_uSize = m_uPos;
}

inline CShmStream::CShmStream()
{
}

inline CShmStream::~CShmStream()
{
}

inline bool CShmStream::Detach()
{
#if !defined(TSCPDK_MSVC)
    return munmap(m_pAddr, m_uSize) == 0;
#else
    return UnmapViewOfFile(m_pAddr) == TRUE;
#endif
}

inline bool CShmStream::Delete(const char* pName)
{
#if !defined(TSCPDK_MSVC)
    return shm_unlink(pName) == 0;
#else
    return true;
#endif
}


NS_TSCPDK_END

#endif	/* MEMORY_INL */

