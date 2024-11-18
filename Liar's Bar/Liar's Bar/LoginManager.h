#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include "Singleton.h"

using namespace std;

class LoginManager
{
public:
	LoginManager() = default;

	void login();
	void registerUser();

private:
	string askForId();
    string askForPassword();
	bool checkIdExists(const string& id);
	bool askForUserConfirmation(const string& id, const string& password);
};

