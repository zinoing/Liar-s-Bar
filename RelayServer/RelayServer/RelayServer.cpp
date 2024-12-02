// RelayServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include "IOCPServer.h"
#include "LoginServerConnector.h"
#include "GameServerConnector.h"

using namespace std;

int main()
{
	cout << "Start Relay Server\n\n";

	cout << "************* Running Connectors *************\n";

	LoginServerConnector* loginServerConnector = LoginServerConnector::getInstance();
	loginServerConnector->runLoginServerConnector();
	cout << "LoginServerConnector is functioning properly\n";

	GameServerConnector* gameServerConnector = GameServerConnector::getInstance();
	gameServerConnector->runGameServerConnector();
	cout << "GameServerConnector is functioning properly\n";

	cout << "All connectors are successfully initialized and functioning properly\n\n";

	PacketManager* packetManager = PacketManager::getInstance();
	packetManager->runPacketManager();

	IOCPServer* server = IOCPServer::getInstance();
	server->runIOCPServer();

}