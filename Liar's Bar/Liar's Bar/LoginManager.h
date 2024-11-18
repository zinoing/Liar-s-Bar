#pragma once

#include <iostream>
#include <string>

#include "Singleton.h"
#include "NetworkManager.h"
#include "PacketManager.h"
#include "PacketType.h"
#include "PacketBuffer.h"

using namespace std;

class LoginManager
{
public:
	LoginManager() = default;

	static void login();
	static void registerUser();
private:
	static string askForId();
	static string askForPassword();
	static bool checkIdExists(const string& id);
	static bool askForUserConfirmation(const string& id, const string& password);
};

