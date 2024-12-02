#pragma once

#include "Stdfax.h"
#include "SocketStructure.h"
#include "ErrorLogger.h"

class ServerConnector
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;

	SOCKET servConnectSock;
	sockaddr_in servConnectAddr;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;
	DWORD transferredBytes;
	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

private:
	void init();
	void connectWithServer(const char* ipAddress, const u_short port);

	void handleIO();

public:
	void sendMessage(const char* message);
	void receiveMessage();
	virtual void runConnector(const char* ipAddress, const u_short port);
};

