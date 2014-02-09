/* 
 * File:   TSStream.h
 * Author: thunderliu
 *
 * Created on 2013年11月24日, 下午3:06
 */

#ifndef __TSSTREAM_H__
#define	__TSSTREAM_H__

#include "TSPlatform.h"


NS_TSCPDK_BEGIN


class TSCPDK_API CStream
{
public:
    enum STREAM_ORIGIN
    {
        kBegin = SEEK_SET,
        kCur = SEEK_CUR,
        kEnd = SEEK_END
    };
    
public:
    virtual ~CStream();
    
    virtual void Rewind() = 0;
    virtual size_t GetSize() const = 0;
    
    virtual bool Eof() const = 0;
    virtual void Seek(long lOffset, STREAM_ORIGIN eOrigin) = 0;
    virtual size_t Tell() const = 0;
    
    virtual size_t Write(const void* pData, size_t uSize, size_t uCount = 1) = 0;
    virtual size_t Read(void* pData, size_t uSize, size_t uCount = 1) = 0;
    
    template <typename TYPE>
    size_t Write(const TYPE* pData, size_t uCount = 1);
    template <typename TYPE>
    size_t Read(TYPE* pData, size_t uCount = 1);

};


NS_TSCPDK_END

#include "TSStream.inl"

#endif	/* __TSSTREAM_H__ */

