#include"lua_net.h"

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}
#include"bytearray.h"
#include"LuaBridge.h"
#include"ngx_c_socket.h"
#include"ngx_func.h"
#include"ngx_c_lockmutex.h"


using namespace luabridge;


void nxg_debug(const char* _str)
{
    ngx_log_stderr(0,_str);
}
bool registerNetAbout(lua_State* tolua_S)
{
    getGlobalNamespace(tolua_S)
        .beginNamespace("net")
            .addFunction("nxg_debug",nxg_debug)
            .beginClass<pthread_mutex_t>("pthread_mutex_t")
                .addConstructor <void (*) (void)> ()
            .endClass()
            .beginClass<CLock>("CLock")
                .addConstructor<void(*)(pthread_mutex_t *)>()
                .addFunction("makeUnlock",&CLock::makeUnlock)
            .endClass()
            .beginClass<ngx_connection_s>("ngx_connection_s")
                .addConstructor <void (*) (void)> ()
                .addData("logicPorcMutex",&ngx_connection_s::logicPorcMutex)
            .endClass()
            .beginClass<_STRUC_MSG_HEADER>("_STRUC_MSG_HEADER")
                .addConstructor <void (*) (void)> ()
                .addData("pConn",&_STRUC_MSG_HEADER::pConn)
                .addData("iCurrsequence",&_STRUC_MSG_HEADER::iCurrsequence)
            .endClass()
            .beginClass<ByteArray>("ByteArray")
                .addConstructor<void(*)(int)>()
                .addFunction("writeInt8",&ByteArray::writeInt8)
                .addFunction("writeInt16",&ByteArray::writeInt16)
                .addFunction("writeInt32",&ByteArray::writeInt32)
                .addFunction("writeInt64",&ByteArray::writeInt64)
                .addFunction("writeFloat",&ByteArray::writeFloat)
                .addFunction("writeDouble",&ByteArray::writeDouble)
                .addFunction("writeString",&ByteArray::writeString)
                .addFunction("writeBytes",&ByteArray::writeBytes)
                .addFunction("readInt8",&ByteArray::readInt8)
                .addFunction("readInt16",&ByteArray::readInt16)
                .addFunction("readInt32",&ByteArray::readInt32)
                .addFunction("readInt64",&ByteArray::readInt64)
                .addFunction("readFloat",&ByteArray::readFloat)
                .addFunction("readDouble",&ByteArray::readDouble)
                .addFunction("readString",&ByteArray::readString)
                .addFunction("readBytes",&ByteArray::readBytes)
            .endClass()
            .deriveClass<MsgPack,ByteArray>("MsgPack")
                .addConstructor<void(*)(_STRUC_MSG_HEADER*,int16_t,int32_t,int16_t,int16_t)>()
                .addFunction("getMsgID",&MsgPack::getMsgID)
                .addFunction("getMsgHader",&MsgPack::getMsgHader)
                .addFunction("send",&MsgPack::send)
                .addFunction("getId",&MsgPack::getId)
                .addFunction("getLogicMutex",&MsgPack::getLogicMutex)
            .endClass()
        .endNamespace();
    

}