#include<iostream>
#include"RedisConnector.h"

int F();

void Test_Set();
void Test_String();
void Test_ZSet();


RedisConnector g_redisConnector{ "127.0.0.1", 6379 ,"",5000 };

int main()
{

	//레디스 접속
	if (g_redisConnector.ConnectRedisServer() == true)
	{
		Test_String();
		//Test_Set();

	};
	g_redisConnector.DisconnectRedisServer();
}

void Test_Zset() 
{
	const std::string  TEST_KEY = "ZSET_TEST";
	const std::string  TEST_VALUE = "A1 B2 T3 Q1 Q2 Q3 C1 C2 C3 D1 D2 D3 E1 E2";
}

void Test_String()
{
	const std::string  TEST_KEY = "STRING_ABC";
	const std::string  TEST_VALUE = "ABC";
	for (int32_t i = 0; i < 10; ++i)
	{
		std::string key = TEST_KEY + std::to_string(i);
		std::string value = TEST_VALUE + std::to_string(i);
		g_redisConnector.InsertString(key, value);
	}

	// 삭제 테스트
	if (g_redisConnector.DeleteString("STRING_ABC3") == true)
	{
		std::cout << "삭제 성공\n";
	}

	//// 가져오기 테스트
	//auto ptr = g_redisConnector.GetStringMembers("STRING_ABC5");
	//if (ptr != nullptr) 
	//{
	//	std::cout << "결과물: " << ptr->str << "\n";
	//}

}

void Test_Set()
{
	const std::string  TEST_KEY = "POINTER";
	const std::string  TEST_VALUE = "A1 B2 T3 Q1 Q2 Q3 C1 C2 C3 D1 D2 D3 E1 E2";

	g_redisConnector.InsertSet(TEST_KEY, TEST_VALUE);
	g_redisConnector.DeleteSet(TEST_KEY, "B2");
	auto ptr = g_redisConnector.GetSetMembers(TEST_KEY);
	

	//g_redisConnector.PrintSetMembers(TEST_KEY);
}
