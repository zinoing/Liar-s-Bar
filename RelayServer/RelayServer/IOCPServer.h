#pragma once

#include "Stdfax.h"
#include "ClientInfo.h"
#include "PacketManager.h"
#include "PacketBuffer.h"
#include "ErrorLogger.h"

#include <chrono>
#include <iomanip>
#include <sstream>

using namespace chrono;

class ClientInfo;

class IOCPServer : public Singleton<IOCPServer>
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;

	SOCKET listenSock;
	sockaddr_in service;

	sockaddr acceptAddr;
	int iAcceptSize = sizeof(acceptAddr);
	SOCKET acceptSock;

	unordered_map<string, ClientInfo*> clients;
	vector<thread> workerThreads;

private:
	void init();
	void bindAndListen();
	void acceptConnection();

	void sendMessage(ClientInfo* clientInfo, const char* message);
	void receiveMessage(ClientInfo* clientInfo);

	void workerThread();

	string createUniqueKey();
public:		
	void runIOCPServer();
	ClientInfo* getClientInfo(string id);
	void addClientInfo(string id);
};

