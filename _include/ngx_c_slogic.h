
#ifndef __NGX_C_SLOGIC_H__
#define __NGX_C_SLOGIC_H__

#include <sys/socket.h>
#include "ngx_c_socket.h"
#include <map>
#include<iostream>
#include<functional>


//using namespace __gnu_cxx;
// class CLogicSocket;
// 	//定义成员函数指针
// typedef bool (CLogicSocket::*handler)(  lpngx_connection_t pConn,      //连接池中连接的指针
//                                         LPSTRUC_MSG_HEADER pMsgHeader,  //消息头指针
//                                         char *pPkgBody,                 //包体指针
//                                         unsigned short iBodyLength);    //包体长度

#define CC_CALLBACK_5(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)
typedef std::function<bool(lpngx_connection_t,LPCOMM_PKG_HEADER,LPSTRUC_MSG_HEADER,char *,unsigned short)> message_handler_function;

//处理逻辑和通讯的子类
class CLogicSocket : public CSocekt   //继承自父类CScoekt
{
public:
	CLogicSocket();                                                         //构造函数
	virtual ~CLogicSocket();                                                //释放函数
	virtual bool Initialize();                                              //初始化函数

public:
	//各种业务逻辑相关函数都在之类
	// bool _HandleRegister(lpngx_connection_t pConn,LPSTRUC_MSG_HEADER pMsgHeader,char *pPkgBody,unsigned short iBodyLength);
	// bool _HandleLogIn(lpngx_connection_t pConn,LPSTRUC_MSG_HEADER pMsgHeader,char *pPkgBody,unsigned short iBodyLength);

public:
	virtual void threadRecvProcFunc(char *pMsgBuf);
	virtual message_handler_function getHanderFunctionById(unsigned short id);
	bool registerMessageHanderFunction(unsigned short id,message_handler_function _function);
public:
	std::map<unsigned short,message_handler_function> m_all_message_hander_function;
};

#endif
