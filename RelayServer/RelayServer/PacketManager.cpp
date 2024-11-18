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
	ClientPacketType type = packetToHandle.readType();

	switch (type) {
	case ClientPacketType::REQ_LOG_IN:
		break;
	case ClientPacketType::REQ_REGISTER:
		break;
	}
}
