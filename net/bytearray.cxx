#include "ByteArray.h"
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>    //uintptr_t
#include <stdarg.h>    //va_start....
#include <unistd.h>    //STDERR_FILENO等
#include <sys/time.h>  //gettimeofday
#include <time.h>      //localtime_r
#include <fcntl.h>     //open
#include <errno.h>     //errno
#include <sys/ioctl.h> //ioctl
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdexcept>
#include <string>
#include "ngx_c_conf.h"
#include "ngx_macro.h"
#include "ngx_global.h"
#include "ngx_func.h"
#include "ngx_c_memory.h"
#include "ngx_c_crc32.h"
#include "ngx_logiccomm.h"  
#include "ngx_c_lockmutex.h" 
int	MsgPack::m_nSN = 1;

ByteArray::ByteArray(int uSize)
{
	mData = new char[uSize];
	memset(mData, 0, uSize);
	mCur = 0;
	mLen = 0;
	mMax = uSize;
}
ByteArray::~ByteArray()
{
	delete []mData;
}
void ByteArray::setCur(int value)
{
	mCur = value;
}
void ByteArray::writeInt8(const int8_t& value)
{
	memcpy(mData+mLen, &value, sizeof(value));
	mLen += sizeof(value);
}
void ByteArray::writeInt16(const int16_t& value)
{
	memcpy(mData+mLen, &value, sizeof(value));
	mLen += sizeof(value);
}
void ByteArray::writeInt32(const int32_t& value)
{
	memcpy(mData+mLen, &value, sizeof(value));
	mLen += sizeof(value);
}
void ByteArray::writeInt64(const int64_t& value)
{
	memcpy(mData+mLen, &value, sizeof(value));
	mLen += sizeof(value);
}
void ByteArray::writeFloat(const float& value)
{
	memcpy(mData+mLen, &value, sizeof(value));
	mLen += sizeof(value);
}
void ByteArray::writeDouble(const double& value)
{
	memcpy(mData+mLen, &value, sizeof(value));
	mLen += sizeof(value);
}
void ByteArray::writeString(const std::string& value)
{
	int len = value.length();
	writeInt16(len);
	memcpy(mData+mLen, value.c_str(), len);
	mLen += len;
}
void ByteArray::writeBytes(const char* buff, int len)
{
	if (buff && len) {
		memcpy(mData+mLen, buff, len);
		mLen += len;
	}
}

int8_t ByteArray::readInt8()
{
	int8_t ret = 0;
	memcpy(&ret, mData + mCur, sizeof(ret));
	mCur += sizeof(ret);
	return ret;
}
int16_t ByteArray::readInt16()
{
	int16_t ret = 0;
	memcpy(&ret, mData + mCur, sizeof(ret));
	mCur += sizeof(ret);
	return ret;
}
int32_t  ByteArray::readInt32()
{
	int32_t ret = 0;
	memcpy(&ret, mData + mCur, sizeof(ret));
	mCur += sizeof(ret);
	return ret;
}
int64_t  ByteArray::readInt64()
{
	int64_t ret = 0;
	memcpy(&ret, mData + mCur, sizeof(ret));
	mCur += sizeof(ret);
    //ret=ntohll(ret);
	return ret;
}
float    ByteArray::readFloat()
{
	float ret = 0;
	memcpy(&ret, mData + mCur, sizeof(ret));
	mCur += sizeof(ret);
	return ret;
}
double   ByteArray::readDouble()
{
	double ret = 0;
	memcpy(&ret, mData + mCur, sizeof(ret));
	mCur += sizeof(ret);
	return ret;
}
std::string ByteArray::readString()
{
	int len = readInt16();
	char* cStr = new char[len + 1];
	memset(cStr,0,len + 1);
	readBytes(cStr, len);
	std::string s(cStr);
	delete[] cStr;
	return s;
}
void ByteArray::readBytes(char* destBuf, int len)
{
	if (destBuf && len) {
		memcpy(destBuf, mData+mCur, len);
		mCur += len;
	}
}


MsgPack::MsgPack(LPSTRUC_MSG_HEADER msgHand,int16_t msgID, int32_t _id,int16_t sertype, int16_t len) 
	: ByteArray(len)
{
	m_msgID = msgID;
    mMsgHand=msgHand;
    m_id=_id;
    m_apkHander = sizeof(COMM_PKG_HEADER);
    m_msgHander = sizeof(STRUC_MSG_HEADER);
}
bool MsgPack::send(){

    LPCOMM_PKG_HEADER pPkgHeader;	
	CMemory  *p_memory = CMemory::GetInstance();
	CCRC32   *p_crc32 = CCRC32::GetInstance();
 
    char *p_sendbuf = (char *)p_memory->AllocMemory(m_msgHander+m_apkHander+getLen(),false);//准备发送的格式，这里是 消息头+包头+包体
    //b)填充消息头
    memcpy(p_sendbuf,mMsgHand,m_msgHander);                   //消息头直接拷贝到这里来
    //c)填充包头
    pPkgHeader = (LPCOMM_PKG_HEADER)(p_sendbuf+m_msgHander);    //指向包头
    pPkgHeader->msgCode = m_msgID;	                        //消息代码，可以统一在ngx_logiccomm.h中定义
    pPkgHeader->msgCode = htons(pPkgHeader->msgCode);	            //htons主机序转网络序 
    pPkgHeader->pkgLen  = htons(m_apkHander + getLen());        //整个包的尺寸【包头+包体尺寸】 
    pPkgHeader->isComeSever=true;
    pPkgHeader->sServerType=1;
    pPkgHeader->sServerType=htons(pPkgHeader->sServerType);
    pPkgHeader->_id = htonl(m_id);
    //d)填充包体
    char* p_sendInfo = (p_sendbuf+m_msgHander+m_apkHander);	//跳过消息头，跳过包头，就是包体了
    //memset(p_sendInfo,1,iSendLen);
    //。。。。。这里根据需要，填充要发回给客户端的内容,int类型要使用htonl()转，short类型要使用htons()转；
    memcpy(p_sendInfo,getData(),getLen()); 
    //e)包体内容全部确定好后，计算包体的crc32值
    pPkgHeader->crc32   = p_crc32->Get_CRC((unsigned char *)p_sendInfo,getLen());
    pPkgHeader->crc32   = htonl(pPkgHeader->crc32);		
    ngx_log_stderr(0,"222222222222!");
    struct sockaddr_in *sock = ( struct sockaddr_in*)&mMsgHand->pConn->s_sockaddr;
    ngx_log_stderr(0,"sendto,连接ip：%s;连接port：%d\n",inet_ntoa(sock->sin_addr),ntohs(sock->sin_port));
    //f)发送数据包
    g_socket.msgSend(p_sendbuf);

    return true;
}
