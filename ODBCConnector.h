#pragma once
#include"DBBase.h"

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

class ODBCProcessor : public DBProcessor<ODBCQuery, ODBCResult>
{
public:
	ODBCProcessor() :DBProcessor{} {}
	void ExcuteQuery(ODBCQuery* pDBContext, DBContextCallBack<ODBCResult> dbResult)override 
	{
	
	}
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
	bool Connect()override 
	{

	}
	bool Disconnect()override 

	{
	
	}
};




