#pragma once
/*
- Developer: PDH
- Descriptor: DB 커넥터들이 상속받는 베이스 클래스들
*/


#include"common/ObjectPool.h"
#include<queue>
#include<functional>

template<class _TDBResult>
using DBContextCallBack = std::function<void(_TDBResult)>;

template<class _TDBContext, class _TDBResult>
class DBProcessor
{
private:
	//ObjectPool<DBQQuery> queryPool_;
public:
	DBProcessor() {};
	virtual void ExcuteQuery(_TDBContext* pDBContext, DBContextCallBack<_TDBResult> dbResult) = 0;
};

template<class _TDBContext, class _TDBResult>
class DBConnector
{
private:
	DBProcessor< _TDBContext, _TDBResult>* dbProcessor_;
public:
	DBConnector() :dbProcessor_(nullptr) {}
	virtual bool Connect() = 0;
	virtual bool Disconnect() = 0;

	bool CreateDBProcessor(DBProcessor< _TDBContext, _TDBResult>* dbProcessor)
	{
		if (dbProcessor_ == nullptr)
		{
			dbProcessor_ = dbProcessor;
			return true;
		}
		return false;
	}

	void Excute(_TDBContext* pDBContext, DBContextCallBack<_TDBResult> pCallBackFunction = nullptr)
	{
		dbProcessor_->ExcuteQuery(pDBContext, pCallBackFunction);
	}
};

