#include "PacketManager.h"


void PacketManager::handlePacket(PacketBuffer& packet, ClientInfo* clientInfo)
{
	int size = packet.readInt();
	int type = packet.readInt();
	packet.setWriteOffset(size);

	if (type < SERVER_PACKET_OFFSET) {
		ClientPacketType clientType = static_cast<ClientPacketType>(type);

		packet.writeString(clientInfo->getUniqueKey());
		size += (sizeof(int) + clientInfo->getUniqueKey().size());
		packet.setWriteOffset(size);

		packet = packet.getData();

		switch (clientType) {
		case ClientPacketType::REQ_LOG_IN:
		{
			LoginServerConnector* loginServerConnector = LoginServerConnector::getInstance();
			loginServerConnector->sendMessage(packet.getData());
			break;
		}
		case ClientPacketType::REQ_REGISTER:
		{
			LoginServerConnector* loginServerConnector = LoginServerConnector::getInstance();
			loginServerConnector->sendMessage(packet.getData());
			break;
		}
		case ClientPacketType::REQ_HOST_ROOM:
		{
			GameServerConnector* gameServerConnector = GameServerConnector::getInstance();
			gameServerConnector->sendMessage(packet.getData());
			break;
		}
		case ClientPacketType::REQ_ENTER_ROOM:
		{
			GameServerConnector* gameServerConnector = GameServerConnector::getInstance();
			gameServerConnector->sendMessage(packet.getData());
			break;
		}
		}
	}
	else {
		// Handle Server Packet
		ServerPacketType serverType = static_cast<ServerPacketType>(type);
		PacketManager* packetManager = PacketManager::getInstance();

		// clientInfo should be nullptr right now
		string uniqueKey = packet.readString();

		IOCPServer* server = IOCPServer::getInstance();
		clientInfo = server->getClientInfo(uniqueKey);

		switch (serverType) {
		case ServerPacketType::ACCEPT_LOG_IN:
		{
			string id = packet.readString();
			clientInfo->setId(id);
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::ACCEPT_LOG_IN, packet));
			break;
		}
		case ServerPacketType::REJECT_LOG_IN:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::REJECT_LOG_IN, packet));
			break;
		case ServerPacketType::ACCEPT_REGISTER:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::ACCEPT_REGISTER, packet));
			break;
		case ServerPacketType::REJECT_REGISTER:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::REJECT_REGISTER, packet));
			break;
		case ServerPacketType::ALLOW_HOST_ROOM:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::ALLOW_HOST_ROOM, packet));
			break;
		case ServerPacketType::REJECT_HOST_ROOM:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::REJECT_HOST_ROOM, packet));
			break;
		case ServerPacketType::ALLOW_ENTER_ROOM:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::ALLOW_ENTER_ROOM, packet));
			break;
		case ServerPacketType::REJECT_ENTER_ROOM:
			clientInfo->sendMessage(packetManager->serializePacket(ServerPacketType::REJECT_ENTER_ROOM, packet));
			break;
		}
	}
}

void PacketManager::handlePacketThread()
{
	while (1) {
		if (!packetQueue.isEmpty()) {
			PacketInfo packetInfo = packetQueue.pop();
			handlePacket(packetInfo.pb, packetInfo.pClientInfo);
		}
	}
}

void PacketManager::pushPacket(const PacketInfo packetInfo)
{
	packetQueue.push(packetInfo);
}

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

const char* PacketManager::serializePacket(ServerPacketType type, PacketBuffer buffer)
{
	int size = buffer.getWriteOffset();

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