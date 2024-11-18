#include "LobbyManager.h"

void LobbyManager::showIntro() {
    cout << "Liar's Bar\n"
        "Liar's Bar imitation game.\n";
}

void LobbyManager::showLobby()
{
    cout << "press 1 to login\n"
        "press 2 to register\n";

    int input;
    cin >> input;

    if (input == LOG_IN) {
        LoginManager::login();
    }
    else if (input == REGISTER) {
        LoginManager::registerUser();
    }
    else {
        printf("Wrong input");
    }

    return;
}
