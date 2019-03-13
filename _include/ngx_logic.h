#ifndef __NGX_LOGIC_H__
#define __NGX_LOGIC_H__

#include"ngx_c_slogic.h"
class CLogicSocket;

extern CLogicSocket  g_socket; 

class ngx_logic{
public:
    ngx_logic();
    ~ngx_logic();

protected:
    size_t m_apkHander;
    size_t m_msgHander;


};

#endif