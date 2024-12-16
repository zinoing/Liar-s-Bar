#include "LoginManager.h"

LoginManager::LoginManager() : isAuthenticated(false)
{
}

bool LoginManager::getIsAuthenticated() const {
    return isAuthenticated;
}

void LoginManager::login()
{
    system("cls");
    string id = askForId();
    string password = askForPassword();

    PacketBuffer packetBuffer;
    packetBuffer.writeString(id);
    packetBuffer.writeString(password);

    NetworkManager* networkManager = NetworkManager::getInstance();
    PacketManager* packetManager = PacketManager::getInstance();
    const char* serializedData = packetManager->serializePacket(ClientPacketType::REQ_LOG_IN, packetBuffer);
    networkManager->sendMessage(serializedData);

    delete[] serializedData;
    return;
}

void LoginManager::registerUser()
{
    system("cls");
    string id = askForId();
    string password = askForPassword();

    PacketBuffer packetBuffer;
    packetBuffer.writeString(id);
    packetBuffer.writeString(password);

    NetworkManager* networkManager = NetworkManager::getInstance();
    PacketManager* packetManager = PacketManager::getInstance();
    const char* serializedData = packetManager->serializePacket(ClientPacketType::REQ_REGISTER, packetBuffer);
    networkManager->sendMessage(serializedData);

    delete[] serializedData;
    return;
}

void LoginManager::alertLoginResult(bool result)
{
    system("cls");

    if (result) {
        cout << "Login successful\n" << endl;
        isAuthenticated = true;
        Sleep(2000);

    }
    else {
        cout << "Id or password do not match. Please try again\n" << endl;

        Sleep(2000);
        LobbyManager* lobbyManger = LobbyManager::getInstance();
        lobbyManger->showLoginPage();
    }

    return;
}

void LoginManager::alertRegisterResult(bool result)
{
    system("cls");

    if (result) {
        cout << "Register successful\n" << endl;
        isAuthenticated = true;
        Sleep(2000);
    }
    else {
        cout << "The given id already exists\n" << endl;

        Sleep(2000);
        LobbyManager* lobbyManger = LobbyManager::getInstance();
        lobbyManger->showLoginPage();
    }

    return;
}

string LoginManager::askForId() 
{
    string username;
    cout << "Please enter id: ";
    cin >> username;
    return username;
}

string LoginManager::askForPassword() 
{
    string password;
    cout << "Please enter password: ";
    cin >> password;
    return password;
}