#pragma once
#include"RedisQuery.h"


class RedisSet : public RedisQuery
{
private:
	/* Set에 데이터를 추가 */
	bool InsertSet()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "SADD %s %s", GetKey().c_str(), GetValue().c_str());
		return SetRedisCommand(command);
	}

	/* Set에 데이터 가져오기 */
	bool GetSetMembers()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "SMEMBERS %s", GetKey().c_str());
		return SetRedisCommand(command);
	}

	/* Set에 있는 데이터 업데이트*/
	bool UpdateSet()
	{
		return InsertSet();
	}

	/* Set에 있는 데이터 삭제*/
	bool DeleteSet()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "SREM  %s %s", GetKey().c_str(), GetValue().c_str());
		return SetRedisCommand(command);
	}

public:
	RedisSet(const REDIS_TYPE type, const std::string key, const std::string& value)
		:RedisQuery(type, key, value) {}
	RedisSet(const REDIS_TYPE type, const std::string key)
		:RedisQuery(type, key) {}

	bool CreateRedisCommand()override
	{
		if (GetRedisType() == REDIS_TYPE::INSERT_SET)
		{
			return InsertSet();
		}
		else if (GetRedisType() == REDIS_TYPE::READ_SET)
		{
			return GetSetMembers();
		}
		else if (GetRedisType() == REDIS_TYPE::UPDATE_SET)
		{
			return UpdateSet();
		}
		else if (GetRedisType() == REDIS_TYPE::DELETE_SET)
		{
			return DeleteSet();
		}
		return false;
	}
};

