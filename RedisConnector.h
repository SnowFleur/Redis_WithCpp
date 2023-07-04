#pragma once

#include<hiredis.h>
#include<memory>
#include<string>
#include<iostream>

#ifdef _DEBUG
#pragma comment(lib,"hiredis.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#pragma comment(lib,"hiredisd.lib")
#endif

/*
HiReids 참고문서 : http://redisgate.kr/redis/clients/hiredis_intro.php
*/

using SharedPtrRedisReplay = std::shared_ptr<redisReply>;

constexpr int MAX_COMMAND_SIZE = 1024;

class RedisConnector
{

private:
	std::unique_ptr<redisContext*>	pRedisConnector_;
	redisContext*					raw_;
	std::string						redisServerIP_;
	std::string						password_;
	uint16_t						port_;
	uint32_t						timeout_;
private:
	redisContext*	GetRedisContextPointer()const;
	bool			CheckRedisReply(redisReply* pRedisReplay);
	bool			IsConnectRedisServer()const;
	void			PrintRedisErrorMessage(const int32_t errorCode)const;
	redisReply*		ExcuteRedisCommand(const char* pCommand);
public:
	RedisConnector(const std::string& redisServerIP, const uint16_t port, const std::string& password, const uint32_t timeout);
	~RedisConnector();
public:
	bool	ConnectRedisServer();
	bool	DisconnectRedisServer();
	void	SetAuth();

	/*Redis Set Structs CRUD  Function*/
	bool					InsertSet(const std::string& key, const std::string& value);											/* Set에 데이터를 추가 */
	SharedPtrRedisReplay	GetSetMembers(const std::string& key);																	/* Set에 데이터 가져오기 */
	bool					UpdateSet(const std::string& key, const std::string& value);											/* Set에 있는 데이터 업데이트*/
	bool					DeleteSet(const std::string& key, const std::string& value);											/* Set에 있는 데이터 삭제*/
	void					PrintSetMembers(const std::string& key);																/* Set에 있는 데이터 모두 출력 */

	/*Redis Strings(key-value) Struct CRUD Function */
	bool					InsertString(const std::string& key, const std::string& value, const int32_t expireSecondTime = -1);	/* Strings에 데이터를 추가 */
	SharedPtrRedisReplay	GetStringMembers(const std::string& key);																/* Strings에 데이터 가져오기 */
	bool					UpdateString(const std::string& key, const std::string& value);											/* Strings에 있는 데이터 업데이트*/
	bool					DeleteString(const std::string& key);																	/* Strings에 있는 데이터 삭제*/

	/*Redis ZSet Struct CRUD Function */
	bool					InsertZSet(const std::string& key, const std::string& value);											/* ZSet에 데이터를 추가 */
	SharedPtrRedisReplay	GetZSetMembers(const std::string& key);																	/* ZSet에 데이터 가져오기 */
	bool					UpdateZSet(const std::string& key, const std::string& value);											/* ZSet에 있는 데이터 업데이트*/
	bool					DeleteZSet(const std::string& key, const std::string& value);											/* ZSet에 있는 데이터 삭제*/
	void					PrintZSetMembers(const std::string& key);																/* ZSet에 있는 데이터 모두 출력 */


};

#define CHECK_CONNECT_REDIS  if(IsConnectRedisServer()==false) 