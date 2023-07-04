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
HiReids ������ : http://redisgate.kr/redis/clients/hiredis_intro.php
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
	bool					InsertSet(const std::string& key, const std::string& value);											/* Set�� �����͸� �߰� */
	SharedPtrRedisReplay	GetSetMembers(const std::string& key);																	/* Set�� ������ �������� */
	bool					UpdateSet(const std::string& key, const std::string& value);											/* Set�� �ִ� ������ ������Ʈ*/
	bool					DeleteSet(const std::string& key, const std::string& value);											/* Set�� �ִ� ������ ����*/
	void					PrintSetMembers(const std::string& key);																/* Set�� �ִ� ������ ��� ��� */

	/*Redis Strings(key-value) Struct CRUD Function */
	bool					InsertString(const std::string& key, const std::string& value, const int32_t expireSecondTime = -1);	/* Strings�� �����͸� �߰� */
	SharedPtrRedisReplay	GetStringMembers(const std::string& key);																/* Strings�� ������ �������� */
	bool					UpdateString(const std::string& key, const std::string& value);											/* Strings�� �ִ� ������ ������Ʈ*/
	bool					DeleteString(const std::string& key);																	/* Strings�� �ִ� ������ ����*/

	/*Redis ZSet Struct CRUD Function */
	bool					InsertZSet(const std::string& key, const std::string& value);											/* ZSet�� �����͸� �߰� */
	SharedPtrRedisReplay	GetZSetMembers(const std::string& key);																	/* ZSet�� ������ �������� */
	bool					UpdateZSet(const std::string& key, const std::string& value);											/* ZSet�� �ִ� ������ ������Ʈ*/
	bool					DeleteZSet(const std::string& key, const std::string& value);											/* ZSet�� �ִ� ������ ����*/
	void					PrintZSetMembers(const std::string& key);																/* ZSet�� �ִ� ������ ��� ��� */


};

#define CHECK_CONNECT_REDIS  if(IsConnectRedisServer()==false) 