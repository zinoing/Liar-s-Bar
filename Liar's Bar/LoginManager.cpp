#include "LoginManager.h"

void LoginManager::login() const
{
    system("cls");
    string id = askForId();
    string password = askForPassword();
    if (!askForUserConfirmation(id, password)) login();
    return;
}

void LoginManager::registerUser() const
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

string LoginManager::askForId() const 
{
    string username;
    cout << "Please enter id: ";
    cin >> username;
    return username;
}

string LoginManager::askForPassword() const 
{
    string password;
    cout << "Please enter password: ";
    cin >> password;
    return password;
}

bool LoginManager::checkIdExists(const string& id) const 
{
    // 해당 아이디가 존재하는지 확인
    return true;
}

bool LoginManager::askForUserConfirmation(const string& id, const string& password) const 
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