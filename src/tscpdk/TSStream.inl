/* 
 * File:   TSStream.inl
 * Author: thunderliu
 *
 * Created on 2013年11月24日, 下午3:08
 */

#ifndef __TSSTREAM_INL__
#define	__TSSTREAM_INL__

#include "TSStream.h"


NS_TSCPDK_BEGIN

inline CStream::~CStream()
{
}


template <typename TYPE>
inline size_t CStream::Write(const TYPE* pData, size_t uCount)
{
    return Write(pData, sizeof(TYPE), uCount);
}

template <typename TYPE>
inline size_t CStream::Read(TYPE* pData, size_t uCount)
{
    return Read(pData, sizeof(TYPE), uCount);
}        

NS_TSCPDK_END

#endif	/* __TSSTREAM_INL__ */

