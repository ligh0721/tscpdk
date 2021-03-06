CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(lua)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET(LIBLUA_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(LIBLUA_SRC_DIR)

IF(tscpdk_BINARY_DIR)
    SET(LIBRARY_OUTPUT_PATH ${tscpdk_BINARY_DIR})
ENDIF(tscpdk_BINARY_DIR)

SET(LIBLUA_H
    ${LIBLUA_SRC_DIR}/lauxlib.h
    ${LIBLUA_SRC_DIR}/lua.h
    ${LIBLUA_SRC_DIR}/lua.hpp
    ${LIBLUA_SRC_DIR}/luaconf.h
    ${LIBLUA_SRC_DIR}/lualib.h
)

SET(LIBLUA_SRC
    ${LIBLUA_SRC_DIR}/lapi.c
    ${LIBLUA_SRC_DIR}/lcode.c
    ${LIBLUA_SRC_DIR}/lctype.c
    ${LIBLUA_SRC_DIR}/ldebug.c
    ${LIBLUA_SRC_DIR}/ldo.c
    ${LIBLUA_SRC_DIR}/ldump.c
    ${LIBLUA_SRC_DIR}/lfunc.c
    ${LIBLUA_SRC_DIR}/lgc.c
    ${LIBLUA_SRC_DIR}/llex.c
    ${LIBLUA_SRC_DIR}/lmem.c
    ${LIBLUA_SRC_DIR}/lobject.c
    ${LIBLUA_SRC_DIR}/lopcodes.c
    ${LIBLUA_SRC_DIR}/lparser.c
    ${LIBLUA_SRC_DIR}/lstate.c
    ${LIBLUA_SRC_DIR}/lstring.c
    ${LIBLUA_SRC_DIR}/ltable.c
    ${LIBLUA_SRC_DIR}/ltm.c
    ${LIBLUA_SRC_DIR}/lundump.c
    ${LIBLUA_SRC_DIR}/lvm.c
    ${LIBLUA_SRC_DIR}/lzio.c
    ${LIBLUA_SRC_DIR}/lauxlib.c
    ${LIBLUA_SRC_DIR}/lbaselib.c
    ${LIBLUA_SRC_DIR}/lbitlib.c
    ${LIBLUA_SRC_DIR}/lcorolib.c
    ${LIBLUA_SRC_DIR}/ldblib.c
    ${LIBLUA_SRC_DIR}/liolib.c
    ${LIBLUA_SRC_DIR}/lmathlib.c
    ${LIBLUA_SRC_DIR}/loslib.c
    ${LIBLUA_SRC_DIR}/lstrlib.c
    ${LIBLUA_SRC_DIR}/ltablib.c
    ${LIBLUA_SRC_DIR}/loadlib.c
    ${LIBLUA_SRC_DIR}/linit.c
)

SET(LUAT_SRC
    ${LIBLUA_SRC}
    lua.c
)

SET(LUAC_SRC
    ${LIBLUA_SRC}
    luac.c
)

ADD_DEFINITIONS(-DLUA_COMPAT_ALL)
ADD_LIBRARY(LIBLUA_A STATIC ${LIBLUA_SRC})
SET_TARGET_PROPERTIES(LIBLUA_A PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
SET_TARGET_PROPERTIES(LIBLUA_A PROPERTIES CLEAN_DIRECT_OUTPUT 1)

IF(WIN32)
    IF(UNIX)
        # CYGWIN
    ELSE(UNIX)
        # MSVC
    ENDIF(UNIX)
ELSEIF(UNIX)
    ADD_DEFINITIONS(-DLUA_USE_LINUX -fPIC)
ENDIF(WIN32)

INSTALL(TARGETS LIBLUA_A ARCHIVE DESTINATION lib)

INSTALL(FILES
    ${LIBLUA_H} DESTINATION include/tscpdk/third_party/lua
)