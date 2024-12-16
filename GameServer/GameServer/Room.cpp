#include "Room.h"

Room::Room() : name(""), type(0), host(nullptr), numberOfPlayersInRoom(0), turn(0)
{
}

Room::Room(string roomName, int roomType, Player* player) : name(roomName), type(roomType), host(player), numberOfPlayersInRoom(0), turn(0)
{
}

bool Room::enterRoom(Player* player)
{
	lock_guard<mutex> lock(roomMutex);

	if (numberOfPlayersInRoom == MAX_PLAYERS_IN_ROOM) {
		return false;
	}

	playersInRoom.push_back(player);
	++numberOfPlayersInRoom;

	if (numberOfPlayersInRoom == MAX_PLAYERS_IN_ROOM) {
		playGame();
	}
	return true;
}

bool Room::leaveRoom(Player* playerToFind)
{
	lock_guard<mutex> lock(roomMutex);

    auto it = find_if(playersInRoom.begin(), playersInRoom.end(),
        [&](Player* player) { return player == playerToFind; });

    if (it == playersInRoom.end()) {
        return false;
    }

	delete playerToFind;

    playersInRoom.erase(it);
    --numberOfPlayersInRoom;
    return true;
}

string Room::getRoomName()
{
	return name;
}

int Room::getNumberOfPlayersInRoom()
{
	return numberOfPlayersInRoom;
}

void Room::playGame()
{
	cout << "[room] [" << name << "] is now playing the game\n";

	PacketManager* packetManager = PacketManager::getInstance();

	for (auto player : playersInRoom) {
		PacketBuffer pb;
		pb.writeInt((int)playersInRoom.size() - 1);

		for (auto otherPlayer : playersInRoom) {
			if (player == otherPlayer) continue;
			pb.writeString(otherPlayer->getId());
		}
		sendMessageToPlayer(player, ServerPacketType::ALLOW_PLAY_GAME, pb);
	}

	dealer.dealCards(playersInRoom);

	for (auto player : playersInRoom) {
		PacketBuffer pb;
		pb.writeCards(player->getCards());
		sendMessageToPlayer(player, ServerPacketType::DEAL_CARDS, pb);
	}

	PacketBuffer pb;
	sendMessageToPlayer(host, ServerPacketType::PLAYER_TURN, pb); // host starts the first turn
}

void Room::changeTurn(string previousPlayerId, vector<Card> cardsToDiscard)
{
	playersInRoom[turn]->setTurnStatus(false);
	turn = (turn + 1) % MAX_PLAYERS_IN_ROOM;
	playersInRoom[turn]->setTurnStatus(true);

	PacketBuffer pb;

	if (!previousPlayerId.empty()) {
		pb.writeString(previousPlayerId);
		pb.writeCards(cardsToDiscard);
	}

	sendMessageToPlayer(playersInRoom[turn], ServerPacketType::PLAYER_TURN, pb);
}

void Room::sendMessageToAllPlayers(const ServerPacketType packetType, PacketBuffer packetBuffer)
{
	for (auto player : playersInRoom) {
		sendMessageToPlayer(player, packetType, packetBuffer);
	}
}

void Room::sendMessageToPlayer(const Player* player, const ServerPacketType packetType, PacketBuffer packetBuffer)
{
	PacketManager* packetManager = PacketManager::getInstance();
	IOCPServer* server = IOCPServer::getInstance();
	PacketBuffer pb;

	pb.writeString(player->getId());
	if (packetBuffer.getWriteOffset() != 0)
		pb.writeBuffer(packetBuffer);

	server->sendMessage(packetManager->serializePacket(packetType, pb));
}	
