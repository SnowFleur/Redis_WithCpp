#pragma once
#include<string>
#include<memory>
#include"RedisConnector.h"
using SharedPtrRedisReplay = std::shared_ptr<redisReply>;

enum class REDIS_TYPE : uint8_t
{
	//기본적인 CRUD
	//Set
	INSERT_SET = 1
	, READ_SET
	, UPDATE_SET
	, DELETE_SET
	//String
	, INSERT_STRING
	, READ_STRING
	, UPDATE_STRING
	, DELETE_STRING
	//ZSet
	, INSERT_ZSET
	, READ_ZSET
	, READ_ZSET_WITH_SCORE
	, UPDATE_ZSET
	, DELETE_ZSET

};

constexpr int MAX_COMMAND_SIZE = 1024;

class RedisQuery
{
private:
	const REDIS_TYPE type_;
	const std::string key_;
	const std::string value_;
	char command_[MAX_COMMAND_SIZE];
public:
	RedisQuery(const REDIS_TYPE type, const std::string key, const std::string& value) : type_(type), key_(key), value_(value)
	{
		memset(command_, 0, MAX_COMMAND_SIZE);
	}
	RedisQuery(const REDIS_TYPE type, const std::string& key) : type_(type), key_(key), value_("")
	{
		memset(command_, 0, MAX_COMMAND_SIZE);
	}
	
	~RedisQuery() 
	{
		memset(command_, 0, MAX_COMMAND_SIZE);
	}


	REDIS_TYPE GetRedisType()const
	{
		return type_;
	}

	const std::string GetKey()const
	{
		return key_;
	}

	const std::string GetValue()const
	{
		return value_;
	}

	bool SetRedisCommand(const char* pCommand)
	{
		if (pCommand != nullptr)
		{
			memcpy_s(command_, MAX_COMMAND_SIZE, pCommand, MAX_COMMAND_SIZE);
			return true;
		}
		return false;
	}
	
	const char* GetRedisCommand()const
	{
		return command_;
	}

	virtual bool CreateRedisCommand() = 0;
	
};

class RedisResult
{
private:
public:
	REDIS_TYPE				redsiType_;
	SharedPtrRedisReplay	pData_;
	bool					result_;
public:
	RedisResult() :redsiType_(), pData_(nullptr), result_(false) {}
};



