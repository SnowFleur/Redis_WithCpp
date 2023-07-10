#pragma once

#include<map>
#include"RedisConnector.h"
#include"RedisQuery.h"

class RedisProcessor : public DBProcessor<RedisQuery, RedisResult>
{
private:
	std::shared_ptr<redisContext*>			pRedisConnector_;
private:
	redisContext*	GetRedisContextPointer()const;
	bool			IsConnectRedisServer()const;
public:
	RedisProcessor(std::shared_ptr<redisContext*> pRedisConnector);
public:
	void ExcuteQuery(RedisQuery* pDBContext, DBContextCallBack<RedisResult> dbResult)override;
	redisReply* ExcuteRedisCommand(const char* pCommand);
};

#define CHECK_CONNECT_REDIS  if(IsConnectRedisServer()==false) 