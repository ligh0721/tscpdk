/* 
 * File:   TSScriptEngine.inl
 * Author: thunderliu
 *
 * Created on 2012年5月7日, 下午4:31
 */

#ifndef __TSSCRIPTENGINE_INL__
#define	__TSSCRIPTENGINE_INL__

#include "TSScriptEngine.h"



NS_TSCPDK_BEGIN

// CScriptEngine

inline CScriptEngine::~CScriptEngine()
{
}


// CAsyncScriptEngine

inline CThread* CAsyncScriptEngine::AsyncRunFile(const char* pFile, const void* pParam)
{
    CAsyncRunFileThread* pThrd = new CAsyncRunFileThread(this, pFile, pParam);
    pThrd->Start();
    return pThrd;
}

inline CThread* CAsyncScriptEngine::AsyncRunString(const char* pStr, const void* pParam)
{
    CAsyncRunStringThread* pThrd = new CAsyncRunStringThread(this, pStr, pParam);
    pThrd->Start();
    return pThrd;
}

inline void CAsyncScriptEngine::OnRunFileOk(CThread* pThrd, const char* pFile, const void* pParam)
{
}

inline void CAsyncScriptEngine::OnRunFileError(CThread* pThrd, const char* pFile, const CStr& roErr, const void* pParam)
{
}

inline void CAsyncScriptEngine::OnRunFileCancel(CThread* pThrd, const char* pFile, const void* pParam)
{
}

inline void CAsyncScriptEngine::OnRunStringOk(CThread* pThrd, const char* pStr, const void* pParam)
{
}

inline void CAsyncScriptEngine::OnRunStringError(CThread* pThrd, const char* pStr, const CStr& roErr, const void* pParam)
{
}

inline void CAsyncScriptEngine::OnRunStringCancel(CThread* pThrd, const char* pStr, const void* pParam)
{
}


// CAsyncScriptEngine::CAsyncRunFileThread

inline CAsyncScriptEngine::CAsyncRunFileThread::CAsyncRunFileThread(CAsyncScriptEngine* pAse, const char* pFile, const void* pParam)
: m_pAse(pAse)
, m_szFile(pFile, strlen(pFile) + 1, true)
, m_pParam(pParam)
, m_oErr(128)
{
}

inline CAsyncScriptEngine::CAsyncRunFileThread::~CAsyncRunFileThread()
{
}

inline long CAsyncScriptEngine::CAsyncRunFileThread::ThreadProc()
{
    pthread_detach(m_uHdl);
    return m_pAse->RunFile(m_szFile, &m_oErr);
}


// CAsyncScriptEngine::CAsyncRunStringThread

inline CAsyncScriptEngine::CAsyncRunStringThread::CAsyncRunStringThread(CAsyncScriptEngine* pAse, const char* pStr, const void* pParam)
: m_pAse(pAse)
, m_szStr(pStr, strlen(pStr) + 1, true)
, m_pParam(pParam)
, m_oErr(128)
{
}

inline CAsyncScriptEngine::CAsyncRunStringThread::~CAsyncRunStringThread()
{
}

inline long CAsyncScriptEngine::CAsyncRunStringThread::ThreadProc()
{
    pthread_detach(m_uHdl);
    return m_pAse->RunString(m_szStr, &m_oErr);
}

// CLuaTable

inline CLuaTable::CLuaTable(lua_State* pL, int iStackPos)
: m_pL(pL)
, m_iStackPos(iStackPos)
{
}

inline CLuaTable::operator int() const
{
    return m_iStackPos;
}

inline void CLuaTable::Attach(lua_State* pL, int iStackPos)
{
    m_pL = pL;
    m_iStackPos = iStackPos;
}

inline void CLuaTable::SetField(const char* pFieldName)
{
    lua_setfield(m_pL, m_iStackPos, pFieldName);
}

inline void CLuaTable::GetField(const char* pFieldName)
{
    lua_getfield(m_pL, m_iStackPos, pFieldName);
}

inline void CLuaTable::SetFunctions(const luaL_Reg* pFuncs)
{
    luaL_setfuncs(m_pL, pFuncs, 0);
}

inline void CLuaTable::RawGet(int iIndex)
{
    lua_rawgeti(m_pL, m_iStackPos, iIndex);
}

inline void CLuaTable::RawSet(int iIndex)
{
    lua_rawseti(m_pL, m_iStackPos, iIndex);
}

inline int CLuaTable::CreateRef()
{
    return luaL_ref(m_pL, m_iStackPos);
}

inline void CLuaTable::ReleaseRef(int iRef)
{
    luaL_unref(m_pL, m_iStackPos, iRef);
}

inline void CLuaTable::Get()
{
    lua_gettable(m_pL, m_iStackPos);
}

inline void CLuaTable::Set()
{
    lua_settable(m_pL, m_iStackPos);
}

// CLuaSe

inline void CLuaSe::Release()
{
    delete this;
}

inline CLuaSe::CLuaSe(bool bNewHandle)
: m_bNewHandle(bNewHandle)
, m_pL(m_bNewHandle ? luaL_newstate() : NULL)
, CONST_TABLE_REGISTRY(m_pL, CLuaTable::CONST_REGISTRY_INDEX)
{
}

inline CLuaSe::CLuaSe(lua_State* pL)
: m_bNewHandle(false)
, m_pL(pL)
, CONST_TABLE_REGISTRY(m_pL, CLuaTable::CONST_REGISTRY_INDEX)
{
}

inline CLuaSe::~CLuaSe()
{
    if (m_bNewHandle)
    {
        assert(m_pL);
        lua_close(m_pL);
    }
}

inline CLuaSe::operator lua_State*()
{
    return m_pL;
}

inline CLuaSe::operator const lua_State*() const
{
    return m_pL;
}

inline lua_State* CLuaSe::GetHandle()
{
    return m_pL;
}

inline void CLuaSe::Detach()
{
    m_pL = NULL;
}

inline void CLuaSe::RegisterCFunction(const char* pName, lua_CFunction pLuaCFunc)
{
    assert(m_pL);
    lua_register(m_pL, pName, pLuaCFunc);
}

inline const char* CLuaSe::ToString(int iStackPos, size_t* pLen)
{
    assert(m_pL);
    return lua_tolstring(m_pL, iStackPos, pLen);
}

inline lua_Number CLuaSe::ToNumber(int iStackPos, int* pIsNum)
{
    assert(m_pL);
    return lua_tonumber(m_pL, iStackPos);
}

inline lua_Integer CLuaSe::ToInteger(int iStackPos, int* pIsNum)
{
    assert(m_pL);
    return lua_tointeger(m_pL, iStackPos);
}

inline bool CLuaSe::ToBoolean(int iStackPos)
{
    assert(m_pL);
    return (bool)lua_toboolean(m_pL, iStackPos);
}

inline lua_CFunction CLuaSe::ToCFunction(int iStackPos)
{
    assert(m_pL);
    return lua_tocfunction(m_pL, iStackPos);
}

inline void* CLuaSe::ToUserData(int iStackPos)
{
    return lua_touserdata(m_pL, iStackPos);
}

inline CLuaSe CLuaSe::ToThread(int iStackPos)
{
    return CLuaSe(lua_tothread(m_pL, iStackPos));
}

inline const char* CLuaSe::CheckString(uint32_t dwArgvIndex, size_t* pLen)
{
    assert(m_pL);
    return luaL_checklstring(m_pL, dwArgvIndex, pLen);
}

inline lua_Number CLuaSe::CheckNumber(uint32_t dwArgvIndex)
{
    assert(m_pL);
    return luaL_checknumber(m_pL, dwArgvIndex);
}

inline lua_Integer CLuaSe::CheckInteger(uint32_t dwArgvIndex)
{
    assert(m_pL);
    return luaL_checkinteger(m_pL, dwArgvIndex);
}

inline const char* CLuaSe::GetGlobalString(const char* pVar, size_t* pLen)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
    const char* pRet = lua_tolstring(m_pL, CONST_STACK_TOP, pLen);
    lua_pop(m_pL, 1);
    return pRet;
}

inline lua_Number CLuaSe::GetGlobalNumber(const char* pVar, int* pIsNum)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
    lua_Number dRet = lua_tonumber(m_pL, CONST_STACK_TOP);
    lua_pop(m_pL, 1);
    return dRet;
}

inline lua_Integer CLuaSe::GetGlobalInteger(const char* pVar, int* pIsNum)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
    int lRet = lua_tointeger(m_pL, CONST_STACK_TOP);
    lua_pop(m_pL, 1);
    return lRet;
}

inline bool CLuaSe::GetGlobalBoolean(const char* pVar)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
    bool bRet = lua_toboolean(m_pL, CONST_STACK_TOP);
    lua_pop(m_pL, 1);
    return bRet;
}

inline lua_CFunction CLuaSe::GetGlobalCFunction(const char* pVar)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
    lua_CFunction pRet = lua_tocfunction(m_pL, CONST_STACK_TOP);
    lua_pop(m_pL, 1);
    return pRet;
}

inline void CLuaSe::PushGetGlobal(const char* pVar)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
}

inline void CLuaSe::PushString(const char* pStr)
{
    assert(m_pL);
    lua_pushstring(m_pL, pStr);
}

inline void CLuaSe::PushString(const char* pStr, size_t uLen)
{
    assert(m_pL);
    lua_pushlstring(m_pL, pStr, uLen);
}

inline void CLuaSe::PushNumber(lua_Number dNum)
{
    assert(m_pL);
    lua_pushnumber(m_pL, dNum);
}

inline void CLuaSe::PushInteger(lua_Integer lNum)
{
    assert(m_pL);
    lua_pushinteger(m_pL, lNum);
}

inline void CLuaSe::PushBoolean(bool bBool)
{
    assert(m_pL);
    lua_pushboolean(m_pL, bBool);
}

inline void CLuaSe::PushCFunction(lua_CFunction pFun)
{
    assert(m_pL);
    lua_pushcfunction(m_pL, pFun);
}

inline void CLuaSe::PushNil()
{
    assert(m_pL);
    lua_pushnil(m_pL);
}

inline void CLuaSe::PushUserData(void* pData)
{
    lua_pushlightuserdata(m_pL, pData);
}

inline void CLuaSe::PushThread(lua_State* pL)
{
    lua_pushthread(pL);
}

inline CLuaSe CLuaSe::PushNewThread()
{
    return CLuaSe(lua_newthread(m_pL));
}

inline void CLuaSe::PushValue(int iStackPos)
{
    lua_pushvalue(m_pL, iStackPos);
}

inline CLuaTable CLuaSe::PushTable()
{
    lua_newtable(m_pL);
    return CLuaTable(this->GetHandle(), GetTopPos());
}

inline void CLuaSe::PushTableField(const char* pItemName, int iStackPos)
{
    PushString(pItemName);
    lua_gettable(m_pL, iStackPos);
}

inline void CLuaSe::RawGetFromTable(int iIndex, int iStackPos)
{
    lua_rawgeti(m_pL, iStackPos, iIndex);
}

inline void CLuaSe::SetGlobal(const char* pVar)
{
    assert(m_pL);
    lua_setglobal(m_pL, pVar);
}

inline void CLuaSe::Pop(uint32_t dwCount)
{
    assert(m_pL);
    lua_pop(m_pL, dwCount);
}

inline int CLuaSe::GetTopPos() const
{
    assert(m_pL);
    return lua_gettop(m_pL);
}

inline void CLuaSe::Insert(int iStackPos)
{
    assert(m_pL);
    lua_insert(m_pL, iStackPos);
}

inline void CLuaSe::Concat(uint32_t dwCount)
{
    assert(m_pL);
    lua_concat(m_pL, dwCount);
}

inline void CLuaSe::Remove(int iStackPos)
{
    assert(m_pL);
    lua_remove(m_pL, iStackPos);
}

#if LUA_VERSION_NUM >= 502

inline lua_Unsigned CLuaSe::ToUnsigned(int iStackPos, int* pIsNum)
{
    assert(m_pL);
    return lua_tounsigned(m_pL, iStackPos);
}

inline lua_Unsigned CLuaSe::CheckUnsigned(uint32_t dwArgvIndex)
{
    assert(m_pL);
    return luaL_checkunsigned(m_pL, dwArgvIndex);
}

inline lua_Unsigned CLuaSe::GetGlobalUnsigned(const char* pVar, int* pIsNum)
{
    assert(m_pL);
    lua_getglobal(m_pL, pVar);
    lua_Unsigned dwRet = lua_tounsigned(m_pL, CONST_STACK_TOP);
    lua_pop(m_pL, 1);
    return dwRet;
}

inline void CLuaSe::PushUnsigned(lua_Unsigned dwNum)
{
    assert(m_pL);
    lua_pushunsigned(m_pL, dwNum);
}

inline bool CLuaSe::Equal(int iStackPos1, int iStackPos2)
{
    assert(m_pL);
    return (bool)lua_compare(m_pL, iStackPos1, iStackPos2, LUA_OPEQ);
}

inline bool CLuaSe::LessThan(int iStackPos1, int iStackPos2)
{
    assert(m_pL);
    return (bool)lua_compare(m_pL, iStackPos1, iStackPos2, LUA_OPLT);
}

inline bool CLuaSe::LessOrEqual(int iStackPos1, int iStackPos2)
{
    assert(m_pL);
    return (bool)lua_compare(m_pL, iStackPos1, iStackPos2, LUA_OPLE);
}

inline void CLuaSe::Copy(int iFromStackPos, int iToStackPos)
{
    assert(m_pL);
    lua_copy(m_pL, iFromStackPos, iToStackPos);
}

inline void CLuaSe::XMove(lua_State* pFromL, lua_State* pToL, int iCount)
{
    lua_xmove(pFromL, pToL, iCount);
}

inline int CLuaSe::YieldL(int iResCount, int iCtx, lua_CFunction pContinue)
{
    return lua_yieldk(m_pL, iResCount, iCtx, pContinue);
}

inline int CLuaSe::ResumeL(lua_State* pFrom, int iArgvCount)
{
    return lua_resume(m_pL, pFrom ? pFrom : NULL, iArgvCount);
}

inline int CLuaSe::GetCtx(int* pCtx)
{
    return lua_getctx(m_pL, pCtx);
}

inline CLuaTable& CLuaSe::GetRegistry()
{
    return CONST_TABLE_REGISTRY;
}

#endif // LUA_VERSION_NUM


NS_TSCPDK_END


#endif	/* __TSSCRIPTENGINE_INL__ */

