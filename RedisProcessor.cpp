#include"RedisProcessor.h"
#include"common/LogCollector.h"

RedisProcessor::RedisProcessor(std::shared_ptr<redisContext*> pRedisConnector) :
	DBProcessor{}
	, pRedisConnector_(pRedisConnector)
{}

redisContext* RedisProcessor::GetRedisContextPointer() const
{
	return *pRedisConnector_.get();
}

bool RedisProcessor::IsConnectRedisServer() const
{
	return  GetRedisContextPointer() == nullptr ? false : true;
}

void RedisProcessor::ExcuteQuery(RedisQuery* pDBContext, DBContextCallBack<RedisResult> dbResult)
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
	dbResult(result);
}

redisReply* RedisProcessor::ExcuteRedisCommand(const char* pCommand)
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
