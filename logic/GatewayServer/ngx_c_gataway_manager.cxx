#include"ngx_c_gataway_manager.h"
#include"ngx_logic_gataway.h"


GatawayManager::GatawayManager()
{
    
}
GatawayManager::~GatawayManager()
{

}

bool GatawayManager::initAllInterface()
{
    ngx_logic_gataway * _gataway = new ngx_logic_gataway();
    addOneClassToVector(_gataway);
    g_socket.registerMessageHanderFunction(1,CC_CALLBACK_5(ngx_logic_gataway::_HandleGataway,_gataway));

    return true;
}