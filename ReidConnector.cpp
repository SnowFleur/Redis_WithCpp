#include"common/LogCollector.h"
#include"RedisConnector.h"
#include"RedisProcessor.h"

RedisConnector::RedisConnector(const std::string& redisServerIP, const uint16_t port, const std::string& password, const uint32_t timeout)
	: pRedisConnector_(nullptr)
	, redisServerIP_(redisServerIP)
	, port_(port)
	, password_(password)
	, timeout_(timeout)
	, DBConnector{}
{}

RedisConnector::~RedisConnector()
{
	if (pRedisConnector_ != nullptr)
	{
		redisFree(GetRedisContextPointer());
	}
}

redisContext* RedisConnector::GetRedisContextPointer() const
{
	return *pRedisConnector_.get();
}

bool RedisConnector::CheckRedisReply(redisReply* pRedisReplay)
{
	if (pRedisReplay == nullptr) {
		printf("redisCommand pRedisReplay is NULL: %s\n", GetRedisContextPointer()->errstr);
		return false;
	}

	if (pRedisReplay->type == REDIS_REPLY_ERROR) {
		printf("Command Error: %s\n", pRedisReplay->str);
		freeReplyObject(pRedisReplay);
		return false;
	}
	return true;
}

bool RedisConnector::IsConnectRedisServer() const
{
	return  GetRedisContextPointer() == nullptr ? false : true;
}

void RedisConnector::PrintRedisErrorMessage(const int32_t errorCode) const
{
	switch (errorCode)
	{
	case  REDIS_ERR_IO:			{PRINT_ERROR_LOG("REDIS_ERR_IO: 레디스 서버와 연결할 수 없습니다.\n"); break; }
	case  REDIS_ERR_OTHER:		{PRINT_ERROR_LOG("REDIS_ERR_OTHER: 그 외 모든 오류??\n"); break; }
	case  REDIS_ERR_EOF:		{PRINT_ERROR_LOG("REDIS_ERR_EOF: 레디스 서버와 연결이 끊겼습니다.\n"); break; }
	case  REDIS_ERR_PROTOCOL:	{PRINT_ERROR_LOG("REDIS_ERR_PROTOCOL: 레디스 프로토콜 에러입니다.\n"); break; }
	case  REDIS_ERR_OOM:		{PRINT_ERROR_LOG("REDIS_ERR_OOM: 레디스서버 혹은 클라이언트의 메모리가 부족합니다.\n"); break; }
	case  REDIS_ERR_TIMEOUT:	{PRINT_ERROR_LOG("REDIS_ERR_TIMEOUT: 데이터 전송의 제한시간이 지났습니다.\n"); break; }
	default:
		break;
	}
}

bool RedisConnector::Connect()
{
	redisContext* pTemp = redisConnect(redisServerIP_.c_str(), port_);

	if (pTemp != nullptr)
	{
		pRedisConnector_ = std::make_unique<redisContext*>(pTemp);

		RedisProcessor* pRedisProcessor = new RedisProcessor(pRedisConnector_);
		if (CreateDBProcessor(pRedisProcessor) == false) 
		{
			delete(pRedisProcessor);
			return false;
		}
	}

	if (pTemp == nullptr)
	{
	}
	else if (pTemp->err)
	{
		PrintRedisErrorMessage(pTemp->err);
		return false;
	}

	else if (pRedisConnector_ == nullptr)
	{
	}
	else
	{
		return true;
	}
	return false;

}

bool RedisConnector::Disconnect()
{
	if (pRedisConnector_ != nullptr)
	{
		redisFree(GetRedisContextPointer());
		pRedisConnector_.reset();
		pRedisConnector_ = nullptr;
		return true;
	}
	return false;
}
