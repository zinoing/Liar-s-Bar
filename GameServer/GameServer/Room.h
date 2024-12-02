#pragma once

#define PUBLIC 1
#define PRIVATE 2

#include "Stdfax.h"
#include "Player.h"

class Room
{
	string name;
	int type;
	string enterCode;

	string hostId;
	vector<Player> playersInRoom;
	int numberOfPlayersInRoom;

public:
	Room(string roomName, int roomType, string hostId);
	void enterRoom(string playerId);
	void leaveRoom(string playerId);
	int getNumberOfPlayersInRoom();
};

