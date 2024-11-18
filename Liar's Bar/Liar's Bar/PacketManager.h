#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "PacketType.h"
#include "PacketBuffer.h"

using namespace std;

class PacketManager : public Singleton<PacketManager>{
public:
    // 패킷을 직렬화하는 함수
    static const char* serializePacket(ClientPacketType type, PacketBuffer packetBuffer);

    static void handlePacket(const char* packet);

};