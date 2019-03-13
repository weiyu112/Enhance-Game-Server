#include"ngx_luamanager.h"
ngx_luamanager* ngx_luamanager::m_instance=nullptr;

void ngx_luamanager::init_lua_model()
{
    m_globlel = luaL_newstate();         //建立一个虚拟机
    luaL_openlibs(m_globlel);
}


void ngx_luamanager::close_luamanger()
{
    if(m_globlel!=nullptr)
    {
        lua_close(m_globlel);
        m_globlel=nullptr;
    }
}