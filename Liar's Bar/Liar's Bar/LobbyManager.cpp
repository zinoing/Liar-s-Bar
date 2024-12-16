#include "LobbyManager.h"

void LobbyManager::showIntro() {
    cout << "Liar's Bar\n"
        << "Liar's Bar imitation game.\n"
        << "Press any key to go next.\n";

    while (!_kbhit()) {
    }

    _getch(); 
}

void LobbyManager::showLoginPage()
{
    LoginManager* loginManager = LoginManager::getInstance();

    system("cls");

    cout << "press 1 to login\n"
        "press 2 to register\n";

    int input;
    cin >> input;

    if (input == LOG_IN) {
        loginManager->login();
    }
    else if (input == REGISTER) {
        loginManager->registerUser();
    }
    else {
        printf("Wrong input");
    }

    return;
}

void LobbyManager::showLobby()
{
    LoginManager* loginManager = LoginManager::getInstance();

    system("cls");

    cout << "press 1 to host\n"
        "press 2 to join\n";

    int input;
    cin >> input;

    if (input == HOST) {
        host();
    }
    else if (input == JOIN) {
        join();
    }
    else {
        printf("Wrong input");
    }

    return;
}

void LobbyManager::host()
{
    system("cls");

    string roomName;
    int roomType;
    cout << "type name of room: ";
    cin >> roomName;

    do {
        system("cls");
        cout << "choose room type (1: public, 2: private): ";
        cin >> roomType;
    } while (!(roomType == 1 || roomType == 2));

    system("cls");

    GameManager* gameManager = GameManager::getInstance();

    PacketBuffer pb;
    pb.writeString(gameManager->getMe()->getId());
    pb.writeString(roomName);
    pb.writeInt(roomType);
    nameOfRoom = roomName;

    PacketManager* packetManager = PacketManager::getInstance();
    const char* serializedPacket = packetManager->serializePacket(ClientPacketType::REQ_HOST_ROOM, pb);

    NetworkManager* networkManager = NetworkManager::getInstance();
    networkManager->sendMessage(serializedPacket);

    cout << "hosting...\n";
    return;
}

void LobbyManager::join()
{
    system("cls");

    string roomName;
    cout << "type name of room: ";
    cin >> roomName;
    nameOfRoom = roomName;

    GameManager* gameManager = GameManager::getInstance();

    PacketBuffer pb;
    pb.writeString(gameManager->getMe()->getId());
    pb.writeString(roomName);

    PacketManager* packetManager = PacketManager::getInstance();
    const char* serializedPacket = packetManager->serializePacket(ClientPacketType::REQ_ENTER_ROOM, pb);

    NetworkManager* networkManager = NetworkManager::getInstance();
    networkManager->sendMessage(serializedPacket);

    cout << "joining...\n";
    return;
}

string LobbyManager::getNameOfRoom()
{
    return nameOfRoom;
}
    