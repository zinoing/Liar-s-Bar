#pragma once

#define PUBLIC 1
#define PRIVATE 2

#define MAX_PLAYERS_IN_ROOM 2

#include "Stdfax.h"
#include "Player.h"
#include "PacketManager.h"
#include "Dealer.h"
#include <mutex>

class Room
{
	// Room Info
	string name;
	int type;
	string enterCode;

	// players info
	Player* host;
	vector<Player*> playersInRoom;
	int numberOfPlayersInRoom;

	// dealer
	Dealer dealer;

	// turn
	int turn;

	mutex roomMutex;

public:
	Room();
	Room(string roomName, int roomType, Player* host);
	bool enterRoom(Player* player);
	bool leaveRoom(Player* player);
	string getRoomName();
	int getNumberOfPlayersInRoom();
	void playGame();
	void changeTurn(string previousPlayerId, vector<Card> cardsToDiscard);

private:
	void sendMessageToAllPlayers(const ServerPacketType packetType, PacketBuffer packetBuffer);
	void sendMessageToPlayer(const Player* player, const ServerPacketType packetType, PacketBuffer packetBuffer);
};

