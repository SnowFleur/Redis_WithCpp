#pragma once
#include"DBBase.h"
#include<string>

class ODBCQuery
{
public:
};

class ODBCResult
{
private:
public:
	bool result_;
};


class ODBCConnector : public DBConnector<ODBCQuery, ODBCResult>
{
private:
	std::string						ODBCServerIP_;
	std::string						password_;
	uint16_t						port_;
public:
	ODBCConnector(const std::string& ODBCServerIP, const uint16_t port, const std::string& password, const uint32_t timeout) 
	{

	}
	~ODBCConnector() 
	{

	}
public:
	bool Connect()override {}
	bool Disconnect()override {}
	void Excute(ODBCQuery* pDBContext, DBContextCallBack<ODBCResult> pCallBackFunction)override {}

};




