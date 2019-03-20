#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string>
#include "ngx_func.h"
extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}
#include"LuaBridge.h"
#include "lua_net.h"
#include"bytearray.h"
#include "lua_wrapper.h"
#include"lua_mysql.h"
#include"ngx_c_lockmutex.h"

#define LogLuaStackNum(L)  ngx_log_stderr(0,"lua stack num: %d.", lua_gettop(L));

lua_State* g_lua_State = NULL;

pthread_mutex_t lua_wrapper::m_callLuamutex;
lua_State* 
lua_wrapper::lua_state() {
	return g_lua_State;
}

void 
lua_wrapper::init() {
	
	if(pthread_mutex_init(&m_callLuamutex, NULL)  != 0)
    {        
        ngx_log_stderr(0,"lua_wrapper::init中pthread_mutex_init(&m_callLuamutex)失败.");
        return;    
    }
	g_lua_State = luaL_newstate();
	luaL_openlibs(g_lua_State);
	registerNetAbout(g_lua_State);
	registerMysqlAbout(g_lua_State);

	std::string path("../../src/luaapp/");
	lua_wrapper::add_search_path(path);
	std::string mainpath("../../src/luaapp/main.lua");
	lua_wrapper::do_file(mainpath);

	//std::string path("../src/luaapp/main.lua");
	//do_file(path);
}
void lua_wrapper::callLuarec_hander(MsgPack* _msg)
{
	CLock clock(&m_callLuamutex);
	lua_getglobal(g_lua_State,"luatocppfunrevbuffer");
	luabridge::push(g_lua_State,_msg);
	int iRet = lua_pcall(g_lua_State, 1, 1, 0);
	if (0 != iRet)
	{
		ngx_log_stderr(0,"lua_pcall failed:%s\n", lua_tostring(g_lua_State, -1));
		return;
	}

	int ret = lua_tointeger(g_lua_State, -1);
	lua_pop(g_lua_State, 1);
	LogLuaStackNum(g_lua_State);
}
void
lua_wrapper::exit() {
	if (g_lua_State != NULL) {
		lua_close(g_lua_State);
		pthread_mutex_destroy(&m_callLuamutex);
		g_lua_State = NULL;
	}
}

bool
lua_wrapper::do_file(std::string& lua_file) {

	if (luaL_dofile(g_lua_State, lua_file.c_str())) {
		//lua_log_error(g_lua_State);
		return false;
	}
	return true;
}

void
lua_wrapper::add_search_path(std::string& path) {
	char strPath[1024] = { 0 };
	sprintf(strPath, "local path = string.match([[%s]],[[(.*)/[^/]*$]])\n package.path = package.path .. [[;]] .. path .. [[/?.lua;]] .. path .. [[/?/init.lua]]\n", path.c_str());
	luaL_dostring(g_lua_State, strPath);
}
