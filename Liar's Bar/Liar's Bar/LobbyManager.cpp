#include "LobbyManager.h"

void LobbyManager::showIntro() {
    cout << "Liar's Bar\n"
        "Liar's Bar imitation game.\n";
}

void LobbyManager::showLobby()
{
    LoginManager& loginManager = Singleton<LoginManager>::getInstance();
    NetworkManager& networkManager = Singleton<NetworkManager>::getInstance();

    networkManager.init();
    networkManager.connectWithServer();

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
