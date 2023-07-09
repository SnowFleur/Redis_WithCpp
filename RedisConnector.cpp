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
	case  REDIS_ERR_IO:			{PRINT_ERROR_LOG("REDIS_ERR_IO: ���� ������ ������ �� �����ϴ�.\n"); break; }
	case  REDIS_ERR_OTHER:		{PRINT_ERROR_LOG("REDIS_ERR_OTHER: �� �� ��� ����??\n"); break; }
	case  REDIS_ERR_EOF:		{PRINT_ERROR_LOG("REDIS_ERR_EOF: ���� ������ ������ ������ϴ�.\n"); break; }
	case  REDIS_ERR_PROTOCOL:	{PRINT_ERROR_LOG("REDIS_ERR_PROTOCOL: ���� �������� �����Դϴ�.\n"); break; }
	case  REDIS_ERR_OOM:		{PRINT_ERROR_LOG("REDIS_ERR_OOM: ���𽺼��� Ȥ�� Ŭ���̾�Ʈ�� �޸𸮰� �����մϴ�.\n"); break; }
	case  REDIS_ERR_TIMEOUT:	{PRINT_ERROR_LOG("REDIS_ERR_TIMEOUT: ������ ������ ���ѽð��� �������ϴ�.\n"); break; }
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
