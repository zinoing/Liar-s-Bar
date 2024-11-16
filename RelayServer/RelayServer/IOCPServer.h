#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DATA_BUF_SIZE 4096

#define READ 0
#define WRITE 1

#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;

typedef struct {
	SOCKET acceptSock;
	sockaddr_in acceptAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[DATA_BUF_SIZE];
	int rwMode;
} PER_IO_DATA, *LPPER_IO_DATA;

class IOCPServer
{
	WSADATA wsaData;
	HANDLE hCompPort;
	SYSTEM_INFO sysInfo;

	SOCKET listenSock;
	sockaddr_in service;

	sockaddr acceptAddr;
	int iAcceptSize = sizeof(acceptAddr);
	SOCKET acceptSock;

	WSABUF dataBuf;
	DWORD transferredBytes;
	char buffer[DATA_BUF_SIZE];
	int recvBytes, flags = 0;

public:
	void init();
	void bindAndListen();
	void acceptConnection();

	void sendMessage(const char message[DATA_BUF_SIZE]);
	void receiveMessage();

	DWORD WINAPI handleIOCompletionThread(LPVOID pCompPort);
	void runIOCPServer();
};

