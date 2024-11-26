#include "PacketManager.h"

const char* PacketManager::serializePacket(ClientPacketType type, PacketBuffer buffer)
{
	int size = buffer.getWriteOffset() + sizeof(int) * 2;

	PacketBuffer rtBuffer;
	rtBuffer.writeInt(size);
	rtBuffer.writeType(type);
	rtBuffer.writeBuffer(buffer);

	char* serializedData = new char[DATA_BUF_SIZE];
	memcpy(serializedData, rtBuffer.getData(), DATA_BUF_SIZE);

	return serializedData;
}

void PacketManager::handlePacket(PacketBuffer& packet)
{
	PacketBuffer packetToHandle(packet);
	int size = packetToHandle.readInt();
	packetToHandle.setWriteOffset(size);

	int type = packetToHandle.readInt();
	ServerPacketType packetType = static_cast<ServerPacketType>(type);

	switch (packetType) {
	case ServerPacketType::ACCEPT_LOG_IN:
		system("cls");
		cout << "Login successful\n" << endl;
		break;
	case ServerPacketType::REJECT_LOG_IN:
		system("cls");
		cout << "Id or password do not match. Please try again\n" << endl;
		break;
	case ServerPacketType::ACCEPT_REGISTER:
		system("cls");
		cout << "Register successful\n" << endl;
		break;
	case ServerPacketType::REJECT_REGISTER:
		system("cls");
		cout << "The given id already exists\n" << endl;
		break;
	}
}
