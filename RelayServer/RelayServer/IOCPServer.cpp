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
	GetSystemInfo(&sysInfo);

	for (int i = 0; i < 16; ++i) {
		thread workerThread([this]() {
			this->handleIOCompletionThread(this);
			});
		workerThread.detach();
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
	service.sin_port = htons(27015);

	int iResult = bind(listenSock, (SOCKADDR*)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
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
		cout << "\rWaiting for another client";

		acceptSock = accept(listenSock, &acceptAddr, &iAcceptSize);

		if (acceptSock == INVALID_SOCKET) {
			cout << "accept failed with error: " << WSAGetLastError() << "\n";
			closesocket(listenSock);
			WSACleanup();
			return;
		}

		printf("Client Accepted...\n");

		PER_HANDLE_DATA* handleInfo = new PER_HANDLE_DATA();
		handleInfo->acceptSock = acceptSock;
		handleInfo->acceptAddr = *((struct sockaddr_in*)&acceptAddr);

		CreateIoCompletionPort((HANDLE)acceptSock, hCompPort, (DWORD)handleInfo, 0);

		PER_IO_DATA* ioInfo = new PER_IO_DATA();
		memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->wsaBuf.len = DATA_BUF_SIZE;
		ioInfo->rwMode = READ;

		WSARecv(acceptSock, &(ioInfo->wsaBuf), 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
	}
}

void IOCPServer::sendMessage(const char message[DATA_BUF_SIZE])
{
	memset(buffer, 0, sizeof(DATA_BUF_SIZE));
	strcpy_s(buffer, message);
	transferredBytes = 0;

	dataBuf.len = DATA_BUF_SIZE;
	dataBuf.buf = buffer;

	PER_IO_DATA* ioInfo = new PER_IO_DATA();
	memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
	ioInfo->wsaBuf.buf = ioInfo->buffer;
	ioInfo->wsaBuf.len = DATA_BUF_SIZE;
	ioInfo->rwMode = WRITE;

	int rc = WSASend(acceptSock, &dataBuf, 1, &transferredBytes, 0, &(ioInfo->overlapped), NULL);
	if ((rc == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING)) {
		cout << "WSASend failed with error: " << WSAGetLastError() << "\n";
	}
	
	return;
}

void IOCPServer::receiveMessage()
{

}

DWORD WINAPI IOCPServer::handleIOCompletionThread(LPVOID pCompPort)
{
	SOCKET completedSock;
	sockaddr_in completedAddr;

	LPPER_HANDLE_DATA completedHandleInfo = nullptr;
	LPPER_IO_DATA completedIOInfo = nullptr;

	while (1) {
		GetQueuedCompletionStatus(hCompPort, &transferredBytes, (PULONG_PTR)&completedHandleInfo, (LPOVERLAPPED*)&completedIOInfo, INFINITE);

		completedSock = completedHandleInfo->acceptSock;
		completedAddr = completedHandleInfo->acceptAddr;

		if (completedIOInfo->rwMode == READ) {
			// 읽기 작업 완료 처리
			cout << "Read data: " << completedIOInfo->buffer << endl;
		}
		else if (completedIOInfo->rwMode == WRITE) {
			// 쓰기 작업 완료 처리
			cout << "Sent message successfully" << endl;
		}
	}

	return 0;
}

void IOCPServer::runIOCPServer()
{
}
