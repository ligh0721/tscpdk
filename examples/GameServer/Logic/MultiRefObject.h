/* 
 * File:   MutiRefObject.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午11:37
 */

#ifndef __MULTIREFOBJECT_H__
#define	__MULTIREFOBJECT_H__


class CMultiRefObject
{
public:
    CMultiRefObject();
    virtual ~CMultiRefObject();
    
    void retain();
    void release();
    
    virtual CMultiRefObject* copy() const;
    
    M_SYNTHESIZE_READONLY(int, m_iRefCount, RefCount);
    M_SYNTHESIZE_STR(DbgClassName);
    
    int getId() const;
    
private:
    void tryRelease();
    
protected:
    const int CONST_ID;
    
};

class CAutoReleasePool
{
public:
    typedef set<CMultiRefObject*> SET_OBJS;
    
public:    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(SET_OBJS, m_setObjs, Objects);
    void addObject(CMultiRefObject* pObject);
    void delObject(CMultiRefObject* pObject);
    void releaseObjects();
    
    M_SINGLETON(AutoReleasePool);
};

class CDbgMultiRefObjectManager
{
public:
    typedef set<CMultiRefObject*> SET_OBJS;
    
public:
    M_SYNTHESIZE_READONLY_PASS_BY_REF(SET_OBJS, m_setObjs, Objects);
    void addObject(CMultiRefObject* pObject);
    void delObject(CMultiRefObject* pObject);
    void printDbgInfo();
    
    M_SINGLETON(DbgMultiRefObjectManager);
};

template <typename MULTIREF_ID_TYPE_PTR>
class CMultiRefMap : public std::map<int, MULTIREF_ID_TYPE_PTR>
{
public:
    typedef typename std::map<int, MULTIREF_ID_TYPE_PTR>::iterator iterator;
    typedef typename std::map<int, MULTIREF_ID_TYPE_PTR>::const_iterator const_iterator;
    
public:
    ~CMultiRefMap();
    
    void addObject(MULTIREF_ID_TYPE_PTR pObj);
    void delObject(int id);
    void delObject(iterator it);
    MULTIREF_ID_TYPE_PTR getObject(int id) const;
    MULTIREF_ID_TYPE_PTR getObject(const const_iterator it) const;
    void delAllObjects();
    
};

template <typename MULTIREF_ID_TYPE_PTR>
class CMultiRefVec : public std::vector<MULTIREF_ID_TYPE_PTR>
{
public:
    ~CMultiRefVec();
    
    void addObject(MULTIREF_ID_TYPE_PTR pObj);
    void delAllObjects();
};



// ----------- Inline Implementation--------------
// CMultiRefMap
template <typename MULTIREF_ID_TYPE_PTR>
inline CMultiRefMap<MULTIREF_ID_TYPE_PTR>::~CMultiRefMap()
{
    delAllObjects();
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::addObject(MULTIREF_ID_TYPE_PTR pObj)
{
    pObj->retain();
    this->insert(make_pair(pObj->getId(), pObj));
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::delObject(int id)
{
    auto it = this->find(id);
    delObject(it);
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::delObject(iterator it)
{
    if (it == this->end())
    {
        return;
    }
    
    MULTIREF_ID_TYPE_PTR pDel = it->second;
    this->erase(it);
    pDel->release();
}

template <typename MULTIREF_ID_TYPE_PTR>
inline MULTIREF_ID_TYPE_PTR CMultiRefMap<MULTIREF_ID_TYPE_PTR>::getObject(int id) const
{
    auto it = this->find(id);
    return getObject(it);
}

template <typename MULTIREF_ID_TYPE_PTR>
inline MULTIREF_ID_TYPE_PTR CMultiRefMap<MULTIREF_ID_TYPE_PTR>::getObject(const const_iterator it) const
{
    return it != this->end() ? it->second : NULL;
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::delAllObjects()
{
    M_MAP_FOREACH(*this)
    {
        MULTIREF_ID_TYPE_PTR pObj = M_MAP_EACH;
        pObj->release();
    }
    this->clear();
}

// CMultiRefVec
template <typename MULTIREF_ID_TYPE_PTR>
inline CMultiRefVec<MULTIREF_ID_TYPE_PTR>::~CMultiRefVec()
{
    delAllObjects();
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefVec<MULTIREF_ID_TYPE_PTR>::addObject(MULTIREF_ID_TYPE_PTR pObj)
{
    pObj->retain();
    this->push_back(pObj);
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefVec<MULTIREF_ID_TYPE_PTR>::delAllObjects()
{
    M_MAP_FOREACH(*this)
    {
        MULTIREF_ID_TYPE_PTR pObj = M_VEC_EACH;
        pObj->release();
    }
    this->clear();
}

#endif	/* __MUTIREFOBJECT_H__ */

