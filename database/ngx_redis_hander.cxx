/*
 * redis.cpp
 *
 *  Created on: 2018年6月7日
 *      Author: oftenlin
 */

#include "ngx_redis_hander.h"

Redis::Redis(){

}

Redis::~Redis()
{
    this->_connect = NULL;
    this->_reply = NULL;
}

bool Redis::connect(std::string host, int port)
{
    this->_connect = redisConnect(host.c_str(), port);
    if(this->_connect != NULL && this->_connect->err)
    {
        printf("connect error: %s\n", this->_connect->errstr);
        return 0;
    }
    return 1;
}

std::string Redis::get(std::string key)
{
    std::string str ="";
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
    if(this->_reply==NULL){
        return str;
    }
    str = this->_reply->str;
    freeReplyObject(this->_reply);
    return str;

}

void Redis::set(std::string key, std::string value)
{
    redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
}

void Redis::lpush(std::string key, std::string value)
{
    redisCommand(this->_connect, "LPUSH %s %s", key.c_str(), value.c_str());
}

int Redis::lget(std::string key,int begin_index,int end_index){
    /* Let's check what we have inside the list */
    redisReply* reply = (redisReply*) redisCommand(this->_connect,"LRANGE %s %d %d",key.c_str(),begin_index,end_index);
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (int j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
//            data.push_back(_reply->element[j]->str);

        }
    }
    freeReplyObject(reply);
    return 1;
}

std::string Redis::hget(const char* key,const  char* hkey){
    const char* argv[3];
    size_t argvlen[3];
    argv[0] = "HGET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    redisReply* reply =(redisReply*) redisCommandArgv(this->_connect, 3, argv, argvlen);
    std::string value;
    if(reply->type != REDIS_REPLY_NIL){
        value = std::string(reply->str,reply->str + reply->len);
    }
    freeReplyObject(reply);
    return value;
}
int Redis::hset(const char* key, const char* hkey, const char* value){
    redisReply* reply =(redisReply*) redisCommand(this->_connect, "HSET %s %s %s",key,hkey, value);
    freeReplyObject(reply);
    return 1;
}
int Redis::hset(const char* key,const char* hkey,const char* hvalue, size_t hvaluelen){
    const char* argv[4];
    size_t argvlen[4];
    argv[0] = "HSET";
    argvlen[0] = 4;
    argv[1] = key;
    argvlen[1] = strlen(key);
    argv[2] = hkey;
    argvlen[2] = strlen(hkey);
    argv[3] = hvalue;
    argvlen[3] = hvaluelen;
    redisReply * reply =(redisReply*) redisCommandArgv(this->_connect, 4, argv, argvlen);
    freeReplyObject(reply);
    return 1;
}

int Redis::del(const char* key){
    int res = 0;
    redisReply* reply = (redisReply*)redisCommand(this->_connect, "DEL %s", key);
    if(reply->type == REDIS_REPLY_INTEGER){
        if(reply->integer == 1L)
            res = 1;
    }
    freeReplyObject(reply);
    return res;
}

/*if Key ID exists*/
int Redis::existsKey(const char* ID){
    redisReply * reply = (redisReply*)redisCommand(this->_connect,"exists %s",ID);
    int res = 0;
    if(reply->type == REDIS_REPLY_INTEGER){
        if(reply->integer == 1L)
            res  = 1;
    }
    freeReplyObject(reply);
    return res;
}