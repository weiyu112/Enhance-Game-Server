#ifndef __LUA_WRAPER_H__
#define __LUA_WRAPER_H__


#include <string>
#include "lua.h"
#include <pthread.h>    //多线程
class MsgPack;
class lua_wrapper {
public:
	lua_wrapper(){
		//m_pWrapper=this;
	}
	static void init();
	static void exit();

	static bool do_file(std::string& lua_file);
	static lua_State* lua_state();
	static void add_search_path(std::string& path);
	static void callLuarec_hander(MsgPack* _msg);
private:
	static pthread_mutex_t m_callLuamutex;
	//static lua_wrapper* m_pWrapper;

};

#endif

