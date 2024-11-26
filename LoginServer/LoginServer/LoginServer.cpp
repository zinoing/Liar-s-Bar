// LoginServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "IOCPServer.h"
#include "DatabaseConnector.h"

using namespace std;

int main()
{
    DatabaseConnector* dbConnector = DatabaseConnector::getInstance();
    dbConnector->runDatabaseConnector();

    IOCPServer* server = IOCPServer::getInstance();
    server->runIOCPServer();
}