#pragma once

#include "Stdfax.h"
#include "Singleton.h"
#include "PacketManager.h"
#include "ErrorLogger.h"
#include "SocketStructure.h"

class NetworkManager : public Singleton<NetworkManager>
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;

	SOCKET clntSock;
	sockaddr_in servAddr;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;
	DWORD transferredBytes;
	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

public:
	void init();
	void connectWithServer();
	void sendMessage(const char* message);
	void receiveMessage();
	void handleIO();
};

