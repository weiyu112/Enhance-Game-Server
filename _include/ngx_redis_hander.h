#ifndef __NGX_REDIS_HANDER_H__
#define __NGX_REDIS_HANDER_H__
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <hiredis/hiredis.h>

class Redis
{
    public:
        Redis();
        ~Redis();
        bool connect(std::string host, int port);
        std::string get(std::string key);
        void set(std::string key, std::string value);
        std::string hget(const char* key,const  char* hkey);
        int existsKey(const char* ID);
        int del(const char* key);
        int hset(const char* key,const char* hkey,const char* hvalue, size_t hvaluelen);
        int hset(const char* key, const char* hkey, const char* value);
        void lpush(std::string key, std::string value);
        int lget(std::string key,int begin_index,int end_index);
    private:
        redisContext* _connect;
        redisReply* _reply;

};


#endif