#pragma once

#include "Stdfax.h"
#include "SocketStructure.h"

class ClientInfo
{
	string id; // id used to identify the client in the server connection

	LPPER_HANDLE_DATA handleData;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;

	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

public:
	ClientInfo(string id, LPPER_HANDLE_DATA handleData);
	~ClientInfo();
	void setId(string clientId);
	string getId();
	void sendMessage(const char* message);
	void receiveMessage();
	void closeSocket();
};