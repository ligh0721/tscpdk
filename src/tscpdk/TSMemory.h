/* 
 * File:   TSMemory.h
 * Author: thunderliu
 *
 * Created on 2012年4月16日, 下午4:29
 */

#ifndef __TSMEMORY_H__
#define	__TSMEMORY_H__


#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "TSStream.h"


NS_TSCPDK_BEGIN

class TSCPDK_API CBuffer
{
public:
    CBuffer(uint32_t dwSize);
    ~CBuffer();
    void* GetBuffer(uint32_t dwPos = 0);
    uint32_t GetBufferSize() const;
    
protected:
    uint8_t* m_pBuf;
    uint32_t m_dwSize;
};

class TSCPDK_API CMemoryStream : public CStream
{
public:
    CMemoryStream();
    CMemoryStream(void* pAddr, size_t dwSize);
    
    void Attach(void* pAddr, size_t uSize);

    virtual void Rewind();
    virtual size_t GetSize() const;
    virtual size_t Tell() const;
    virtual bool Eof() const;
    virtual void Seek(long lOffset, STREAM_ORIGIN eOrigin);
    
    virtual size_t Write(const void* pData, size_t uSize, size_t uCount = 1);
    virtual size_t Read(void* pData, size_t uSize, size_t uCount = 1);
    
    void* Read(size_t uSize);
    void* GetAddress(size_t uOffset = 0);
    const void* GetAddress(size_t uOffset = 0) const;
    
    void Truncate();

protected:
    void* m_pAddr;
    size_t m_uPos;
    size_t m_uSize;
};

#define BH_OBJ __oBh_1DC65F21__
#define BH_BUF (BH_OBJ.GetAddress())
#define BH_POS (BH_OBJ.Tell())
#define BH_SIZE (BH_OBJ.GetSize())

#define BH_NEW(buff, size) CMemoryStream BH_OBJ((buff), (size))
#define BH_REF(bf) CMemoryStream& BH_OBJ = bf

#define BH_GET_NEXT_RET(type, var, text) \
type* var = (type*)BH_OBJ.Read(sizeof(type)); \
do \
{ \
    unlikely(!var) \
    { \
        LOG_ERR("ERR | pkg len(%d) read(%d) err, " text, (int)BH_OBJ.GetSize(), (int)sizeof(type)); \
        return false; \
    } \
} while (false)

#define BH_CHECK_NEXT_RET(size, text) \
do \
{ \
    unlikely(!BH_OBJ.Read((size))) \
    { \
        LOG_ERR("ERR | pkg len(%d) read(%d) err, " text, (int)BH_OBJ.GetSize(), (int)(size)); \
        return false; \
    } \
} while (false)

#define BH_GET_NEXT_RET_EX(bh, type, var, text) \
type* var = (type*)bh.Read(sizeof(type)); \
do \
{ \
    unlikely(!var) \
    { \
        LOG_ERR("ERR | pkg len(%d) read(%d) err, " text, (int)bh.GetSize(), (int)sizeof(type)); \
        return false; \
    } \
} while (false)

#define BH_CHECK_NEXT_RET_EX(bh, size, text) \
do \
{ \
    unlikely(!bh.Read((size))) \
    { \
        LOG_ERR("ERR | pkg len(%d) read(%d) err, " text, (int)bh.GetSize(), (int)(size)); \
        return false; \
    } \
} while (false)

#define BH_GET_NEXT(type, var) type* var = (type*)BH_OBJ.Read(sizeof(type))
#define BH_CHECK_NEXT(size) BH_OBJ.Read((size))

#define BH_GET(type, var, base, off) type* var = (type*)((size_t)(base) + (off))


class TSCPDK_API CShmStream : public CMemoryStream
{
public:
    CShmStream();
    virtual ~CShmStream();
    
    bool Init(const char* pName, size_t uSize);
    bool Attach(const char* pName, size_t uSize = 0);
    bool Detach();
    static bool Delete(const char* pName);
};


NS_TSCPDK_END

#include "TSMemory.inl"

#endif	/* MEMORY_H */

