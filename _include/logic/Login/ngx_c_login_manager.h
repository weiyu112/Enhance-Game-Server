#ifndef __NGX_C_LOGIN_MANGER_H__
#define __NGX_C_LOGIN_MANGER_H__
#include"ngx_logic_manager.h"

class LoginManager :public LogicManager
{
public:
	LoginManager();
	~LoginManager();
public:
	bool initAllInterface();


};

#endif