// Liar's Bar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "LobbyManager.h"

using namespace std;

int main()
{
    NetworkManager* networkManager = NetworkManager::getInstance();
    networkManager->init();
    networkManager->connectWithServer();

    LobbyManager* lobbyManager = LobbyManager::getInstance();
    lobbyManager->showIntro();
    lobbyManager->showLobby();

    while (1) {}
}