#pragma once
    
#include <queue>
#include <mutex>
#include "PacketBuffer.h"
#include "ErrorLogger.h"

class PacketQueue {
public:
    PacketQueue();
    ~PacketQueue();

    void push(const PacketBuffer packetInfo);
    PacketBuffer pop();

    bool isEmpty() const;

private:
    queue<PacketBuffer> packetQueue;
    mutex mutex;
};
