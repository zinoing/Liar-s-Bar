// GameServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include "IOCPServer.h"

using namespace std;

int main()
{
    cout << "Start Game Server\n\n";

    PacketManager* packetManager = PacketManager::getInstance();
    packetManager->runPacketManager();

    IOCPServer* server = IOCPServer::getInstance();
    server->runIOCPServer();
}