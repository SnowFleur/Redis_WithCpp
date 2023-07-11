#include"common/LogCollector.h"
#include"RedisConnector.h"
#include"RedisQuery.h"

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

redisReply* RedisConnector::ExcuteRedisCommand(const char* pCommand)
{

	try
	{
		if (pRedisConnector_ == nullptr)	return nullptr;
		if (pCommand == nullptr)			return nullptr;

		// hiredis�� Raw�����͸� �ޱ� ������ ������ʹ� ����Ʈ�����Ͱ� �ƴ� Raw�� ����
		// ��ü ��������� Hiredis�� ������ ������ ����� �� ����ϱ� ���� 
		redisReply* pRedisReplay = static_cast<redisReply*> (redisCommand(*pRedisConnector_.get(), pCommand));
		if (pRedisReplay != nullptr)
		{
			if (REDIS_REPLY_ERROR == pRedisReplay->type)
			{
				freeReplyObject(pRedisReplay);
				return nullptr;
			}
			return pRedisReplay;
		}
	}
	catch (...)
	{
		PRINT_ERROR_LOG("ExcuteRedisCommand error\n");
	}
	return nullptr;
}


bool RedisConnector::Connect()
{
	redisContext* pTemp = redisConnect(redisServerIP_.c_str(), port_);

	if (pTemp != nullptr)
	{
		pRedisConnector_ = std::make_unique<redisContext*>(pTemp);

		if (pRedisConnector_ == nullptr)
		{
			return false;
		}
		return true;
	}
	else if (pTemp->err)
	{
		PrintRedisErrorMessage(pTemp->err);
		return false;
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

void RedisConnector::Excute(RedisQuery* pDBContext, DBContextCallBack<RedisResult> pCallBackFunction)
{
	CHECK_CONNECT_REDIS return;

	RedisResult result{};

	try
	{
		if (pDBContext->CreateRedisCommand() == true)
		{
			result.redsiType_ = pDBContext->GetRedisType();
			redisReply* pRedisReplay = ExcuteRedisCommand(pDBContext->GetRedisCommand());
			if (pRedisReplay != nullptr)
			{
				result.result_ = true;

				//Read Data
				if (pRedisReplay->type == REDIS_REPLY_STRING || pRedisReplay->type == REDIS_REPLY_ARRAY)
				{
					result.pData_ = std::shared_ptr<redisReply>(pRedisReplay, [](redisReply* reply)
						{
							std::cout << "Ŀ���һ����� ȣ��\n";
							freeReplyObject(reply);
						});
				}
				else
				{
					freeReplyObject(pRedisReplay);
				}
			}
			else
			{
				std::cout << pDBContext->GetKey() << ": ";
				result.result_ = false;
			}
		}
	}
	catch (...)
	{
		PRINT_ERROR_LOG("ExcuteQuery error\n");
	}
	//�������
	pCallBackFunction(result);
}
