#include "GameServerConnector.h"

void GameServerConnector::runGameServerConnector()
{
	runConnector("127.0.0.1", GAME_SERVER_PORT);
}
