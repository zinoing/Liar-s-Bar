#include "ServerConnector.h"
#include "PacketManager.h"

void ServerConnector::init()
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

    servConnectSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (servConnectSock == INVALID_SOCKET)
    {
        ErrorLogger::logAndExit("WSASocket", WSAGetLastError());
        return;
    }

    int reuseAddr = 1;
    setsockopt(servConnectSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseAddr, sizeof(reuseAddr));

    u_long nonBlocking = 1;
    ioctlsocket(servConnectSock, FIONBIO, &nonBlocking);

    cout << "Connector initialized successfully\n";
}

void ServerConnector::connectWithServer(const char* ipAddress, const u_short port)
{
    memset(&servConnectAddr, 0, sizeof(servConnectAddr));
    servConnectAddr.sin_family = AF_INET;
    servConnectAddr.sin_addr.S_un.S_addr = inet_addr(ipAddress);
    servConnectAddr.sin_port = htons(port);

    if (connect(servConnectSock, (sockaddr*)&servConnectAddr, sizeof(servConnectAddr)) == SOCKET_ERROR)
    {
        int errCode = WSAGetLastError();
        if (errCode != WSAEWOULDBLOCK)
        {
            ErrorLogger::logAndExit("connect", WSAGetLastError(), servConnectSock);
            return;
        }
    }

    CreateIoCompletionPort((HANDLE)servConnectSock, hCompPort, (ULONG_PTR)servConnectSock, 0);

    cout << "Connect to the server\n";

    thread workerThread([this]() {
        this->handleIO();
        });
    workerThread.detach();

    receiveMessage();
}

void ServerConnector::sendMessage(const char* message)
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

    int rc = WSASend(servConnectSock,
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

void ServerConnector::receiveMessage()
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

    int rc = WSARecv(servConnectSock,
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

void ServerConnector::handleIO()
{
    DWORD transferredBytes = 0;

    LPPER_HANDLE_DATA pHandleInfo = nullptr;
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
                cout << "Client disconnected" << endl;
                if(pHandleInfo->sock != NULL)
                    closesocket(pHandleInfo->sock);
                delete completedIOInfo;
                continue;
            }
            else {
                ErrorLogger::logAndExit("GetQueuedCompletionStatus", WSAGetLastError(), pHandleInfo->sock);
                break;
            }
        }

        if (completedIOInfo->rwMode == READ) {

            PacketBuffer pb(completedIOInfo->buffer);

            PacketManager* packetManager = PacketManager::getInstance();
            packetManager->pushPacket(PacketInfo(pb, nullptr));

            receiveMessage();
        }

        delete completedIOInfo;
    }

    return;
}

void ServerConnector::runConnector(const char* ipAddress, const u_short port)
{
    init();
    connectWithServer(ipAddress, port);
}
