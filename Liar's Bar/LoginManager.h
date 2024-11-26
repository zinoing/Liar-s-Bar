#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include "Singleton.h"

using namespace std;

class LoginManager
{
public:
	LoginManager() = default;  // 기본 생성자 필요

	void login() const;
	void registerUser() const;

private:
	string askForId() const;
    string askForPassword() const;
	bool checkIdExists(const string& id) const;
	bool askForUserConfirmation(const string& id, const string& password) const;
};

