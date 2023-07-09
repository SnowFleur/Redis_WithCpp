#pragma once
#include<string>
#include<memory>

using SharedPtrRedisReplay = std::shared_ptr<redisReply>;

/*각 CRUD마다 Range로 나눈것 MAX를 넘어서면 안됨!!*/
enum class REDIS_TYPE : uint8_t
{
	//Insert
	INSERT_SET = 1
	, INSERT_STRING
	, INSERT_ZSET
	, INSERT_MAX = 51
	//Read
	, READ_SET
	, READ_STRING
	, READ_ZSET
	, READ_MAX = 102
	//Update
	, UPDATE_SET
	, UPDATE_STRING
	, UPDATE_ZSET
	, UPDATE_MAX = 153
	// Delete
	, DELETE_SET
	, DELETE_STRING
	, DELETE_ZSET
	, DELETE_MAX = 204
	// Print 
	, PRINT_SET
	, PRINT_STRING
	, PRINT_ZSET
	, PRINT_MAX = 255
};

class RedisQuery
{
	const REDIS_TYPE type_;
	const std::string key_;
	const std::string value_;
public:
	RedisQuery(const REDIS_TYPE type, const std::string key, const std::string& value) : type_(type), key_(key), value_(value)
	{}
	RedisQuery(const REDIS_TYPE type, const std::string& key) : type_(type), key_(key), value_("")
	{}

	inline REDIS_TYPE GetRedisType()const 		{ return type_; }
	inline const std::string GetKey()const 		{ return key_; }
	inline const std::string GetValue()const 	{ return value_; }
};

class RedisResult
{
private:
public:
	REDIS_TYPE				redsiType_;
	SharedPtrRedisReplay	pData_;
	bool					result_;
};
