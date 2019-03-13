#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>    //uintptr_t
#include <stdarg.h>    //va_start....
#include <unistd.h>    //STDERR_FILENO等
#include <sys/time.h>  //gettimeofday
#include <time.h>      //localtime_r
#include <fcntl.h>     //open
#include <errno.h>     //errno
//#include <sys/socket.h>
#include <sys/ioctl.h> //ioctl
#include <arpa/inet.h>
#include <pthread.h>   //多线程
#include "ngx_c_conf.h"
#include "ngx_macro.h"
#include "ngx_global.h"
#include "ngx_func.h"
#include "ngx_c_memory.h"
#include "ngx_c_crc32.h"
#include "ngx_logiccomm.h"  
#include "ngx_c_lockmutex.h"  
#include"ngx_logic_gataway.h"


ngx_logic_gataway::ngx_logic_gataway(/* args */):
m_changeSendMapIdMax(0)
{
}

ngx_logic_gataway::~ngx_logic_gataway()
{
}

bool ngx_logic_gataway::_HandleGataway(lpngx_connection_t pConn,LPCOMM_PKG_HEADER pMypkHeader,LPSTRUC_MSG_HEADER pMsgHeader,char *pPkgBody,unsigned short iBodyLength)
{
    //(1)首先判断包体的合法性
    if(pPkgBody == NULL) //具体看客户端服务器约定，如果约定这个命令[msgCode]必须带包体，那么如果不带包体，就认为是恶意包，直接不处理    
    {        
        return false;
    }
		    
    int iRecvLen = sizeof(STRUCT_REGISTER); 
    if(iRecvLen != iBodyLength) //发送过来的结构大小不对，认为是恶意包，直接不处理
    {     
        return false; 
    }

    //(2)对于同一个用户，可能同时发送来多个请求过来，造成多个线程同时为该 用户服务，比如以网游为例，用户要在商店中买A物品，又买B物品，而用户的钱 只够买A或者B，不够同时买A和B呢？
       //那如果用户发送购买命令过来买了一次A，又买了一次B，如果是两个线程来执行同一个用户的这两次不同的购买命令，很可能造成这个用户购买成功了 A，又购买成功了 B
       //所以，为了稳妥起见，针对某个用户的命令，我们一般都要互斥,我们需要增加临界的变量于ngx_connection_s结构中
    CLock lock(&pConn->logicPorcMutex); //凡是和本用户有关的访问都互斥
    
    //(3)取得了整个发送过来的数据
    //LPSTRUCT_REGISTER p_RecvInfo = (LPSTRUCT_REGISTER)pPkgBody; 

    //(4)这里可能要考虑 根据业务逻辑，进一步判断收到的数据的合法性，
       //当前该玩家的状态是否适合收到这个数据等等【比如如果用户没登陆，它就不适合购买物品等等】
        //这里大家自己发挥，自己根据业务需要来扩充代码，老师就不带着大家扩充了。。。。。。。。。。。。
    //。。。。。。。。

    //(5)给客户端返回数据时，一般也是返回一个结构，这个结构内容具体由客户端/服务器协商，这里我们就以给客户端也返回同样的 STRUCT_REGISTER 结构来举例    
    //LPSTRUCT_REGISTER pFromPkgHeader =  (LPSTRUCT_REGISTER)(((char *)pMsgHeader)+m_iLenMsgHeader);	//指向收到的包的包头，其中数据后续可能要用到
	LPCOMM_PKG_HEADER pPkgHeader;	
    LPSTRUC_MSG_HEADER _pMsgHeader;
    
	CMemory  *p_memory = CMemory::GetInstance();
	CCRC32   *p_crc32 = CCRC32::GetInstance();
    int iSendLen = iBodyLength;  
    ngx_log_stderr(0,"iSendLen:%d!\n",iSendLen);

    
    //a)分配要发送出去的包的内存

//iSendLen = 65000; //unsigned最大也就是这个值
    char *p_sendbuf = (char *)p_memory->AllocMemory(m_msgHander+m_apkHander+iSendLen,false);//准备发送的格式，这里是 消息头+包头+包体
    //ngx_log_stderr(0,"packagelen:%d!\n",m_msgHander+m_apkHander+iSendLen);
    pPkgHeader = (LPCOMM_PKG_HEADER)(p_sendbuf+m_msgHander);
    if(pMypkHeader->isComeSever)
    {
        //ngx_log_stderr(0,"pMypkHeader->_id%d!\n",htonl(pMypkHeader->_id));
        _pMsgHeader=getOneMsgHeaderBySocketFd(htonl(pMypkHeader->_id));
        removeOneSendItemById(htonl(pMypkHeader->_id));
        // ngx_log_stderr(0,"_pMsgHeader->iCurrsequence:%d!\n", _pMsgHeader->iCurrsequence);
        // ngx_log_stderr(0,"_pMsgHeader->pConn->iCurrsequence:%d!\n", _pMsgHeader->pConn->iCurrsequence);
        // lpngx_connection_t _Conn = _pMsgHeader->pConn;
                
        // struct sockaddr_in *sock = ( struct sockaddr_in*)&_Conn->s_sockaddr;
        // ngx_log_stderr(0,"pMypkHeader->isComeSever,连接ip：%s;连接port：%d\n",inet_ntoa(sock->sin_addr),ntohs(sock->sin_port));
        memcpy(p_sendbuf,_pMsgHeader,m_msgHander); 
    }
    else{
        _pMsgHeader=(LPSTRUC_MSG_HEADER)p_memory->AllocMemory(m_msgHander,true);
        _pMsgHeader->pConn=g_socket.ngx_get_one_childserver_connectbyservertype(pMypkHeader->sServerType);
        //ngx_log_stderr(0,"5555555555555555555555555555555!\n");
        _pMsgHeader->iCurrsequence=_pMsgHeader->pConn->iCurrsequence;
        // lpngx_connection_t _Conn = _pMsgHeader->pConn;
                
        // struct sockaddr_in *sock = ( struct sockaddr_in*)&_Conn->s_sockaddr;
        // ngx_log_stderr(0,"pMypkHeader->isComeSever,连接ip：%s;连接port：%d\n",inet_ntoa(sock->sin_addr),ntohs(sock->sin_port));
        memcpy(p_sendbuf,_pMsgHeader,m_msgHander);  
    }
    if(_pMsgHeader)
    {
        p_memory->FreeMemory(_pMsgHeader);
        _pMsgHeader=nullptr;
    }
                      //消息头直接拷贝到这里来

    memcpy(p_sendbuf+m_msgHander+m_apkHander,pPkgBody,iSendLen);

    
    
    //c)填充包头
        //指向包头
    if(pMypkHeader->isComeSever)
    {
        pPkgHeader->pkgLen=htons(pMypkHeader->pkgLen); 
        pPkgHeader->msgCode=htons(pMypkHeader->msgCode);
        pPkgHeader->sServerType=htons(pMypkHeader->sServerType);
        pPkgHeader->_id=htonl(pMypkHeader->_id);
        pPkgHeader->isComeSever=pMypkHeader->isComeSever;
        pPkgHeader->crc32 = htonl(pMypkHeader->crc32);
    }
    else{
        int _id=getOneId();
        addChangeSendMsgListByFd(_id,pMsgHeader);
        pPkgHeader->pkgLen=htons(pMypkHeader->pkgLen); 
        pPkgHeader->msgCode=htons(pMypkHeader->msgCode);
        pPkgHeader->sServerType=htons(pMypkHeader->sServerType);
        pPkgHeader->_id=htonl(_id);
        pPkgHeader->isComeSever=pMypkHeader->isComeSever;
        pPkgHeader->crc32 = htonl(pMypkHeader->crc32);
    }
    //ngx_log_stderr(0,"3333333333333333333333333333333size:%d!\n",sizeof(p_sendbuf));
    // pMypkHeader->msgCode = pMypkHeader->msgCode;	                        //消息代码，可以统一在ngx_logiccomm.h中定义
    // pPkgHeader->msgCode = htons(pPkgHeader->msgCode);	            //htons主机序转网络序 
    // pPkgHeader->pkgLen  = htons(m_apkHander + iSendLen);        //整个包的尺寸【包头+包体尺寸】 
    //d)填充包体
    //void* p_sendInfo = (void*)(p_sendbuf+m_msgHander+m_apkHander);	//跳过消息头，跳过包头，就是包体了
    // // //。。。。。这里根据需要，填充要发回给客户端的内容,int类型要使用htonl()转，short类型要使用htons()转；
    
    // // //e)包体内容全部确定好后，计算包体的crc32值
    // pPkgHeader->crc32   = p_crc32->Get_CRC((unsigned char *)pPkgBody,iSendLen);
    // pPkgHeader->crc32   = htonl(pPkgHeader->crc32);		
    //ngx_log_stderr(0,"222222222222!");
    //f)发送数据包
    g_socket.msgSend(p_sendbuf);  
    /*if(ngx_epoll_oper_event(
                                pConn->fd,          //socekt句柄
                                EPOLL_CTL_MOD,      //事件类型，这里是增加
                                EPOLLOUT,           //标志，这里代表要增加的标志,EPOLLOUT：可写
                                0,                  //对于事件类型为增加的，EPOLL_CTL_MOD需要这个参数, 0：增加   1：去掉 2：完全覆盖
                                pConn               //连接池中的连接
                                ) == -1)        
    {
        ngx_log_stderr(0,"1111111111111111111111111111111111111111111111111111111111111!");
    } */
    
    /*
    sleep(100);  //休息这么长时间
    //如果连接回收了，则肯定是iCurrsequence不等了
    if(pMsgHeader->iCurrsequence != pConn->iCurrsequence)
    {
        //应该是不等，因为这个插座已经被回收了
        ngx_log_stderr(0,"插座不等,%L--%L",pMsgHeader->iCurrsequence,pConn->iCurrsequence);
    }
    else
    {
        ngx_log_stderr(0,"插座相等哦,%L--%L",pMsgHeader->iCurrsequence,pConn->iCurrsequence);
    }
    
    */
    //ngx_log_stderr(0,"执行了CLogicSocket::_HandleRegister()!");
    return true;
}


LPSTRUC_MSG_HEADER ngx_logic_gataway::getOneMsgHeaderBySocketFd(int _fd)
{
    //ngx_log_stderr(0,"3333333333333333333333333333333!m_allChangeSendMsgHeader.size():%d\n",m_allChangeSendMsgHeader.size());
    if(m_allChangeSendMsgHeader.find(_fd)!=m_allChangeSendMsgHeader.end())
    {
        return m_allChangeSendMsgHeader.at(_fd);
    }

    return nullptr;
}

void ngx_logic_gataway::addChangeSendMsgListByFd(int _fd,LPSTRUC_MSG_HEADER _msg)
{
    CMemory  *p_memory = CMemory::GetInstance();
    LPSTRUC_MSG_HEADER _mmsg = (LPSTRUC_MSG_HEADER)p_memory->AllocMemory(m_msgHander,false);
    memcpy(_mmsg,_msg,m_msgHander);
    m_allChangeSendMsgHeader.insert(std::pair<int,LPSTRUC_MSG_HEADER>(_fd,_mmsg));
}

void ngx_logic_gataway::removeOneSendItemById(int _id)
{
    auto iter = m_allChangeSendMsgHeader.find(_id);
    if(iter !=m_allChangeSendMsgHeader.end())
    {
        m_allChangeSendMsgHeader.erase(_id);
        comeBackOneId(_id);
    }
}

int  ngx_logic_gataway::getOneId()
{
    if(m_changeSendMapId.size()>0)
    {
        int _id = *m_changeSendMapId.begin();
        m_changeSendMapId.pop_front();
        return _id;
    }
    else{
        return ++m_changeSendMapIdMax;
    }
}
void ngx_logic_gataway::comeBackOneId(int _id)
{
    m_changeSendMapId.push_back(_id);
}
