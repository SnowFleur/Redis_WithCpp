#pragma once
#include"RedisQuery.h"


class RedisZSet : public RedisQuery
{
private:
	int16_t		startIndex_;
	int16_t		endIndex_;
private:
	/* Zset에 데이터를 추가 */
	bool InsertZSet()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "ZADD %s %s", GetKey().c_str(), GetValue().c_str());
		return SetRedisCommand(command);
	}

	/* Zset에 데이터 가져오기 */
	bool GetZSetMembers(bool withScroes)
	{
		char command[MAX_COMMAND_SIZE];
		if (withScroes == true)
		{
			sprintf_s(command, MAX_COMMAND_SIZE, "ZRANGE %s %d %d withscores", GetKey().c_str(), startIndex_, endIndex_);
		}
		else
		{
			sprintf_s(command, MAX_COMMAND_SIZE, "ZRANGE %s %d %d", GetKey().c_str(), startIndex_, endIndex_);
		}
		return SetRedisCommand(command);
	}

	/* Zset에 있는 데이터 업데이트*/
	bool UpdateZSet()
	{
		return InsertZSet();
	}

	/* Zset에 있는 데이터 삭제*/
	bool DeleteZSet()
	{
		char command[MAX_COMMAND_SIZE];
		sprintf_s(command, MAX_COMMAND_SIZE, "ZREM  %s %s", GetKey().c_str(), GetValue().c_str());
		return SetRedisCommand(command);
	}

public:
	RedisZSet(const REDIS_TYPE type, const std::string key, const std::string& value)
		:RedisQuery(type, key, value), startIndex_(0), endIndex_(0) {}
	RedisZSet(const REDIS_TYPE type, const std::string key)
		:RedisQuery(type, key), startIndex_(0), endIndex_(0) {}
	RedisZSet(const REDIS_TYPE type, const std::string key, const int16_t startIndex, const int16_t endIndex)
		:RedisQuery(type, key),startIndex_(startIndex),endIndex_(endIndex){}

	bool CreateRedisCommand()override
	{
		if (GetRedisType() == REDIS_TYPE::INSERT_ZSET)
		{
			return InsertZSet();
		}
		else if (GetRedisType() == REDIS_TYPE::READ_ZSET)
		{
			return GetZSetMembers(false);
		}
		else if (GetRedisType() == REDIS_TYPE::READ_ZSET_WITH_SCORE)
		{
			return GetZSetMembers(true);
		}
		else if (GetRedisType() == REDIS_TYPE::UPDATE_ZSET)
		{
			return UpdateZSet();
		}
		else if (GetRedisType() == REDIS_TYPE::DELETE_ZSET)
		{
			return DeleteZSet();
		}
		return false;
	}

};

