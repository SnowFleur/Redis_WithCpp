#pragma once
/*
- Developer: PDH
- Descriptor:  Log 출력 및 로그 저장용? --> Log Server를 만드는 것도 방법 (UDP)
- 컴파일 버전에 따른 버전처리
- Todo: File IO ,UDP 통신기능
*/
#include<iostream>
#include<time.h>
#include"WindowsHeader.h"
#include"DefineUtilty.h"
#include"Singleton.h"

class CLogCollector :public Singleton<CLogCollector>
{
private:
	tm	dateInfor_;
public:
	CLogCollector()
	{
#ifdef _DEBUG
		AllocConsole();
		FILE* consoleStream = nullptr;
		freopen_s(&consoleStream, "CONOUT$", "w", stdout);
#endif
		time_t timer = time(NULL);
		if (localtime_s(&dateInfor_, &timer) != 0)
		{
			std::cout << "현재 시간을 얻을 수 없습니다.\n";
		}
	}
	void UpdateTime()
	{
		time_t timer = time(NULL);
		if (localtime_s(&dateInfor_, &timer) != 0)
		{
			std::cout << "현재 시간을 얻을 수 없습니다.\n";
		}
	}

#if VER_CPP_17 // c++17이상 ( Fold expressions)
	template<class... _Args>
	void PrintLog(_Args... args)
	{
		//((std::cout << args << " "), ...) << "\n";
		UpdateTime();

		printf("[%04d-%02d-%02d-%02d:%02d:%02d] ",
			dateInfor_.tm_year + 1900, dateInfor_.tm_mon + 1, dateInfor_.tm_mday,
			dateInfor_.tm_hour, dateInfor_.tm_min, dateInfor_.tm_sec);
		((std::cout << args), ...);
	}

#elif VER_CPP_14 || VER_CPP_11  //C++ 14 || C++11
	template<class _Ty, class... _Args>
	void PrintLog(_Ty message, _Args... args)const
	{
		std::cout << message;
		PrintLog(args...);
	}

	template<class _Ty>
	void PrintLog(_Ty message)const
	{
		//std::cout << message << "\n";
		std::cout << message;
	}
#else
	//ERROR
#endif //  _MSC_VER 

};

/*
- Meesage: 출력 내용
*/
#define PRINT_LOG(message,...)          CLogCollector::GetInstance()->PrintLog(message,__VA_ARGS__)
#define PRINT_INFO_LOG(message,...)     CLogCollector::GetInstance()->PrintLog("[INFOR] ",message,__VA_ARGS__)
#define PRINT_ERROR_LOG(message,...)    CLogCollector::GetInstance()->PrintLog("[ERROR] ",message,__VA_ARGS__)


