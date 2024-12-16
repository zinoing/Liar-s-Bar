#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define LOG_IN 1
#define REGISTER 2

#define HOST 1
#define JOIN 2

#define PUBLIC 1
#define PRIVATE 2

#include <iostream>
#include <conio.h>

#include "Singleton.h"
#include "NetworkManager.h"
#include "LoginManager.h"

using namespace std;

class LobbyManager : public Singleton<LobbyManager>
{
	string nameOfRoom;
public:
	void showIntro();
	void showLoginPage();
	void showLobby();
	void host();
	void join();
	string getNameOfRoom();
};

