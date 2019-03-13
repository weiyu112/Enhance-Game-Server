#include "ngx_c_slogic.h"  
#include"ngx_logic_manager.h"
LogicManager::LogicManager()
{

}
LogicManager::~LogicManager()
{
    removeAllLogicClass();
}

bool LogicManager::initAllInterface()
{
    
    return true;
}


void LogicManager::addOneClassToVector(ngx_logic * _logic)
{
    m_allLogicClass.push_back(_logic);
}

void LogicManager::removeAllLogicClass()
{
    for(int i = 0; i < m_allLogicClass.size(); i++)
    {
        /* code */
        delete m_allLogicClass[i];
    }
    m_allLogicClass.clear();
}