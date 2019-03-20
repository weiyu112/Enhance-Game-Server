#ifndef __NET_ByteArray_H__
#define __NET_ByteArray_H__

#include <memory>
#include <stdint.h>
#include <stdexcept>
#include <string.h>
#include<vector>
#include"ngx_c_socket.h"

class ByteArray
{
public:	
	ByteArray(int uSize = 1024);

	~ByteArray();
	char* getData(){return mData;}
	int   getCur(){return mCur;}
	int   getMax(){return mMax;}
	int   getLen(){return mLen;}

	void resizeTo(int32_t size);
	void clear();

	void  setCur(int value);
	void  setLen(int val){mLen = val;}

	template<class T>
	void writeNum(const T& value)
	{
		memcpy(mData+mLen, &value, sizeof(value));
		mLen += sizeof(value);
	}

	template<class T>
	T readNum()
	{
		T ret = 0;
		memcpy(&ret, mData+mCur, sizeof(ret));
		mCur += sizeof(ret);
		return ret;
	}

	void writeInt8(const int8_t& value);
	//void writeUInt8(const uint8_t& value);

	void writeInt16(const int16_t& value);
	//void writeUInt16(const uint16_t& value);

	void writeInt32(const int32_t& value);
	//void writeUInt32(const uint32_t& value);

	void writeInt64(const int64_t& value);
	//void writeUInt64(const uint64_t& value);

	void writeFloat(const float& value);
	void writeDouble(const double& value);
	void writeString(const std::string& value);
	void writeBytes(const char* buff, int len);

	int8_t		readInt8();
	//uint8_t	 readUInt8();
	int16_t		readInt16();
	//uint16_t readUInt16();
	int32_t		readInt32();
	//uint32_t readUInt32();	
	int64_t		readInt64();
	//uint64_t readUInt64();
	float		readFloat();
	double		readDouble();
	std::string readString();
	void		readBytes(char* destBuf, int len);

private:
	char* mData;
	int mCur;
	int mLen;
	int mMax;
    
};

#define MSG_HEAD_ENCODE		sizeof(int16_t)
#define MSG_HEAD_DATALEN	sizeof(int16_t)

class MsgPack : public ByteArray
{
public:
	MsgPack(LPSTRUC_MSG_HEADER msgHand,int16_t msgID, int32_t _id,int16_t sertype, int16_t len=1024); 
	// 	: ByteArray(len)
	// {
	// 	m_msgID = msgID;
    //     mMsgHand=msgHand;
    //     m_apkHander = sizeof(COMM_PKG_HEADER);
    //     m_msgHander = sizeof(STRUC_MSG_HEADER);
	// }
	~MsgPack(){}

	int16_t	getHeadLen()
	{
		return MSG_HEAD_ENCODE + MSG_HEAD_DATALEN;
	}
	int16_t getDataLen()
	{
		int16_t dataLen = getLen() - getHeadLen();
		return dataLen;
	}
	void	beginWrite()
	{

	}
	void	endWrite()
	{
		int32_t dataLen = getDataLen();
		memcpy(getData() + MSG_HEAD_ENCODE, &dataLen, sizeof(int16_t));		
	}

	int16_t	getMsgID() {return m_msgID;}
	void setMsgID(int32_t id)
	{
		m_msgID = id;
	}
    LPSTRUC_MSG_HEADER getMsgHader(){return mMsgHand;}
	void setOpCode(int16_t val) {m_opCode = val;}
	int16_t getOpCode() {return m_opCode;}
    int32_t getId(){return m_id;}
    pthread_mutex_t * getLogicMutex(){return &mMsgHand->pConn->logicPorcMutex;}


    bool send();

	static int32_t getNextSN(){ return m_nSN++; }

private:
	int16_t			m_msgID;
	int16_t			m_opCode;
	static int32_t	m_nSN;
    int32_t         m_id;
    int16_t         m_sertype;
    LPSTRUC_MSG_HEADER mMsgHand;
    size_t m_apkHander;
    size_t m_msgHander;
};

#endif