#include "PacketManager.h"
#include "DatabaseConnector.h"

void PacketManager::handlePacket(PacketBuffer& packet)
{
	int size = packet.readInt();
	packet.setWriteOffset(size);

	int type = packet.readInt();

	if (type < SERVER_PACKET_OFFSET) {
		ClientPacketType clientType = static_cast<ClientPacketType>(type);

		switch (clientType) {
		case ClientPacketType::REQ_LOG_IN:
		{
			string id = packet.readString();
			string passsword = packet.readString();
			string uniqueKey = packet.readString();

			DatabaseConnector* dbConnector = DatabaseConnector::getInstance();
			IOCPServer* server = IOCPServer::getInstance();

			if (dbConnector->logIn(id, passsword)) {
				PacketBuffer packetToSend;
				packetToSend.writeString(uniqueKey);
				packetToSend.writeString(id);
				server->sendMessage(serializePacket(ServerPacketType::ACCEPT_LOG_IN, packetToSend));
			}
			else {
				PacketBuffer packetToSend;
				packetToSend.writeString(uniqueKey);
				packetToSend.writeString(id);
				server->sendMessage(serializePacket(ServerPacketType::REJECT_LOG_IN, packetToSend));
			}
			break;
		}
		case ClientPacketType::REQ_REGISTER:
		{
			string id = packet.readString();
			string passsword = packet.readString();
			string uniqueKey = packet.readString();

			DatabaseConnector* dbConnector = DatabaseConnector::getInstance();
			IOCPServer* server = IOCPServer::getInstance();

			if (dbConnector->registerUser(id, passsword)) {
				PacketBuffer packetToSend;
				packetToSend.writeString(uniqueKey);
				packetToSend.writeString(id);
				server->sendMessage(serializePacket(ServerPacketType::ACCEPT_REGISTER, packetToSend));
			}
			else {
				PacketBuffer packetToSend;
				packetToSend.writeString(uniqueKey);
				packetToSend.writeString(id);
				server->sendMessage(serializePacket(ServerPacketType::REJECT_REGISTER, packetToSend));
			}
			break;
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