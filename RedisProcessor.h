#pragma once

#include<map>
#include"RedisConnector.h"
#include"RedisQuery.h"

constexpr int MAX_COMMAND_SIZE = 1024;


class RedisProcessor : public DBProcessor<RedisQuery, RedisResult>
{
private:
	std::map<REDIS_TYPE, std::string>		rediscommand_;
	std::shared_ptr<redisContext*>				pRedisConnector_;
private:
	redisContext*	GetRedisContextPointer()const;
	bool			IsConnectRedisServer()const;
public:
	RedisProcessor(std::shared_ptr<redisContext*> pRedisConnector);
public:
	void ExcuteQuery(RedisQuery* pDBContext, DBContextCallBack<RedisResult> dbResult)override;
	redisReply* ExcuteRedisCommand(const char* pCommand);

	/*Redis CROUD*/
	bool					InsertData(std::string* pCommand, RedisQuery* pRedisQuery);
	SharedPtrRedisReplay	GetValue(std::string* pCommand, RedisQuery* pRedisQuery);
	bool					UpdateData(std::string* pCommand, RedisQuery* pRedisQuery);
	bool					DeleteData(std::string* pCommand, RedisQuery* pRedisQuery);
	bool					PrintMembers(std::string* pCommand, RedisQuery* pRedisQuery);
};

#define CHECK_CONNECT_REDIS  if(IsConnectRedisServer()==false) 