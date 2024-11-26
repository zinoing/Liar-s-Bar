#include "NetworkManager.h"
#include <thread>
#include <winsock2.h>
#include <iostream>
#include <cstring>

void NetworkManager::init()
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

    clntSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (clntSock == INVALID_SOCKET)
    {
        ErrorLogger::logAndExit("WSASocket", WSAGetLastError());
        WSACleanup();
        return;
    }

    int reuseAddr = 1;
    setsockopt(clntSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseAddr, sizeof(reuseAddr));

    // 클라이언트 소켓에 비동기식 작업을 위한 설정을 해줍니다.
    u_long nonBlocking = 1;
    ioctlsocket(clntSock, FIONBIO, &nonBlocking);

    cout << "NetworkManager initialized successfully\n";
}

void NetworkManager::connectWithServer()
{
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(8000);

    if (connect(clntSock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        int errCode = WSAGetLastError();
        if (errCode != WSAEWOULDBLOCK)
        {
            ErrorLogger::logAndExit("connect", WSAGetLastError(), clntSock);
            WSACleanup();
            return;
        }
    }

    CreateIoCompletionPort((HANDLE)clntSock, hCompPort, (ULONG_PTR)clntSock, 0);

    cout << "Connect to the server\n";

    thread workerThread([this]() {
        this->handleIO();
        });
    workerThread.detach();

    receiveMessage();
}

void NetworkManager::sendMessage(const char* message)
{
    memcpy(sendBuffer, message, DATA_BUF_SIZE);
    transferredBytes = 0;

    sendDataBuf.len = DATA_BUF_SIZE;
    sendDataBuf.buf = sendBuffer;

    PER_IO_DATA* ioInfo = new PER_IO_DATA();
    memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
    ioInfo->wsaBuf.buf = sendBuffer;
    ioInfo->wsaBuf.len = sendDataBuf.len;
    ioInfo->rwMode = WRITE;

    int rc = WSASend(clntSock, 
        &sendDataBuf, 
        1, 
        &transferredBytes, 
        0, 
        &(ioInfo->overlapped), 
        NULL);
    
    if ((rc == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
    {
        ErrorLogger::log("WSASend", WSAGetLastError());
    }
}

void NetworkManager::receiveMessage()
{
    memset(recvBuffer, 0, DATA_BUF_SIZE);
    recvDataBuf.len = DATA_BUF_SIZE;
    recvDataBuf.buf = recvBuffer;

    PER_IO_DATA* ioInfo = new PER_IO_DATA();
    memset(&(ioInfo->overlapped), 0, sizeof(WSAOVERLAPPED));
    ioInfo->wsaBuf.buf = ioInfo->buffer;
    ioInfo->wsaBuf.len = DATA_BUF_SIZE;
    ioInfo->rwMode = READ;
    DWORD flags = 0;

    int rc = WSARecv(clntSock, 
        &(ioInfo->wsaBuf),
        1, &transferredBytes, 
        (LPDWORD)&flags, 
        &(ioInfo->overlapped), 
        NULL);

    if ((rc == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
    {
        ErrorLogger::log("WSARecv", WSAGetLastError());
    }
}

void NetworkManager::handleIO()
{
    LPPER_HANDLE_DATA* pHandleInfo = nullptr;
    LPPER_IO_DATA completedIOInfo = nullptr;

    while (1) {
        BOOL success = GetQueuedCompletionStatus(hCompPort,
            &transferredBytes,
            (PULONG_PTR)&pHandleInfo,
            (LPOVERLAPPED*)&completedIOInfo,
            INFINITE);

        if (!success || transferredBytes == 0) {
            int err = WSAGetLastError();
            if (err == ERROR_NETNAME_DELETED || err == WSAESHUTDOWN) {
                cout << "Server disconnected" << endl;
                closesocket(clntSock);
                delete completedIOInfo;
                continue;
            }
            else {
                ErrorLogger::logAndExit("GetQueuedCompletionStatus", WSAGetLastError(), clntSock);
                break;
            }
        }

        if (completedIOInfo->rwMode == READ) {
            cout << "Read data: " << completedIOInfo->buffer << endl;

            PacketBuffer pb(completedIOInfo->buffer);

            PacketManager* packetManager = PacketManager::getInstance();
            packetManager->handlePacket(pb);

            receiveMessage();
        }
        else if (completedIOInfo->rwMode == WRITE) {
            cout << "Sent message successfully" << endl;
        }

        delete completedIOInfo;
    }

    return;
}