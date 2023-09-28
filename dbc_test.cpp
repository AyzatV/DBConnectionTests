#include "dbc_test.h"

namespace avhw32
{
	TestDBConnection::TestDBConnection() {}
	TestDBConnection::~TestDBConnection() {}

	bool TestDBConnection::open(const std::string& connection)
	{
		bool result = (!connected && connection.size() > 10);
		if (result) connected = true;
		return result;
	}

	bool TestDBConnection::close()
	{
		bool result = connected;
		if (result) connected = false;
		return result;
	}

	std::string TestDBConnection::execQuery(const std::string& query)
	{
		return (connected && query == LEGAL_QUERY) ? QUERY_RESULT : std::string();
	}

	DBUser::DBUser() {}

	DBUser::~DBUser() {}

	bool DBUser::openConnection(const std::string& connection, I_DBConnection& dbc) { return dbc.open(connection); }

	bool DBUser::useConnection(const std::string& query, I_DBConnection& dbc) { return (dbc.execQuery(query) == QUERY_RESULT); }

	bool DBUser::closeConnection(I_DBConnection& dbc) { return dbc.close(); }

	void DBConnectionTestSuite::setUp() { user = new DBUser(); }

	void DBConnectionTestSuite::tearDown() { delete user; }

	// open DB connection testing
	TEST_F(DBConnectionTestSuite, testcase1)
	{
		TestDBConnection tdbc;
		std::string connection = BAD_STRING;
		ASSERT_FALSE(user->openConnection(connection, tdbc));	// ������ ���������� �� �������������
		connection = CONNECTION_STRING;
		ASSERT_TRUE(user->openConnection(connection, tdbc));	// ������ ���������� �������������
		ASSERT_FALSE(user->openConnection(connection, tdbc));	// ��������� �������� ����������
	}

	// query DB testing
	TEST_F(DBConnectionTestSuite, testcase2)
	{
		TestDBConnection tdbc;
		std::string connection = CONNECTION_STRING;
		std::string query = LEGAL_QUERY;
		ASSERT_FALSE(user->useConnection(query, tdbc));			// ���������� ������ �� �������� ����������
		user->openConnection(connection, tdbc);
		ASSERT_TRUE(user->useConnection(query, tdbc));			// ���������� ������ ����� �������� ����������
		query = BAD_QUERY;
		ASSERT_FALSE(user->useConnection(query, tdbc));			// ������������ ������ ����� �������� ����������
	}

	// close DB connection testing
	TEST_F(DBConnectionTestSuite, testcase3)
	{
		TestDBConnection tdbc;
		std::string connection = CONNECTION_STRING;
		ASSERT_FALSE(user->closeConnection(tdbc));				// ������� �������� �� ��������� ����������
		user->openConnection(connection, tdbc);
		ASSERT_TRUE(user->closeConnection(tdbc));				// �������� ��������� ����������
	}

	// mock DB connection testing
	TEST_F(DBConnectionTestSuite, testcase4)
	{
		MockDBConnection mdbc;
		std::string connection = CONNECTION_STRING;
		std::string query = LEGAL_QUERY;
		
		// ��������� ����������� ����� �������� ���������� � ��������� �������������� ����������
		EXPECT_CALL(mdbc, open).WillOnce(::testing::Return(true));
		// ��������� ����������� ����� �������� ������� � ��������� ��������� ����������
		EXPECT_CALL(mdbc, execQuery).WillOnce(::testing::Return(QUERY_RESULT));
		// ��������� ����������� ����� �������� ���������� � ��������� �������������� ����������
		EXPECT_CALL(mdbc, close).WillOnce(::testing::Return(true));

		ASSERT_TRUE(user->openConnection(connection, mdbc));	// �������� ���������� � �������������� ���-�������
		ASSERT_TRUE(user->useConnection(query, mdbc));			// ������ � �������������� ���-�������
		ASSERT_TRUE(user->closeConnection(mdbc));				// �������� ���������� � �������������� ���-�������
	}
}