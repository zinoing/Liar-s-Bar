#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DATA_BUF_SIZE 4096

#define READ 2
#define WRITE 3

#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

using namespace std;

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

class IOCPServer
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

	void sendMessage(const char* message);
	void receiveMessage();

	void workerThread();

public:
	void runIOCPServer();
};

