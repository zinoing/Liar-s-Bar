#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "PacketType.h"
#include "PacketBuffer.h"

using namespace std;

class PacketManager : public Singleton<PacketManager>{
public:
    // ��Ŷ�� ����ȭ�ϴ� �Լ�
    static const char* serializePacket(ClientPacketType type, PacketBuffer packetBuffer);

    static void handlePacket(const char* packet);

};