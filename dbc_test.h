#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace avhw32
{
	const std::string CONNECTION_STRING = "connection_string";
	const std::string BAD_STRING = "bad_string";
	const std::string LEGAL_QUERY = "legal_query";
	const std::string BAD_QUERY = "bad_query";
	const std::string QUERY_RESULT = "query_result";
	
	class I_DBConnection
	{
	public:
		I_DBConnection() {}
		virtual ~I_DBConnection() {}
		virtual bool open(const std::string& connection) = 0;
		virtual bool close() = 0;
		virtual std::string execQuery(const std::string& query) = 0;
	protected:
		bool connected = false;
	};

	class TestDBConnection : public I_DBConnection
	{
	public:
		TestDBConnection();
		virtual ~TestDBConnection();
		virtual bool open(const std::string& connection) override;
		virtual bool close() override;
		virtual std::string execQuery(const std::string& query) override;
	};

	class MockDBConnection : public I_DBConnection
	{
	public:
		MOCK_METHOD(bool, open, (const std::string& connection), (override));
		MOCK_METHOD(bool, close, (), (override));
		MOCK_METHOD(std::string, execQuery, (const std::string& query), (override));
	};

	class DBUser
	{
	public:
		DBUser();
		~DBUser();
		bool openConnection(const std::string& connection, I_DBConnection& dbc);
		bool useConnection(const std::string& query, I_DBConnection& dbc);
		bool closeConnection(I_DBConnection& dbc);
	};

	class DBConnectionTestSuite : public ::testing::Test
	{
	protected:
		void setUp();
		void tearDown();
		DBUser* user;
	};
}