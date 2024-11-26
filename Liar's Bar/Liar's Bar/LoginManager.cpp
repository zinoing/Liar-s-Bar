#include "LoginManager.h"

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