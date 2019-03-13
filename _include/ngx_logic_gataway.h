#ifndef __NGX_LOGIC_GATAWAY_H__
#define __NGX_LOGIC_GATAWAY_H__
#include"ngx_logic.h"
#include <map>
#include<iostream>
class ngx_logic_gataway :public ngx_logic
{
private:
    /* data */
public:
    ngx_logic_gataway(/* args */);
    ~ngx_logic_gataway();

public:
    bool _HandleGataway(lpngx_connection_t pConn,LPCOMM_PKG_HEADER pMypkHeader,LPSTRUC_MSG_HEADER pMsgHeader,char *pPkgBody,unsigned short iBodyLength);

private:
    LPSTRUC_MSG_HEADER getOneMsgHeaderBySocketFd(int _fd);

    void addChangeSendMsgListByFd(int _fd,LPSTRUC_MSG_HEADER _msg);

    void removeOneSendItemById(int _id);

    int  getOneId();
    void comeBackOneId(int _id);

private:
    std::map<int,LPSTRUC_MSG_HEADER> m_allChangeSendMsgHeader;

    std::list<int >    m_changeSendMapId;
    int                m_changeSendMapIdMax;



    
};





#endif