#include "RoomManager.h"

bool RoomManager::hostRoom(string roomName, int roomType, Player* player)
{
	if (findRoomByName(roomName) == nullptr) {
		Room* room = new Room(roomName, roomType, player);
		rooms.push_back(room);

		if (room->enterRoom(player)) {
			cout << "[" << player->getId() << "] hosted room\n";
			return true;
		}
	}

	return false;
}

bool RoomManager::enterRoom(string roomName, Player* player)
{
	Room* room = findRoomByName(roomName);
	if (room == nullptr) {
		return false;
	}
	else {
		if (room->enterRoom(player)) {
			cout << "[" << player->getId() << "] entered room\n";
			return true;
		}
		return false;
	}
}

bool RoomManager::leaveRoom(string roomName, Player* player)
{
	Room* room = findRoomByName(roomName);
	if (room == nullptr) {
		return false;
	}
	else {
		room->leaveRoom(player);
		cout << "[" << player->getId() << "] left room\n";

		if (room->getNumberOfPlayersInRoom() == 0) {
			deleteRoom(room);
		}

		return true;
	}
}

void RoomManager::deleteRoom(Room* room)
{
	rooms.erase(remove(rooms.begin(), rooms.end(), room), rooms.end());
	delete room;
}

Room* RoomManager::findRoomByName(const string& roomName)
{
	for (auto room : rooms) {
		if (room->getRoomName() == roomName) {
			return room;
		}
	}
	return nullptr;
}