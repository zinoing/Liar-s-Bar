#include "PacketManager.h"

void PacketManager::handlePacket(PacketBuffer& packet)
{
	int size = packet.readInt();
	packet.setWriteOffset(size);

	int type = packet.readInt();

	if (type < SERVER_PACKET_OFFSET) {
		ClientPacketType clientType = static_cast<ClientPacketType>(type);

		switch (clientType) {
		case ClientPacketType::REQ_HOST_ROOM:
		{
			string hostId = packet.readString();
			string roomName = packet.readString();
			int roomType = packet.readInt();
			Player* player = new Player(hostId);

			RoomManager* roomManager = RoomManager::getInstance();
			IOCPServer* server = IOCPServer::getInstance();

			PacketBuffer packetToSend;
			packetToSend.writeString(hostId);

			if (roomManager->hostRoom(roomName, roomType, player)) {
				server->sendMessage(serializePacket(ServerPacketType::ALLOW_HOST_ROOM, packetToSend));
			}
			else {
				server->sendMessage(serializePacket(ServerPacketType::REJECT_HOST_ROOM, packetToSend));
			}
			break;
		}
		case ClientPacketType::REQ_ENTER_ROOM:
		{
			string playerId = packet.readString();
			string roomName = packet.readString();
			Player* player = new Player(playerId);

			RoomManager* roomManager = RoomManager::getInstance();
			IOCPServer* server = IOCPServer::getInstance();

			PacketBuffer packetToSend;
			packetToSend.writeString(playerId);

			if (roomManager->enterRoom(roomName, player)) {
				server->sendMessage(serializePacket(ServerPacketType::ALLOW_ENTER_ROOM, packetToSend));
			}
			else {
				server->sendMessage(serializePacket(ServerPacketType::REJECT_ENTER_ROOM, packetToSend));
			}
			break;
		}
		case ClientPacketType::PlAYER_TURN_OVER:
		{
			string playerId = packet.readString();
			string roomName = packet.readString();

			vector<Card> cardsDiscarded = packet.readCards();

			RoomManager* roomManager = RoomManager::getInstance();
			Room* roomPlayerIn = roomManager->findRoomByName(roomName);

			roomPlayerIn->changeTurn(playerId, cardsDiscarded);
		}

		default:
			int errorCode = 9999;
			ErrorLogger::log("handlePacket", errorCode);
			break;
		}
	}
	else {
		int errorCode = 9999;
		ErrorLogger::log("handlePacket", errorCode);
	}
}

void PacketManager::handlePacketThread()
{
	while (1) {
		if (!packetQueue.isEmpty()) {
			PacketBuffer pb = packetQueue.pop();
			handlePacket(pb);
		}
	}
}

void PacketManager::pushPacket(const PacketBuffer& packetBuffer)
{
	packetQueue.push(packetBuffer);
}

const char* PacketManager::serializePacket(const ServerPacketType& type, PacketBuffer& buffer)
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

void PacketManager::runPacketManager()
{
	cout << "************* Running packet manager *************\n";

	for (int i = 0; i < 1; ++i) {
		threads.emplace_back([this]() { handlePacketThread(); });
	}
}