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
        std::cout << "WSAStartup failed with error: " << WSAGetLastError() << "\n";
        return;
    }

    clntSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (clntSock == INVALID_SOCKET)
    {
        std::cout << "WSASocket failed with error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    int reuseAddr = 1;
    setsockopt(clntSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseAddr, sizeof(reuseAddr));

    // 클라이언트 소켓에 비동기식 작업을 위한 설정을 해줍니다.
    u_long nonBlocking = 1;
    ioctlsocket(clntSock, FIONBIO, &nonBlocking);

    std::cout << "NetworkManager initialized successfully\n";
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
            cout << "connect failed with error: " << errCode << "\n";
            WSACleanup();
            return;
        }
    }

    cout << "Connect to the server\n";

    thread workerThread([this]() {
        this->handleIO();
        });
    workerThread.detach();

    receiveMessage();
}

void NetworkManager::sendMessage(const char* message)
{
    strcpy_s(sendBuffer, message);
    transferredBytes = 0;

    sendDataBuf.len = (ULONG)strlen(sendBuffer);
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
        std::cout << "WSASend failed with error: " << WSAGetLastError() << "\n";
    }
    else
    {
        std::cout << "Sent message: " << message << std::endl;
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
    flags = 0;

    // 비동기적으로 데이터 수신
    int rc = WSARecv(clntSock, 
        &(ioInfo->wsaBuf),
        1, &transferredBytes, 
        (LPDWORD)&flags, 
        &(ioInfo->overlapped), 
        NULL);

    if ((rc == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
    {
        std::cout << "WSARecv failed with error: " << WSAGetLastError() << "\n";
    }
}

void NetworkManager::handleIO()
{
    while (true)
    {
        DWORD dwFlags = 0;
        DWORD bytesTransferred = 0;
        PER_IO_DATA* ioInfo = new PER_IO_DATA();

        // I/O 작업이 완료되면, 완료된 I/O 작업을 처리합니다.
        int rc = WSAGetOverlappedResult(clntSock, &(ioInfo->overlapped), &bytesTransferred, TRUE, &dwFlags);
        if (rc == 0)
        {
            cout << "WSAGetOverlappedResult failed with error: " << WSAGetLastError() << "\n";
            delete ioInfo;
            return;
        }

        // 읽기 작업이 완료되었을 경우
        if (ioInfo->rwMode == READ)
        {
            cout << "Received data: " << ioInfo->buffer << std::endl;
        }
        // 쓰기 작업이 완료되었을 경우
        else if (ioInfo->rwMode == WRITE)
        {
            cout << "Sent message successfully\n";
        }

        delete ioInfo;
    }
}
