#ifndef __NGX_C_GATAWAY_MANGER_H__
#define __NGX_C_GATAWAY_MANGER_H__
#include"ngx_logic_manager.h"

class GatawayManager :public LogicManager
{
public:
	GatawayManager();
	~GatawayManager();
public:
	bool initAllInterface();


};

#endif