#include "HeaderFiles/ShoppingSystem.h"

ShoppingSystem::ShoppingSystem(std::string dbName, std::string username, std::string password)
    : _dbName(dbName), _username(username), _password(password)
{
    ConnectToDataBase();
}

void ShoppingSystem::ConnectToDataBase()
{
}

void ShoppingSystem::RunInitialSQLQueries(std::string dbName, std::string username, std::string password)
{
    system("start cmd");
}

std::string ShoppingSystem::getDbName() const
{
    return _dbName;
}

std::string ShoppingSystem::getUsername() const
{
    return _username;
}

std::string ShoppingSystem::getPassword() const
{
    return _password;
}