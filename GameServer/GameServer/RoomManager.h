#pragma once

#include "Stdfax.h"
#include "Singleton.h"
#include "Room.h"

class Room;

class RoomManager : public Singleton<RoomManager>
{
	vector<Room*> rooms;

public:
	bool hostRoom(string roomName, int roomType, Player* player);
	bool enterRoom(string roomName, Player* player);
	bool leaveRoom(string roomName, Player* player);
	void deleteRoom(Room* room);
	Room* findRoomByName(const string& roomName);
private:
};

