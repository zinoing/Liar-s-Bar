#pragma once

#include "Stdfax.h"
#include "Singleton.h"
#include "Room.h"

class RoomManager : public Singleton<RoomManager>
{
	vector<Room> rooms;

public:
	bool hostRoom(string roomName, int roomType, string hostId);
	bool enterRoom(string roomName, string playerId);
};

