#include "HeaderFiles/ShoppingSystem.h"

ShoppingSystem::ShoppingSystem(std::string dbName, std::string username, std::string password)
    : _dbName(dbName), _username(username), _password(password)
{
    ConnectToDataBase();
}

void ShoppingSystem::ConnectToDataBase()
{
    std::cout << "Connecting to Database!" << std::endl;

    std::string connectionParams = "dbname=" + _dbName + " user=" + _username + " password=" + _password;
    PGconn *conn = PQconnectdb(connectionParams.c_str());

    // Check for a successful connection
    if (PQstatus(conn) != CONNECTION_OK)
    {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn); // Close the connection
        // Handle the error or return without further action
        return;
    }

    // Perform database operations here...

    // Close the connection
    PQfinish(conn);
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