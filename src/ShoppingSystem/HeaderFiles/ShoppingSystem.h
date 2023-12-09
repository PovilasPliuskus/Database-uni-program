#ifndef SHOPPINGSYSTEM_H
#define SHOPPINGSYSTEM_H

#include <string>
#include <iostream>
#include <limits>
#include "../../../include/libpq-fe.h"

class ShoppingSystem
{
private:
    std::string _dbName;
    std::string _username;
    std::string _password;
    PGconn *_conn;

    bool ConnectToDataBase();
    void DisconnectFromDataBase();
    void ShowMenu();
    int RetrieveUsersInput();
    bool IsValidInput(int input);
    void ExecuteUsersCommand(int input);

public:
    ShoppingSystem(std::string dbName, std::string username, std::string password);
    void Start();

    std::string getDbName() const;
    std::string getUsername() const;
    std::string getPassword() const;
};

#endif // SHOPPINGSYSTEM_H