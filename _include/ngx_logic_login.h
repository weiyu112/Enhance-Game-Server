#ifndef __NGX_LOGIC_LOGIN_H__
#define __NGX_LOGIC_LOGIN_H__
#include"ngx_logic.h"
class ngx_logic_login :public ngx_logic
{
private:
    /* data */
public:
    ngx_logic_login(/* args */);
    ~ngx_logic_login();

public:
    bool _HandleRegister(lpngx_connection_t pConn,LPCOMM_PKG_HEADER pMypkHeader,LPSTRUC_MSG_HEADER pMsgHeader,char *pPkgBody,unsigned short iBodyLength);



    
};





#endif