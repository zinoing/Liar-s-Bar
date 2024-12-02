// LoginServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "IOCPServer.h"
#include "DatabaseConnector.h"

using namespace std;

int main()
{
    cout << "Start Login Server\n\n";

    DatabaseConnector* dbConnector = DatabaseConnector::getInstance();
    dbConnector->runDatabaseConnector();

    PacketManager* packetManager = PacketManager::getInstance();
    packetManager->runPacketManager();

    IOCPServer* server = IOCPServer::getInstance();
    server->runIOCPServer();
}