#include"RedisProcessor.h"
#include"common/LogCollector.h"

RedisProcessor::RedisProcessor(std::shared_ptr<redisContext*> pRedisConnector) :
	DBProcessor{}
	, pRedisConnector_(pRedisConnector)
{
	//Set
	rediscommand_.emplace(REDIS_TYPE::INSERT_SET, "SADD");
	rediscommand_.emplace(REDIS_TYPE::READ_SET, "SMEMBERS");
	//rediscommand_.emplace(REDIS_TYPE::UPDATE_SET, "SADD");
	rediscommand_.emplace(REDIS_TYPE::DELETE_SET, "SREM");
	rediscommand_.emplace(REDIS_TYPE::PRINT_SET, "SMEMBERS");
	// String
	rediscommand_.emplace(REDIS_TYPE::INSERT_STRING, "SET");
	rediscommand_.emplace(REDIS_TYPE::READ_STRING, "GET");
	//rediscommand_.emplace(REDIS_TYPE::UPDATE_STRING, "SADD");
	rediscommand_.emplace(REDIS_TYPE::DELETE_STRING, "DEL");
	//rediscommand_.emplace(REDIS_TYPE::PRINT_STRING, "SMEMBERS");

	// Zset
	rediscommand_.emplace(REDIS_TYPE::INSERT_ZSET, "ZADD");
	rediscommand_.emplace(REDIS_TYPE::READ_ZSET, "ZRANGE");
	//rediscommand_.emplace(REDIS_TYPE::UPDATE_ZSET, "SADD");
	rediscommand_.emplace(REDIS_TYPE::DELETE_ZSET, "ZREM");
	//rediscommand_.emplace(REDIS_TYPE::PRINT_ZSET, "SMEMBERS");

}

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

	RedisResult result;
	result.redsiType_ = pDBContext->GetRedisType();
	result.result_ = false;

	try
	{
		//C
		if (pDBContext->GetRedisType() < REDIS_TYPE::INSERT_MAX)
		{
			result.result_ = InsertData(&rediscommand_[pDBContext->GetRedisType()], pDBContext);
		}
		//R
		else if (pDBContext->GetRedisType() < REDIS_TYPE::READ_MAX)
		{
			result.pData_ = GetValue(&rediscommand_[pDBContext->GetRedisType()], pDBContext);;
			if (result.pData_ != nullptr)
			{
				result.result_ = true;
			}
		}
		//U
		else if (pDBContext->GetRedisType() < REDIS_TYPE::UPDATE_MAX)
		{
			result.result_ = UpdateData(&rediscommand_[pDBContext->GetRedisType()], pDBContext);
		}
		//D
		else if (pDBContext->GetRedisType() < REDIS_TYPE::DELETE_MAX)
		{
			result.result_ = DeleteData(&rediscommand_[pDBContext->GetRedisType()], pDBContext);
		}
		else if (pDBContext->GetRedisType() < REDIS_TYPE::PRINT_MAX)
		{
			result.result_ = PrintMembers(&rediscommand_[pDBContext->GetRedisType()], pDBContext);
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
		if (pRedisConnector_ == nullptr) return nullptr;

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


bool RedisProcessor::InsertData(std::string* pCommand, RedisQuery* pRedisQuery)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "%s %s %s", pCommand->c_str(), pRedisQuery->GetKey().c_str(), pRedisQuery->GetValue().c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		freeReplyObject(pRedisReplay);
		return true;
	}
	return false;
}

SharedPtrRedisReplay RedisProcessor::GetValue(std::string* pCommand, RedisQuery* pRedisQuery)
{
	CHECK_CONNECT_REDIS return nullptr;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "%s %s", pCommand->c_str(), pRedisQuery->GetKey().c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		return std::shared_ptr<redisReply>(pRedisReplay, [](redisReply* reply)
			{
				std::cout << " GetSetMembers 커스텀삭제자 호출\n";
				freeReplyObject(reply);
			});
	}
	return nullptr;
}

bool RedisProcessor::UpdateData(std::string* pCommand, RedisQuery* pRedisQuery)
{
	CHECK_CONNECT_REDIS return false;

	return false;
}

bool RedisProcessor::DeleteData(std::string* pCommand, RedisQuery* pRedisQuery)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "%s %s %s", pCommand->c_str(), pRedisQuery->GetKey().c_str(), pRedisQuery->GetValue().c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	//실제로 삭제를 성공했을 때 보내야하지 않을까?
	if (pRedisReplay != nullptr)
	{
		freeReplyObject(pRedisReplay);
		return true;
	}
	return false;
}

bool RedisProcessor::PrintMembers(std::string* pCommand, RedisQuery* pRedisQuery)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "%s %s", pCommand->c_str(), pRedisQuery->GetKey().c_str());

	std::cout << pRedisQuery->GetKey()<< " : ";
	redisReply* pRedisReplay = ExcuteRedisCommand(command);

	for (size_t i = 0; i < pRedisReplay->elements; ++i)
	{
		std::cout << pRedisReplay->element[i]->str << ", ";
	}
	std::cout << "\n";

	freeReplyObject(pRedisReplay);
	return true;
}

