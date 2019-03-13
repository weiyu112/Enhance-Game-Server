#ifndef __NGX_LOGIC_MANAGER_H__
#define __NGX_LOGIC_MANAGER_H__
#include<vector>
#include"ngx_logic.h"

class CLogicSocket;

extern CLogicSocket  g_socket; 

class LogicManager
{
public:
    LogicManager();
    virtual ~LogicManager();
public:
    virtual bool initAllInterface();
    void addOneClassToVector(ngx_logic * _logic);
    void removeAllLogicClass();
public:
    std::vector<ngx_logic*> m_allLogicClass;

};

#endif