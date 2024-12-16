#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "PacketType.h"
#include "PacketBuffer.h"
#include "LoginManager.h"
#include "GameManager.h"

using namespace std;

class PacketManager : public Singleton<PacketManager>{
public:
    const char* serializePacket(ClientPacketType type, PacketBuffer packetBuffer);

    void handlePacket(PacketBuffer& packet);

};