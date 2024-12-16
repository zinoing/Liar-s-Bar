#pragma once

#include <iostream>
#include <string>
#include <mutex>

#include "Singleton.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "PacketManager.h"
#include "PacketType.h"
#include "PacketBuffer.h"

using namespace std;

class LoginManager : public Singleton<LoginManager>
{
public:
private:
	bool isAuthenticated;

public:
	LoginManager();

	bool getIsAuthenticated() const;
	void login();
	void registerUser();
	void alertLoginResult(bool result);
	void alertRegisterResult(bool result);
private:
	string askForId();
	string askForPassword();
};

