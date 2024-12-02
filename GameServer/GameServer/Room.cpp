#include "Room.h"

Room::Room(string roomName, int roomType, string hostId) : name(roomName), type(roomType), hostId(hostId), numberOfPlayersInRoom(1)
{
}

void Room::enterRoom(string playerId)
{
}

void Room::leaveRoom(string playerId)
{
}

int Room::getNumberOfPlayersInRoom()
{
	return 0;
}
