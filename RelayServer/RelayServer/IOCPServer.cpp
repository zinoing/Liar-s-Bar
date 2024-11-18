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
	service.sin_port = htons(RELAY_SERVER_PORT);

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

		acceptSock = accept(listenSock, &acceptAddr, &iAcceptSize);

		if (acceptSock == INVALID_SOCKET) {
			cout << "accept failed with error: " << WSAGetLastError() << "\n";
			closesocket(listenSock);
			WSACleanup();
			return;
		}


		LPPER_HANDLE_DATA handleInfo = new PER_HANDLE_DATA();
		handleInfo->sock = acceptSock;
		handleInfo->sockAddr = *((struct sockaddr_in*)&acceptAddr);

		// check if this connection is from login server
		if (handleInfo->sockAddr.sin_port == LOGIN_SERVER_PORT) {
			cout << "Connected to login server" << endl;
		}

		ClientInfo* client = new ClientInfo(handleInfo);
		clients.push_back(client);
		printf("Client Accepted\n");

		CreateIoCompletionPort((HANDLE)acceptSock, hCompPort, (ULONG_PTR)client, 0);

		client->receiveMessage();
	}
}

void IOCPServer::sendMessage(ClientInfo clientInfo, const char* message)
{
	clientInfo.sendMessage(message);
	return;
}

void IOCPServer::receiveMessage(ClientInfo clientInfo)
{
	clientInfo.receiveMessage();
	return;
}

void IOCPServer::workerThread()
{
	DWORD transferredBytes = 0;

	ClientInfo* pClientInfo = nullptr;
	LPPER_IO_DATA completedIOInfo = nullptr;

	while (1) {
		BOOL success = GetQueuedCompletionStatus(hCompPort, 
			&transferredBytes, 
			(PULONG_PTR)&pClientInfo,
			(LPOVERLAPPED*)&completedIOInfo,
			INFINITE);
		
		if (!success || transferredBytes == 0) {
			int err = WSAGetLastError();
			if (err == ERROR_NETNAME_DELETED || err == WSAESHUTDOWN) {
				cout << "Client disconnected." << endl;
				pClientInfo->closeSocket();
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

			PacketBuffer pb(completedIOInfo->buffer);
			pb.readType();
			cout << "Id: " << pb.readString() << " Password: " << pb.readString() << endl;

			pClientInfo->receiveMessage();
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
