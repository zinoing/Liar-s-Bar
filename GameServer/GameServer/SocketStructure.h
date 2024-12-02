#pragma once

#include <winsock2.h>

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