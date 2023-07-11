#include<iostream>
#include"RedisConnector.h"
#include"RedisQuery.h"
#include"RedisString.h"
#include"RedisSet.h"
#include"RedisZset.h"

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
		g_pDBConnector->Disconnect();
	}
	else 
	{
		std::cout << "커넥트 실패\n";
	}
}

void Result(RedisResult result)
{

	if (result.result_ == true)
	{
		std::cout << "성공\n";

		if (result.redsiType_ == REDIS_TYPE::READ_SET) 
		{
			for (size_t i = 0; i < result.pData_->elements; ++i)
			{
				std::cout << result.pData_->element[i]->str << ", ";
			}
			std::cout << "\n";
		}

		else if (result.redsiType_ == REDIS_TYPE::READ_ZSET )
		{
			for (size_t i = 0; i < result.pData_->elements; ++i)
			{
				std::cout << result.pData_->element[i]->str << ", ";
			}
			std::cout << "\n";
		}

		else if (result.redsiType_ == REDIS_TYPE::READ_ZSET_WITH_SCORE)
		{
			std::cout << "-------------------------------------------\n";
			for (size_t i = 0; i < result.pData_->elements; i += 2)
			{
				std::cout << result.pData_->element[i + 1]->str << " -> " << result.pData_->element[i]->str << " \n";
			}
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
		RedisSet query(REDIS_TYPE::INSERT_SET, TEST_KEY, TEST_VALUE);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		RedisSet query(REDIS_TYPE::DELETE_SET, TEST_KEY, "B2");
		g_pDBConnector->Excute(&query, Result);
	}

	{
		RedisSet query(REDIS_TYPE::READ_SET, TEST_KEY);
		g_pDBConnector->Excute(&query, Result);
	}

	//포인터로 받지 말까?  --> 이동생성자 고민
	//g_pDBConnector->Excute(&RedisQuery(REDIS_TYPE::INSERT_SET, TEST_KEY, TEST_VALUE), Result);
}

void Test_ZSet()
{

	// http://redisgate.kr/redis/command/zrem.php 
	// 해당 URL 시뮬레이(문장 띄어쓰기만 제외)

	const std::string  TEST_KEY = "MYCOM";
	const std::string  INSERT_VALUE1 = "2009 Sun_microsystems 1992 Wang 2002 Netscape";
	const std::string  INSERT_VALUE2 = "1998 Digital_Equipment 2005 K-mart 1987 American_motors";
	const std::string  DELETE_VALUE1 = "American _otors Wang Digital_Equipment";

	{
		// Insert
		RedisZSet query(REDIS_TYPE::INSERT_ZSET, TEST_KEY, INSERT_VALUE1);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		// Insert
		RedisZSet query(REDIS_TYPE::INSERT_ZSET, TEST_KEY, INSERT_VALUE2);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		// Read
		RedisZSet query(REDIS_TYPE::READ_ZSET_WITH_SCORE, TEST_KEY, 0, -1);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		// Delete
		RedisZSet query(REDIS_TYPE::DELETE_ZSET, TEST_KEY, DELETE_VALUE1);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		// Read
		RedisZSet query(REDIS_TYPE::READ_ZSET_WITH_SCORE, TEST_KEY, 0, -1);
		g_pDBConnector->Excute(&query, Result);
	}


}

void Test_String()
{
	const std::string  TEST_KEY = "STRING_ABC";
	const std::string  TEST_VALUE = "ABC";
	for (int32_t i = 0; i < 10; ++i)
	{
		//Insert 
		std::string key = TEST_KEY + std::to_string(i);
		std::string value = TEST_VALUE + std::to_string(i);

		RedisString query(REDIS_TYPE::INSERT_STRING, key, value);
		g_pDBConnector->Excute(&query, Result);
	}

	{
		// 삭제 테스트
		RedisString query(REDIS_TYPE::DELETE_STRING, "STRING_ABC");
		g_pDBConnector->Excute(&query, Result);
	}
	
	{
		// 가져오기 테스트(ABC5)
		RedisString query(REDIS_TYPE::READ_STRING, "STRING_ABC5");
		g_pDBConnector->Excute(&query, Result);
	}

}