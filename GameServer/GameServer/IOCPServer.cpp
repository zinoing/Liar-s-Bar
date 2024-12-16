#include "IOCPServer.h"

void IOCPServer::init()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		ErrorLogger::logAndExit("WSAStartup", WSAGetLastError());
		return;
	}

	hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hCompPort == NULL) {
		ErrorLogger::logAndExit("CreateIoCompletionPort", WSAGetLastError());
		return;
	}
	GetSystemInfo(&sysInfo);

	for (int i = 0; i < 1; ++i) {
		workerThreads.emplace_back([this]() { workerThread(); });
	}

	// Initialize socket
	listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSock == INVALID_SOCKET) {
		ErrorLogger::logAndExit("WSASocket", WSAGetLastError());
		WSACleanup();
		return;
	}

	cout << "GameServer initialized successfully\n";
	return;
}

void IOCPServer::bindAndListen()
{
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(GAME_SERVER_PORT);

	int iResult = bind(listenSock, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		ErrorLogger::logAndExit("bind", WSAGetLastError(), listenSock);
		return;
	}

	if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
		ErrorLogger::logAndExit("listen", WSAGetLastError(), listenSock);
		return;
	}

	cout << "GameServer has been successfully bound and is now listening\n";
	return;
}

void IOCPServer::acceptConnection()
{
	while (1) {
		cout << "\rWaiting for relay server\n";

		relayServSock = accept(listenSock, &relayServAddr, &iRelayServAddrSize);

		if (relayServSock == INVALID_SOCKET) {
			ErrorLogger::logAndExit("accept", WSAGetLastError(), listenSock);
			return;
		}

		printf("Connected with RelayServer\n");

		LPPER_HANDLE_DATA handleInfo = new PER_HANDLE_DATA();
		handleInfo->sock = relayServSock;
		handleInfo->sockAddr = *((struct sockaddr_in*)&relayServAddr);

		CreateIoCompletionPort((HANDLE)relayServSock, hCompPort, (ULONG_PTR)&handleInfo, 0);

		receiveMessage();
	}
}

void IOCPServer::sendMessage(const char* message)
{
	memcpy(sendBuffer, message, DATA_BUF_SIZE);

	sendDataBuf.len = DATA_BUF_SIZE;
	sendDataBuf.buf = sendBuffer;

	PER_IO_DATA* ioInfo = new PER_IO_DATA();
	memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
	ioInfo->wsaBuf.buf = sendBuffer;
	ioInfo->wsaBuf.len = sendDataBuf.len;
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
		ErrorLogger::log("WSASend", WSAGetLastError());
	}

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
		ErrorLogger::log("WSARecv", WSAGetLastError());
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
			if (err == WSA_IO_PENDING) continue;
			else if (err == ERROR_NETNAME_DELETED || err == WSAESHUTDOWN) {
				cout << "RelayServer disconnected" << endl;
				closesocket(relayServSock);
				delete completedIOInfo;
				continue;
			}
			else {
				ErrorLogger::logAndExit("GetQueuedCompletionStatus", WSAGetLastError(), listenSock);
				break;
			}
		}

		if (completedIOInfo->rwMode == READ) {
			PacketBuffer pb(completedIOInfo->buffer);
			PacketManager* packetManager = PacketManager::getInstance();
			packetManager->pushPacket(pb);
			receiveMessage();
		}
		delete completedIOInfo;
	}

	return;
}

void IOCPServer::runIOCPServer()
{
	cout << "************* Running IOCPServer *************\n";

	init();
	bindAndListen();
	acceptConnection();
}
