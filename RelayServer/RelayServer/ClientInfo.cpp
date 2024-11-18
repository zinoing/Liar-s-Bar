#include "ClientInfo.h"

ClientInfo::ClientInfo(LPPER_HANDLE_DATA handleData)
{
	this->handleData = handleData;

	memset(sendBuffer, 0, DATA_BUF_SIZE);
	memset(recvBuffer, 0, DATA_BUF_SIZE);
}

ClientInfo::~ClientInfo()
{
	delete handleData;
}

void ClientInfo::sendMessage(const char* message)
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

	int rc = WSASend(handleData->sock,
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
}

void ClientInfo::receiveMessage()
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

	int rc = WSARecv(handleData->sock,
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
}

void ClientInfo::closeSocket()
{
	closesocket(handleData->sock);
}