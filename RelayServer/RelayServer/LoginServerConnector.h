#pragma once

#include "ServerConnector.h"
#include "Singleton.h"

class LoginServerConnector : public ServerConnector, public Singleton<LoginServerConnector>
{
public:
	void runLoginServerConnector();
};

