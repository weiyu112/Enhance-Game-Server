#include"ngx_c_login_manager.h"
#include"ngx_logic_login.h"


LoginManager::LoginManager()
{
    
}
LoginManager::~LoginManager()
{

}

bool LoginManager::initAllInterface()
{
    ngx_logic_login * _logic = new ngx_logic_login();
    addOneClassToVector(_logic);
    g_socket.registerMessageHanderFunction(5,CC_CALLBACK_5(ngx_logic_login::_HandleRegister,_logic));

    return true;
}