#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DATA_BUF_SIZE 4096

#define READ 2
#define WRITE 3

#include <iostream>
#include <winsock2.h>
#include <thread>

#include "Singleton.h"

using namespace std;

typedef struct {
	SOCKET acceptSock;
	sockaddr_in acceptAddr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[DATA_BUF_SIZE];
	int rwMode;
} PER_IO_DATA, * LPPER_IO_DATA;

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

