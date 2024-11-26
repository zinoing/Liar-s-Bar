#pragma once

#include "Stdfax.h"
#include "SocketStructure.h"

class ClientInfo
{
	string uniqueKey;

	LPPER_HANDLE_DATA handleData;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;

	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

public:
	ClientInfo(string uniqueKey, LPPER_HANDLE_DATA handleData);
	~ClientInfo();
	string getUniqueKey();
	void sendMessage(const char* message);
	void receiveMessage();
	void closeSocket();
};