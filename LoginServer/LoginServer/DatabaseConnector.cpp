#include "DatabaseConnector.h"

void DatabaseConnector::init()
{
    driver = get_driver_instance();
    conn = unique_ptr<sql::Connection>(driver->connect(server, name, password));
    conn->setSchema("auth_system");

    cout << "DatabaseConnector initialized successfully\n";
}

bool DatabaseConnector::logIn(string id, string password)
{
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        string query = "SELECT * FROM users WHERE id = ?";

        pstmt = conn->prepareStatement(query);
        pstmt->setString(1, id);
        res = pstmt->executeQuery();

        if (res->next()) {
            string selectedId = res->getString("id");
            string selectedPassword = res->getString("password");
            cout << "id = " << selectedId;
            cout << "password = " << selectedPassword;
        
            if (selectedPassword != password) {
                cout << "Password is incorred\n" << endl;
                delete pstmt;
                delete res;
                return false;
            }
        }
        else {
            cout << "No user found with the given id\n" << endl;
            delete pstmt;
            delete res;
            return false;
        }
        
        delete pstmt;
        delete res;
        return true;
    }
    catch (sql::SQLException& e) {
        cout << "SQL error: " << e.what() << endl;
        if (pstmt) delete pstmt;
        if (res) delete res;
        return false;
    }
    catch (std::exception& e) {
        cout << "Error: " << e.what() << endl;
        if (pstmt) delete pstmt;
        if (res) delete res;
        return false;
    }
}

bool DatabaseConnector::registerUser(string id, string password)
{
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        string query = "SELECT id FROM users WHERE id = ?";
        pstmt = conn->prepareStatement(query);

        pstmt->setString(1, id);

        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "The given id already exists\n" << endl;
            delete pstmt;
            delete res;
            return false;
        }

        query = "INSERT INTO users (id, password) VALUES (?, ?)";
        pstmt = conn->prepareStatement(query); 

        pstmt->setString(1, id);
        pstmt->setString(2, password);

        pstmt->executeUpdate();

        delete pstmt;
        delete res;
        return true;
    }
    catch (sql::SQLException& e) {
        cout << "SQL error: " << e.what() << endl;
        if (pstmt) delete pstmt;
        if (res) delete res;
        return false;
    }
    catch (std::exception& e) {
        cout << "Error: " << e.what() << endl;
        if (pstmt) delete pstmt;
        if (res) delete res;
        return false;
    }
}

void DatabaseConnector::runDatabaseConnector()
{
    cout << "************* Running Connector *************\n";
    init();
}

void DatabaseConnector::closeDatabaseConnector()
{
}
