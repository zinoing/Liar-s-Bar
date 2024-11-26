// RelayServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include "IOCPServer.h"

using namespace std;

int main()
{
	cout << "************* Running Connectors *************\n";
	LoginServerConnector* loginServerConnector = LoginServerConnector::getInstance();

	loginServerConnector->runLoginServerConnector("127.0.0.1", LOGIN_SERVER_PORT);
	cout << "LoginServerConnector is functioning properly\n";

	cout << "All connectors are successfully initialized and functioning properly\n";

	PacketManager* packetManager = PacketManager::getInstance();
	packetManager->runPacketManager();

	IOCPServer* server = IOCPServer::getInstance();
	server->runIOCPServer();

}