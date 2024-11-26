#include "PacketQueue.h"

PacketQueue::PacketQueue()
{
}

PacketQueue::~PacketQueue()
{
}

void PacketQueue::push(const PacketInfo packetInfo)
{
	lock_guard<std::mutex> lock(mutex);
	packetQueue.push(packetInfo);
}

PacketInfo PacketQueue::pop()
{
    std::lock_guard<std::mutex> lock(mutex);

    if(packetQueue.empty())
        ErrorLogger::logAndExit("packetQueue is empty", 9998);

    PacketInfo packetInfo = packetQueue.front();
    packetQueue.pop();
    return packetInfo;
}

bool PacketQueue::isEmpty() const
{
    return packetQueue.empty();
}
