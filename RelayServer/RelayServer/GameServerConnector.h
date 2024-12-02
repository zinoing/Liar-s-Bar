#pragma once

#include "ServerConnector.h"
#include "Singleton.h"

class GameServerConnector : public ServerConnector, public Singleton<GameServerConnector>
{
public:
	void runGameServerConnector();
};
