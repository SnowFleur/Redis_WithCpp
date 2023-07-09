#pragma once

/*
- Developer: PDH
- Descriptor: 싱글턴을 사용할 클래스가 상속받는 싱글턴 Class
*/

#include<mutex>

template<class _Ty>
class Singleton 
{
private:
    static _Ty* instance_;
    static std::once_flag onceFlag_; // 여러 스레드에서 불려도 1번만 불리게 
protected:
    Singleton() = default;
public:
    virtual ~Singleton() = default;

    static _Ty* GetInstance() 
    {
        std::call_once(onceFlag_, []()
            {
                if (instance_ == nullptr)
                {
                    instance_ = new _Ty();
                }
            });
        return instance_;
    }

    static void Release()
    {
        if (instance_ != nullptr)
        {
            delete instance_;
            instance_ = nullptr;
        }
    }

};


template<class _Ty>
_Ty* Singleton<_Ty>::instance_ = nullptr;

template<class _Ty>
std::once_flag Singleton<_Ty>::onceFlag_;


