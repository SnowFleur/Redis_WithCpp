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
							std::cout << "커스텀삭제자 호출\n";
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
	//결과전송
	dbResult(result);
}

redisReply* RedisProcessor::ExcuteRedisCommand(const char* pCommand)
{

	try
	{
		if (pRedisConnector_ == nullptr)	return nullptr;
		if (pCommand == nullptr)			return nullptr;

		// hiredis는 Raw포인터를 받기 때문에 여기부터는 스마트포인터가 아닌 Raw로 전달
		// 객체 수명관리는 Hiredis를 제외한 곳에서 사용할 때 사용하기 위해 
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
