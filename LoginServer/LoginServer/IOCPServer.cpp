#include "IOCPServer.h"

void IOCPServer::init()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed with error: " <<  WSAGetLastError() << "\n";
		return;
	}

	hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hCompPort == NULL) {
		cout << "CreateIoCompletionPort failed with error: " << GetLastError() << "\n";
		return;
	}
	GetSystemInfo(&sysInfo);

	for (int i = 0; i < 16; ++i) {
		workerThreads.emplace_back([this]() { workerThread(); });
	}

	// Initialize socket
	listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSock == INVALID_SOCKET) {
		cout << "WSASocket failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		return;
	}

	cout << "RelayServer initialized successfully\n";
	return;
}

void IOCPServer::bindAndListen()
{
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(8001);

	int iResult = bind(listenSock, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		cout << "bind failed with error: " << WSAGetLastError();
		closesocket(listenSock);
		WSACleanup();
		return;
	}

	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
		cout << "listen function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		return;
	}

	cout << "RelayServer has been successfully bound and is now listening.\n";
	return;
}

void IOCPServer::acceptConnection()
{
	while (1) {
		cout << "\rWaiting for another client\n";

		relayServSock = accept(listenSock, &relayServAddr, &iRelayServAddrSize);

		if (relayServSock == INVALID_SOCKET) {
			cout << "accept failed with error: " << WSAGetLastError() << "\n";
			closesocket(listenSock);
			WSACleanup();
			return;
		}

		printf("Client Accepted\n");

		LPPER_HANDLE_DATA handleInfo = new PER_HANDLE_DATA();
		handleInfo->sock = relayServSock;
		handleInfo->sockAddr = *((struct sockaddr_in*)&relayServAddr);

		CreateIoCompletionPort((HANDLE)relayServSock, hCompPort, (ULONG_PTR)&handleInfo, 0);

		receiveMessage();
	}
}

void IOCPServer::sendMessage(const char* message)
{
	memset(sendBuffer, 0, sizeof(message));
	strcpy_s(sendBuffer, message);

	sendDataBuf.len = strlen(message);
	sendDataBuf.buf = sendBuffer;

	PER_IO_DATA* ioInfo = new PER_IO_DATA();
	memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
	ioInfo->wsaBuf.buf = sendDataBuf.buf;
	ioInfo->wsaBuf.len = strlen(message);
	ioInfo->rwMode = WRITE;

	DWORD transferredBytes = 0;
	DWORD flags = 0;

	int rc = WSASend(relayServSock,
		&sendDataBuf,
		1,
		&transferredBytes,
		flags,
		(LPWSAOVERLAPPED) & (ioInfo->overlapped),
		NULL);

	if ((rc == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
		cout << "WSASend failed with error: " << WSAGetLastError() << "\n";
	}

	return;
	return;
}

void IOCPServer::receiveMessage()
{
	memset(recvBuffer, 0, DATA_BUF_SIZE);
	recvDataBuf.len = DATA_BUF_SIZE;
	recvDataBuf.buf = recvBuffer;

	PER_IO_DATA* ioInfo = new PER_IO_DATA();
	memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
	ioInfo->wsaBuf.buf = ioInfo->buffer;
	ioInfo->wsaBuf.len = DATA_BUF_SIZE;
	ioInfo->rwMode = READ;

	DWORD transferredBytes = 0;
	DWORD flags = 0;

	int rc = WSARecv(relayServSock,
		&(ioInfo->wsaBuf),
		1,
		&transferredBytes,
		(LPDWORD)&flags,
		(LPWSAOVERLAPPED) & (ioInfo->overlapped),
		NULL);

	if ((rc == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
		cout << "WSARecv failed with error: " << WSAGetLastError() << "\n";
		return;
	}
	return;
}

void IOCPServer::workerThread()
{
	DWORD transferredBytes = 0;

	LPPER_HANDLE_DATA handleInfo = nullptr;
	LPPER_IO_DATA completedIOInfo = nullptr;

	while (1) {
		BOOL success = GetQueuedCompletionStatus(hCompPort, 
			&transferredBytes, 
			(PULONG_PTR)&handleInfo,
			(LPOVERLAPPED*)&completedIOInfo,
			INFINITE);
		
		if (!success || transferredBytes == 0) {
			int err = WSAGetLastError();
			if (err == ERROR_NETNAME_DELETED || err == WSAESHUTDOWN) {
				cout << "Client disconnected." << endl;
				closesocket(relayServSock);
				delete completedIOInfo;
				continue;
			}
			else {
				cout << "GetQueuedCompletionStatus failed: " << err << endl;
				break;
			}
		}

		if (completedIOInfo->rwMode == READ) {
			cout << "Read data: " << completedIOInfo->buffer << endl;
			receiveMessage();
		}
		else if (completedIOInfo->rwMode == WRITE) {
			cout << "Sent message successfully" << endl;
		}

		delete completedIOInfo;
	}

	return;
}

void IOCPServer::runIOCPServer()
{
	init();
	bindAndListen();
	acceptConnection();
}
