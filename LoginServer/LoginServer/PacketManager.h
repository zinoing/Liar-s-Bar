#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "PacketType.h"
#include "PacketBuffer.h"
#include "IOCPServer.h"
#include "ErrorLogger.h"
#include "PacketQueue.h"

using namespace std;

class IOCPServer;

class PacketManager : public Singleton<PacketManager> {

    PacketQueue packetQueue;
    vector<thread> threads;

private:
    void handlePacket(PacketBuffer& packet);
    void handlePacketThread();

public:
    void pushPacket(const PacketBuffer& packetBuffer);

    const char* serializePacket(const ServerPacketType& type, PacketBuffer& packetBuffer);

    void runPacketManager();
};