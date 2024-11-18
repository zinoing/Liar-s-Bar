#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DATA_BUF_SIZE 4096
#define RELAY_SERVER_PORT 8000
#define LOGIN_SERVER_PORT 8001

#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

#include "ClientInfo.h"
#include "PacketManager.h"
#include "PacketBuffer.h"

using namespace std;

class ClientInfo;

class IOCPServer
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;

	SOCKET listenSock;
	sockaddr_in service;

	sockaddr acceptAddr;
	int iAcceptSize = sizeof(acceptAddr);
	SOCKET acceptSock;

	vector<ClientInfo*> clients;
	vector<thread> workerThreads;
private:
	void init();
	void bindAndListen();
	void acceptConnection();

	void sendMessage(ClientInfo clientInfo, const char* message);
	void receiveMessage(ClientInfo clientInfo);

	void workerThread();

public:	
	void runIOCPServer();
};

