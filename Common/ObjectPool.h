#pragma once
/*
- Developer: PDH
- Descriptor: 스마트포인터 기반 ObJect풀
- Shared 포인터를 사용하면서 GC로 관리를 할 수 있게 한다.
- To do operater new를 통한 메모리 할당 보강이 필요함
*/

#define PRINT_MEMORY_ADDRESS
#include<memory>
#include<new>
#include<stack>
#include<iostream>

#define VER2		//GetFreeObject함수에 매개변수가 없는 버전

template<class _Ty>
class ObjectPool
{
private:
	std::stack<std::unique_ptr<_Ty>>	objectPool_;
	size_t	objectPoolSize_;
public:
	ObjectPool(const uint32_t objectPoolSize):
		objectPoolSize_(objectPoolSize)
	{
	}

	virtual ~ObjectPool() 
	{
		objectPoolSize_ = 0;
	}

	template<class... _Args>
	void InitFreeObject(_Args&&... args) 
	{
		for (uint32_t i = 0; i < objectPoolSize_; ++i)
		{
			objectPool_.emplace(std::make_unique<_Ty>(std::forward<_Args>(args)...));
		}
	}

	void InitFreeObject(std::unique_ptr<_Ty> pFreeObject)
	{
		if (pFreeObject != nullptr)
		{
			objectPool_.emplace(std::move(pFreeObject));
		}
	}

	size_t GetObjectPoolSize()const
	{
		return objectPoolSize_;
	}

#ifdef VER2
	std::shared_ptr<_Ty> GetFreeObject()
	{
		std::unique_ptr<_Ty> pObject = nullptr;

		//비어있지 않다면 기존에 있는 데이터를준다.
		if (objectPool_.empty() == false)
		{
			pObject = std::move(objectPool_.top());
			objectPool_.pop();
		}
		else 
		{
			PRINT_ERROR_LOG(typeid(_Ty).name(), " Pool is Empty\n");
		}

		//8byte=(포인터+ 컨트롤블럭)
		std::shared_ptr<_Ty> pMemory
		(
			pObject.release()
			, [this](_Ty* p)
			{
				std::cout << "커스텀 삭제자\n";
				if (p != nullptr)
				{
					objectPool_.emplace(p);
				}
			}
		);
		return pMemory;
	}
#else
	template<class... _Args>
	std::shared_ptr<_Ty> GetFreeObject(_Args&&... args)
	{
		std::unique_ptr<_Ty> pObject = nullptr;

		//비어있지 않다면 기존에 있는 데이터를준다.
		if (objectPool_.empty() == false)
		{
			pObject = std::move(objectPool_.top());
		}
		//부족하다면 새로 만들자
		else
		{
			pObject = std::make_unique<_Ty>(std::forward<_Args>(args)...);
		}
		//8byte=(포인터+ 컨트롤블럭)
		std::shared_ptr<_Ty> pMemory
		(
			pObject.release()
			, [this](_Ty* p)
			{
				std::cout << "커스텀 삭제자\n";
				if (p != nullptr)
				{
					objectPool_.emplace(p);
				}
			}
		);
		return pMemory;
	}
#endif // VER2
	
};
