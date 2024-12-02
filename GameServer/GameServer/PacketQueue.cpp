#include "PacketQueue.h"

PacketQueue::PacketQueue()
{
}

PacketQueue::~PacketQueue()
{
}

void PacketQueue::push(const PacketBuffer packetBuffer)
{
    lock_guard<std::mutex> lock(mutex);
    packetQueue.push(packetBuffer);
}

PacketBuffer PacketQueue::pop()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (packetQueue.empty())
        ErrorLogger::logAndExit("packetQueue is empty", 9998);

    PacketBuffer pb = packetQueue.front();
    packetQueue.pop();
    return pb;
}

bool PacketQueue::isEmpty() const
{
    return packetQueue.empty();
}
