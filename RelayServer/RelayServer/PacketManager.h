#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "PacketType.h"
#include "PacketBuffer.h"
#include "ClientInfo.h"
#include "IOCPServer.h"
#include "LoginServerConnector.h"
#include "GameServerConnector.h"
#include "PacketQueue.h"

using namespace std;

class PacketManager : public Singleton<PacketManager>{
    PacketQueue packetQueue;
    vector<thread> threads;

private:
    void handlePacket(PacketBuffer& packet, ClientInfo* clientInfo);
    void handlePacketThread();
public:
    void pushPacket(const PacketInfo packetInfo);

    const char* serializePacket(ClientPacketType type, PacketBuffer packetBuffer);
    const char* serializePacket(ServerPacketType type, PacketBuffer packetBuffer);

    void runPacketManager();
};