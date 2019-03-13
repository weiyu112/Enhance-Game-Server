#ifndef __NGX_LUAMANGER_H__
#define __NGX_LUAMANGER_H__
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>

class ngx_luamanager{
private:
    ngx_luamanager(){};
public:
    ~ngx_luamanager(){};

private:
    static ngx_luamanager* m_instance;
    lua_State *m_globlel=nullptr;        //建立一个虚拟机


public:
    static ngx_luamanager* getInstance()
    {
        if (m_instance==nullptr)
        {
            if(m_instance == nullptr)
            {
                m_instance = new ngx_luamanager();
                static CGarhuishou cl;
            }
        }

        return m_instance;
    }


    class CGarhuishou 
	{
	public:				
		~CGarhuishou()
		{
			if (ngx_luamanager::m_instance)
			{						
				delete ngx_luamanager::m_instance; //这个释放是整个系统退出的时候，系统来调用释放内存的哦
				ngx_luamanager::m_instance = nullptr;				
			}
		}
	};


public:
    void init_lua_model();
    void close_luamanger();
};


#endif