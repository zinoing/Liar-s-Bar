#pragma once

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "Stdfax.h"
#include "Singleton.h"

class DatabaseConnector : public Singleton<DatabaseConnector>
{
    const string server = "tcp://127.0.0.1:3306";
    const string name = "root";
    const string password = "0000";

    sql::Driver* driver;
    unique_ptr<sql::Connection> conn;
    
    void init();

public:
    bool logIn(string id, string password);
    bool registerUser(string id, string password);
    void runDatabaseConnector();
    void closeDatabaseConnector();
};