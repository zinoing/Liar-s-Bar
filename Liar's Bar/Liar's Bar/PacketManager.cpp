#include "PacketManager.h"

const char* PacketManager::serializePacket(ClientPacketType type, PacketBuffer buffer)
{
	PacketBuffer rtBuffer;
	rtBuffer.writeType(type);
	rtBuffer.writeBuffer(buffer);

	char* serializedData = new char[DATA_BUF_SIZE];
	memcpy(serializedData, rtBuffer.getData(), DATA_BUF_SIZE);

	return serializedData;
}

void PacketManager::handlePacket(const char* packet)
{
	PacketBuffer packetToHandle(packet);
	ServerPacketType type = packetToHandle.readType();

	switch (type) {
	case ServerPacketType::ACCEPT_LOG_IN:
	{
		string id = packetToHandle.readString();
		string password = packetToHandle.readString();
		break;
	}
	case ServerPacketType::REJECT_LOG_IN:
		break;
	case ServerPacketType::ACCEPT_REGISTER:
		break;
	case ServerPacketType::REJECT_REGISTER:
		break;
	}
}
