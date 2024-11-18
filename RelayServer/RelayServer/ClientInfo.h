#pragma once

#define DATA_BUF_SIZE 4096

#define READ 2
#define WRITE 3

#include <winsock2.h>
#include "IOCPServer.h"

typedef struct {
	SOCKET sock;
	sockaddr_in sockAddr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[DATA_BUF_SIZE];
	int rwMode;
} PER_IO_DATA, * LPPER_IO_DATA;

class ClientInfo
{
	LPPER_HANDLE_DATA handleData;

	WSABUF sendDataBuf;
	WSABUF recvDataBuf;

	char sendBuffer[DATA_BUF_SIZE];
	char recvBuffer[DATA_BUF_SIZE];

public:
	ClientInfo(LPPER_HANDLE_DATA handleData);
	~ClientInfo();
	void sendMessage(const char* message);
	void receiveMessage();
	void closeSocket();
};

