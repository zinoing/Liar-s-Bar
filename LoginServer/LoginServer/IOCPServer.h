#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DATA_BUF_SIZE 4096
#define RELAY_SERVER_PORT 8000
#define LOGIN_SERVER_PORT 8001

#define READ 2
#define WRITE 3

#include "Stdfax.h"
#include "SocketStructure.h"
#include "ErrorLogger.h"
#include "PacketManager.h"

class IOCPServer : public Singleton<IOCPServer>
{
private:
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;

	SOCKET listenSock;
	sockaddr_in service;

	sockaddr relayServAddr;
	int iRelayServAddrSize = sizeof(relayServAddr);
	SOCKET relayServSock;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;

	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

	vector<thread> workerThreads;

private:
	void init();
	void bindAndListen();
	void acceptConnection();

	void workerThread();

public:
	void sendMessage(const char* message);
	void receiveMessage();
	void runIOCPServer();
};

