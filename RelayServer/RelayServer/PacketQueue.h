#pragma once

#include <queue>
#include <mutex>
#include "ClientInfo.h"
#include "PacketBuffer.h"
#include "ErrorLogger.h"

struct PacketInfo {
    PacketBuffer pb;
    ClientInfo* pClientInfo;

    PacketInfo(PacketBuffer pb, ClientInfo* pClientInfo) {
        this->pb = pb;
        this->pClientInfo = pClientInfo;
    }
};

class PacketQueue {
public:
    PacketQueue();
    ~PacketQueue();

    void push(const PacketInfo packetInfo);
    PacketInfo pop();

    bool isEmpty() const;

private:
    queue<PacketInfo> packetQueue;
    mutex mutex;
};
