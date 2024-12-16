// Liar's Bar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Stdfax.h"
#include "LobbyManager.h"

using namespace std;

int main()
{
    NetworkManager* networkManager = NetworkManager::getInstance();
    networkManager->init();
    networkManager->connectWithServer();

    LobbyManager* lobbyManager = LobbyManager::getInstance();
    lobbyManager->showIntro();
    lobbyManager->showLoginPage();

    while (1) {}
}