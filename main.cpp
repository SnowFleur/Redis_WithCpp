#include<iostream>
#include"RedisConnector.h"
#include"RedisQuery.h"

void Result(RedisResult result);
void Test_Set();
void Test_ZSet();
void Test_String();

DBConnector<RedisQuery, RedisResult>* g_pDBConnector = new RedisConnector{ "127.0.0.1", 6379, "", 5000 };

int main() 
{
	if (g_pDBConnector->Connect() == true)
	{
		std::cout << "커넥트 성공\n";
		//Test_String();
		Test_ZSet();
		//Test_Set();
	}
	g_pDBConnector->Disconnect();
}

void Result(RedisResult result)
{

	if (result.result_ == true)
	{

		std::cout << "성공\n";

		if (result.pData_ != nullptr && result.pData_->str != nullptr)
		{
			std::cout << "가져온데이터: " << result.pData_->str << "\n";
		}
	}
	else
	{
		std::cout << "실패 ㅡㅡ\n";
	}
}

void Test_Set()
{

	const std::string  TEST_KEY = "POINTER";
	const std::string  TEST_VALUE = "A1 B2 T3 Q1 Q2 Q3 C1 C2 C3 D1 D2 D3 E1 E2";

	{
		RedisQuery query(REDIS_TYPE::INSERT_SET, TEST_KEY, TEST_VALUE);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		RedisQuery query(REDIS_TYPE::DELETE_SET, TEST_KEY, "B2");
		g_pDBConnector->Excute(&query, Result);
	}

	{
		RedisQuery query(REDIS_TYPE::PRINT_SET, TEST_KEY);
		g_pDBConnector->Excute(&query, Result);
	}

	//포인터로 받지 말까?  --> 이동생성자 고민
	//g_pDBConnector->Excute(&RedisQuery(REDIS_TYPE::INSERT_SET, TEST_KEY, TEST_VALUE), Result);
}

void Test_ZSet()
{
	const std::string  TEST_KEY = "ZSET_TEST";
	const std::string  TEST_VALUE = "60 Korea 20 America 40 Japan 100 Germany 80 China";

	{
		RedisQuery query(REDIS_TYPE::INSERT_ZSET, TEST_KEY, TEST_VALUE);
		g_pDBConnector->Excute(&query, Result);
	}
}

void Test_String()
{
	const std::string  TEST_KEY = "STRING_ABC";
	const std::string  TEST_VALUE = "ABC";
	for (int32_t i = 0; i < 10; ++i)
	{
		std::string key = TEST_KEY + std::to_string(i);
		std::string value = TEST_VALUE + std::to_string(i);
		RedisQuery query(REDIS_TYPE::INSERT_STRING, key, value);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		// 삭제 테스트
		RedisQuery query(REDIS_TYPE::DELETE_STRING, "STRING_ABC", TEST_VALUE);
		g_pDBConnector->Excute(&query, Result);
	}
	
	{
		// 가져오기 테스트
		RedisQuery query(REDIS_TYPE::READ_STRING, "STRING_ABC5");
		g_pDBConnector->Excute(&query, Result);
	}

}