#include "HeaderFiles/ShoppingSystem.h"

ShoppingSystem::ShoppingSystem(std::string dbName, std::string username, std::string password)
    : _dbName(dbName), _username(username), _password(password), _conn(nullptr)
{
    // ConnectToDataBase();
    // DisconnectFromDataBase();
}

void ShoppingSystem::Start()
{
    if (ConnectToDataBase())
    {
        while (_conn != nullptr)
        {
            ShowMenu();
            DisconnectFromDataBase();
        }
    }
    else
    {
        std::cerr << "Failed to connect to the database." << std::endl;
    }
}

void ShoppingSystem::ShowMenu()
{
    std::cout << "0. Exit program" << std::endl;
    std::cout << "1. Something to do" << std::endl;
}

bool ShoppingSystem::ConnectToDataBase()
{
    std::cout << "Connecting to Database..." << std::endl;

    std::string connectionParams = "dbname=" + _dbName + " user=" + _username + " password=" + _password;
    _conn = PQconnectdb(connectionParams.c_str());

    // Check for a successful connection
    if (PQstatus(_conn) != CONNECTION_OK)
    {
        std::cerr << "Connection to database failed: " << PQerrorMessage(_conn) << std::endl;
        PQfinish(_conn); // Close the connection

        return false;
    }

    // Close the connection
    // PQfinish(_conn);
    std::cout << "Connection was successfull!" << std::endl;
    return true;
}

void ShoppingSystem::DisconnectFromDataBase()
{
    if (_conn)
    {
        PQfinish(_conn);
        std::cout << "Disconnected from the database." << std::endl;
        _conn = nullptr;
    }
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