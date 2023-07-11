#pragma once
#include<hiredis.h>
#include"DBBase.h"

#ifdef _DEBUG
#pragma comment(lib,"hiredis.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#pragma comment(lib,"hiredisd.lib")
#endif

class RedisQuery;
class RedisResult;

class RedisConnector : public DBConnector<RedisQuery, RedisResult>
{
private:
	std::shared_ptr<redisContext*>	pRedisConnector_;
	std::string						redisServerIP_;
	std::string						password_;
	uint16_t						port_;
	uint32_t						timeout_;
private:
	redisContext*   GetRedisContextPointer()const;
	bool			CheckRedisReply(redisReply* pRedisReplay);
	bool			IsConnectRedisServer()const;
	void			PrintRedisErrorMessage(const int32_t errorCode)const;
	redisReply*		ExcuteRedisCommand(const char* pCommand);
public:
	RedisConnector(const std::string& redisServerIP, const uint16_t port, const std::string& password, const uint32_t timeout);
	~RedisConnector();
public:
	bool Connect()override;
	bool Disconnect()override;
	void Excute(RedisQuery* pDBContext, DBContextCallBack<RedisResult> pCallBackFunction = nullptr)override;
};

#define CHECK_CONNECT_REDIS  if(IsConnectRedisServer()==false) 