#pragma once
/*
- Developer: PDH
- Descriptor: DB Ŀ���͵��� ��ӹ޴� ���̽� Ŭ������
*/

#include<functional>

template<class _TDBResult>
using DBContextCallBack = std::function<void(_TDBResult)>;

template<class _TDBContext, class _TDBResult>
class DBConnector
{
private:
	//DBProcessor< _TDBContext, _TDBResult>* dbProcessor_;
public:
	DBConnector() = default;
	virtual bool Connect() = 0;
	virtual bool Disconnect() = 0;
	virtual void Excute(_TDBContext* pDBContext, DBContextCallBack<_TDBResult> pCallBackFunction = nullptr) = 0;
};



