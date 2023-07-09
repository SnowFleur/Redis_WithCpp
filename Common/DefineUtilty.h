#pragma once
/*
- Developer: PDH
- Descriptor: �������� ��ũ�� ��ɵ�
-
*/

//Link: https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
//https://docs.microsoft.com/ko-kr/cpp/preprocessor/predefined-macros?view=msvc-160

#include<memory>

#define VER_CPP_20      _MSVC_LANG==202002L
#define VER_CPP_17      _MSVC_LANG==201703L
#define VER_CPP_14      _MSVC_LANG==201402L
#define VER_CPP_11      _MSVC_LANG==201103L
#define VER_CPP_LEGACY  _MSVC_LANG<201103L

#define SAFE_DELETE(x)          if(x!=nullptr)delete x;
#define SAFE_DELETE_MULTI(x)    if(x!=nullptr)delete[] x;



#define SHARED_PTR(x)   using x##SharedPtr		= std::shared_ptr<x>
#define WEAK_PTR(x)		using x##WeakPtr		= std::weak_ptr<x>
#define UNIQUE_PTR(x)   using x##UniquePtr		= std::unique_ptr<x>

#if !VER_CPP_LEGACY  //C++11�̻� �����ϴ� �����Ϸ�
#define DELETE_COPY(name)                    \
public:                                      \
name& operator=(const name&)    = delete;    \
name(const name&)               = delete;                     

#define DELETE_MOVE(name)                    \
public:                                      \
name& operator=(name&&)noexcept =delete;     \
name(name&&)noexcept            =delete;      
#elif VER_CPP_LEGACY
#define DELETE_COPY(name)                    \
private:                                     \
name& operator=(const name&);                \
name(const name&);                     
#endif


//#define CPP_SERVER	//C++�� C#�� �������� ���ݾ� �־ ũ�ν� ������ ���� define


// Session �Ҹ��ڰ� �� ȣ��Ǵ��� Ȯ�ο�
//#define PRINT_DEBUG_SESSION_DESTRUCTOR

// �޸� Ǯ�� �ƴ� New/Delete �Լ��� ����ϰڴ�. 
#define USED_NEW_DELETE_FUNCTION

// DisconnectEx �α� Ȯ�ο�
#define TRACE_ASYNC_DISCONNECT

// �� ������ ��� ������ ����ϴ� ���� �Լ��� ����� �ϰڴ�.
//define _PRINT_ALL_BUFFER       

// SEND IO Ȯ�ο�
//#define TRACE_ASYNC_SEND

// RECV IO Ȯ�ο�
//#define TRACE_ASYNC_RECV

