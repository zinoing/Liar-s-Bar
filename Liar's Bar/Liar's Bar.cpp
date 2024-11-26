// Liar's Bar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Singleton.h"
#include "LoginManager.h"

#define LOG_IN 1
#define REGISTER 2

using namespace std;

void showIntro() {
    cout << "Liar's Bar\n"
        "Liar's Bar imitation game.\n";
}

void showLobby() {

    const LoginManager& loginManager = Singleton<LoginManager>::getInstance();

    cout << "press 1 to login\n"
        "press 2 to register\n";

    int input;
    cin >> input;
    
    if (input == LOG_IN) {
        loginManager.login();
    }
    else if (input == REGISTER) {
        loginManager.registerUser();
    }
    else {
        printf("Wrong input");
    }
    return;
}

int main()
{
    showIntro();
    showLobby();

}