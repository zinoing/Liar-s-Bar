#pragma once

#include "Stdfax.h"
#include "SocketStructure.h"

class ClientInfo
{
	string uniqueKey; // uniqueKey used to identify the client in the server connection
	string id; // id of the client or user used in the chat session

	LPPER_HANDLE_DATA handleData;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;

	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

public:
	ClientInfo(string uniqueKey, LPPER_HANDLE_DATA handleData);
	~ClientInfo();
	void setId(string clientId);
	string getUniqueKey();
	void sendMessage(const char* message);
	void receiveMessage();
	void closeSocket();
};