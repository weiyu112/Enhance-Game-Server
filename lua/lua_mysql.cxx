#include"lua_mysql.h"

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}
#include"NattyConnectionPool.h"
#include"LuaBridge.h"
#include"ngx_c_socket.h"
#include"ngx_func.h"



using namespace luabridge;


bool registerMysqlAbout(lua_State* tolua_S)
{
    getGlobalNamespace(tolua_S)
        .beginNamespace("mysql") 
            // .beginClass<NTY_MYSQL_CONN>("NTY_MYSQL_CONN")
            //     .addConstructor<void(*)(int)>()
            // .endClass()
            // .beginClass<NTY_MYSQL_SET>("NTY_MYSQL_SET")
            //     .addConstructor<void(*)(int)>()
            // .endClass()
            // .addFunction("ntyConnPoolGetConnection",&ntyConnPoolGetConnection)
            // .addFunction("ntyConnPoolExecuteQuery",&ntyConnPoolExecuteQuery)
            // //.addFunction("ntyShrimpStmtResultSetNext",&ntyShrimpStmtResultSetNext)
            // .addFunction("ntyConnPoolRetConnection",&ntyConnPoolRetConnection)
            // .addFunction("ntyResultSetgetLLong",&ntyResultSetgetLLong)
            // .addFunction("ntyResultSetgetString",&ntyResultSetgetString)
            // .addFunction("ntyResultSetgetInt",&ntyResultSetgetInt)
        .endNamespace();
    

}