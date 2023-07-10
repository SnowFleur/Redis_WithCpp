#pragma once

#include"RedisQuery.h"

class RedisString : public RedisQuery
{
private:
	int32_t expireSecondTime_;
private:
	/* Strings에 데이터를 추가 */
	bool InsertString()
	{
		char command[MAX_COMMAND_SIZE];
		if (expireSecondTime_ == -1)
		{
			sprintf_s(command, MAX_COMMAND_SIZE, "SET %s %s", GetKey().c_str(), GetValue().c_str());
		}
		else
		{
			sprintf_s(command, MAX_COMMAND_SIZE, "SET %s %s EX %d", GetKey().c_str(), GetValue().c_str(), expireSecondTime_);
		}
		return SetRedisCommand(command);
	}

	/* Strings에 데이터 가져오기 */
	bool GetStringMembers()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "GET %s", GetKey().c_str());
		return SetRedisCommand(command);
	}

	/* Strings에 있는 데이터 업데이트*/
	bool UpdateString()
	{
		return InsertString();
	}

	/* Strings에 있는 데이터 삭제*/
	bool DeleteString()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "DEL  %s", GetKey().c_str());
		return SetRedisCommand(command);
	}

public:

	RedisString(const REDIS_TYPE type, const std::string key, const std::string& value)
		:RedisQuery(type, key, value), expireSecondTime_(-1) {}
	RedisString(const REDIS_TYPE type, const std::string key)
		:RedisQuery(type, key), expireSecondTime_(-1) {}
	RedisString(const REDIS_TYPE type, const std::string key, const std::string& value, const int32_t expireSecondTime)
		:RedisQuery(type, key, value), expireSecondTime_(expireSecondTime) {}

	bool CreateRedisCommand()override
	{
		if (GetRedisType() == REDIS_TYPE::INSERT_STRING)
		{
			return InsertString();
		}
		else if (GetRedisType() == REDIS_TYPE::READ_STRING)
		{
			return GetStringMembers();
		}
		else if (GetRedisType() == REDIS_TYPE::UPDATE_STRING)
		{
			return UpdateString();
		}
		else if (GetRedisType() == REDIS_TYPE::DELETE_SET)
		{
			return DeleteString();
		}
		return false;
	}
};

