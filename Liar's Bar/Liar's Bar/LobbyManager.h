#pragma once

#define LOG_IN 1
#define REGISTER 2

#include <iostream>
#include "Singleton.h"
#include "NetworkManager.h"
#include "LoginManager.h"

using namespace std;

class LobbyManager : public Singleton<LobbyManager>
{
public:
	void showIntro();
	void showLobby();
};

