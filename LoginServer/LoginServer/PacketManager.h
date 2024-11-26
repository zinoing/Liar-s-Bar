#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "PacketType.h"
#include "PacketBuffer.h"
#include "IOCPServer.h"
#include "ErrorLogger.h"

using namespace std;

class IOCPServer;

class PacketManager : public Singleton<PacketManager> {
public:
    const char* serializePacket(ServerPacketType type, PacketBuffer packetBuffer);

    void handlePacket(IOCPServer* server, PacketBuffer& packet);

};