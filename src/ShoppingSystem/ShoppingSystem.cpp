#include "HeaderFiles/ShoppingSystem.h"

ShoppingSystem::ShoppingSystem(std::string dbName, std::string username, std::string password)
    : _dbName(dbName), _username(username), _password(password), _conn(nullptr) {}

void ShoppingSystem::Start()
{
    if (ConnectToDataBase())
    {
        while (_conn != nullptr)
        {
            ShowMenu();
            int usersInput = RetrieveUsersInput();
            ExecuteUsersCommand(usersInput);
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
    std::cout << "1. View all shops" << std::endl;
    std::cout << "2. View all products" << std::endl;
    std::cout << "3. View shops and products" << std::endl;
    std::cout << "4. Add a new product" << std::endl;
    std::cout << "5. Change the price of a product" << std::endl;
    std::cout << "6. Get rid of a product" << std::endl;
}

int ShoppingSystem::RetrieveUsersInput()
{
    int input;

    while (true)
    {
        std::cout << "> ";
        if (std::cin >> input)
        {
            if (IsValidInput(input))
            {
                break;
            }
            else
            {
                std::cerr << "There is no such command!" << std::endl;
            }
        }
        else
        {
            std::cerr << "There is no such command!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return input;
}

bool ShoppingSystem::IsValidInput(int input)
{
    return input >= 0 && input <= 6;
}

void ShoppingSystem::ExecuteUsersCommand(int input)
{
    switch (input)
    {
    case 0:
        DisconnectFromDataBase();
        break;
    case 1:
        PrintTable("popl8979.Parduotuve");
        break;
    case 2:
        PrintTable("popl8979.Produktas");
        break;
    case 3:
        PrintTable("popl8979.ParduotuvesIrProduktai");
        break;
    default:
        std::cout << "Not implemented, yet" << std::endl;
    }
}

void ShoppingSystem::PrintTable(std::string tableName)
{
    if (!_conn)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return;
    }

    std::string sqlQuery = "SELECT * FROM " + tableName + ";";
    PGresult *result = PQexec(_conn, sqlQuery.c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query execution failed: " << PQerrorMessage(_conn) << std::endl;
        PQclear(result);
        return;
    }

    // Get the number of rows and columns in the result
    int numRows = PQntuples(result);
    int numCols = PQnfields(result);

    int colWidth = 100 / numCols;

    // Print column names
    for (int col = 0; col < numCols - 1; ++col)
    {
        std::cout << std::setw(colWidth) << std::left << PQfname(result, col) << "|";
    }
    // Print the last column without the '|'
    std::cout << std::setw(colWidth) << std::left << PQfname(result, numCols - 1) << std::endl;

    // Print line of dashes
    for (int col = 0; col <= 100; ++col)
    {
        std::cout << "-";
    }
    std::cout << std::endl;

    // Print the data
    for (int row = 0; row < numRows; ++row)
    {
        for (int col = 0; col < numCols - 1; ++col)
        {
            std::cout << std::setw(colWidth) << std::left << PQgetvalue(result, row, col) << "|";
        }
        // Print the last column without the '|'
        std::cout << std::setw(colWidth) << std::left << PQgetvalue(result, row, numCols - 1) << std::endl;
    }
    std::cout << std::endl;

    // Clear the result object
    PQclear(result);
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