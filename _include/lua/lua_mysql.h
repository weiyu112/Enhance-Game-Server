#ifndef __LUA_MYSQL_H__
#define __LUA_MYSQL_H__
extern "C" {
#include "lua.h"
}
bool registerMysqlAbout(lua_State* tolua_S);

#endif