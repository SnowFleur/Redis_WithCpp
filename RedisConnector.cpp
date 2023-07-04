#include"RedisConnector.h"

RedisConnector::RedisConnector(const std::string& redisServerIP, const uint16_t port,
	const std::string& password, const uint32_t timeout)
{
	pRedisConnector_ = nullptr;

	redisServerIP_ = redisServerIP;
	port_ = port;
	password_ = password;
	timeout_ = timeout;
}

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
	case  REDIS_ERR_IO:			{std::cout << "REDIS_ERR_IO: 레디스 서버와 연결할 수 없습니다.\n"; break; }
	case  REDIS_ERR_OTHER:		{std::cout << "REDIS_ERR_OTHER: 그 외 모든 오류??\n"; break; }
	case  REDIS_ERR_EOF:		{std::cout << "REDIS_ERR_EOF: 레디스 서버와 연결이 끊겼습니다.\n"; break; }
	case  REDIS_ERR_PROTOCOL:	{std::cout << "REDIS_ERR_PROTOCOL: 레디스 프로토콜 에러입니다.C\n"; break; }
	case  REDIS_ERR_OOM:		{std::cout << "REDIS_ERR_OOM: 레디스서버 혹은 클라이언트의 메모리가 부족합니다.\n"; break; }
	case  REDIS_ERR_TIMEOUT:	{std::cout << "REDIS_ERR_TIMEOUT: 데이터 전송의 제한시간이 지났습니다.\n"; break; }
	default:
		break;
	}
}

redisReply* RedisConnector::ExcuteRedisCommand(const char* pCommand)
{
	CHECK_CONNECT_REDIS return nullptr;
	try
	{
		redisReply* pRedisReplay = static_cast<redisReply*> (redisCommand(GetRedisContextPointer(), pCommand));
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
		std::cout << "ExcuteRedisCommand error\n";
	}
	return nullptr;
}

bool RedisConnector::ConnectRedisServer()
{
	redisContext* pTemp = redisConnect(redisServerIP_.c_str(), port_);

	if (pTemp != nullptr)
	{
		pRedisConnector_ = std::make_unique<redisContext*>(pTemp);
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

bool RedisConnector::DisconnectRedisServer()
{
	if (pRedisConnector_ != nullptr)
	{
		redisFree(GetRedisContextPointer());
		pRedisConnector_.release();
		pRedisConnector_ = nullptr;
		return true;
	}
	return false;
}

/* AUTH */
void RedisConnector::SetAuth()
{
	if (IsConnectRedisServer() == false) return;
	redisReply* pRedisReplay = static_cast<redisReply*>(redisCommand(GetRedisContextPointer(), "AUTH %s", password_));
}

/*****************************************
************SET 자료구조 CRUD ***********
******************************************/
bool RedisConnector::InsertSet(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "SADD %s %s", key.c_str(), value.c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		freeReplyObject(pRedisReplay);
		return true;
	}
	return false;
}

SharedPtrRedisReplay RedisConnector::GetSetMembers(const std::string& key)
{
	CHECK_CONNECT_REDIS return nullptr;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "SMEMBERS %s", key.c_str());

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

bool RedisConnector::UpdateSet(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;

	return false;
}

bool RedisConnector::DeleteSet(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "SREM %s %s", key.c_str(), value.c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		freeReplyObject(pRedisReplay);
		return true;
	}
	return false;
}

void RedisConnector::PrintSetMembers(const std::string& key)
{
	CHECK_CONNECT_REDIS return;

	redisReply* pRedisReplay = static_cast<redisReply*>(redisCommand(GetRedisContextPointer(), "SMEMBERS %s", key.c_str()));

	std::cout << key << " : ";
	for (size_t i = 0; i < pRedisReplay->elements; ++i)
	{
		std::cout << pRedisReplay->element[i]->str << ", ";
	}
	std::cout << "\n";

	freeReplyObject(pRedisReplay);
}

/*****************************************
************ Strings 자료구조 CRUD ***********
******************************************/

bool RedisConnector::InsertString(const std::string& key, const std::string& value, const int32_t expireSecondTime)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	if (expireSecondTime == -1)
	{
		sprintf_s(command, MAX_COMMAND_SIZE, "SET %s %s", key.c_str(), value.c_str());
	}
	else
	{
		sprintf_s(command, MAX_COMMAND_SIZE, "SET %s %s EX %d", key.c_str(), value.c_str(), expireSecondTime);
	}

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		freeReplyObject(pRedisReplay);
		return true;
	}
	return false;
}

SharedPtrRedisReplay RedisConnector::GetStringMembers(const std::string& key)
{
	CHECK_CONNECT_REDIS return nullptr;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "GET %s", key.c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		return std::shared_ptr<redisReply>(pRedisReplay, [](redisReply* reply)
			{
				std::cout << " GetStringMembers 커스텀삭제자 호출\n";
				freeReplyObject(reply);
			});
	}

	return nullptr;
}

bool RedisConnector::UpdateString(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;
	return InsertString(key, value);
}

bool RedisConnector::DeleteString(const std::string& key)
{
	CHECK_CONNECT_REDIS return false;

	char command[MAX_COMMAND_SIZE];
	sprintf_s(command, MAX_COMMAND_SIZE, "DEL  %s", key.c_str());

	redisReply* pRedisReplay = ExcuteRedisCommand(command);
	if (pRedisReplay != nullptr)
	{
		freeReplyObject(pRedisReplay);
		return true;
	}
	return false;
}

/*****************************************
************ ZSet 자료구조 CRUD ***********
******************************************/

bool RedisConnector::InsertZSet(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;
}

SharedPtrRedisReplay RedisConnector::GetZSetMembers(const std::string& key)
{
	CHECK_CONNECT_REDIS return nullptr;
}

bool RedisConnector::UpdateZSet(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;
}

bool RedisConnector::DeleteZSet(const std::string& key, const std::string& value)
{
	CHECK_CONNECT_REDIS return false;
}

void RedisConnector::PrintZSetMembers(const std::string& key)
{
	CHECK_CONNECT_REDIS return;
}
