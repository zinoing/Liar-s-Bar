#include "LoginManager.h"

void LoginManager::login()
{
    system("cls");
    string id = askForId();
    string password = askForPassword();
    //if (!askForUserConfirmation(id, password)) login();

    PacketBuffer packetBuffer;
    packetBuffer.writeString(id);
    packetBuffer.writeString(password);

    NetworkManager* networkManager = NetworkManager::getInstance();
    const char* serializedData = PacketManager::serializePacket(ClientPacketType::REQ_LOG_IN, packetBuffer);
    networkManager->sendMessage(serializedData);

    delete[] serializedData;
    return;
}

void LoginManager::registerUser()
{
    system("cls");
    string id = askForId();
    if (checkIdExists(id)) {
        printf("The id already exists.\n");
        registerUser();
    }

    string password = askForPassword();
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

bool LoginManager::checkIdExists(const string& id) 
{
    // 해당 아이디가 존재하는지 확인
    return true;
}

bool LoginManager::askForUserConfirmation(const string& id, const string& password) 
{
    if (!checkIdExists(id)) {
        cout << "Id or password do not match. Please try again." << endl;
        Sleep(2000);
        return false;
    }

    string confirm_password; // 데이터베이스 쿼리 문에서 받아오기

    if (confirm_password != password) {
        cout << "Id or password do not match. Please try again." << endl;
        Sleep(2000);
        return false;
    }

    return true;
}