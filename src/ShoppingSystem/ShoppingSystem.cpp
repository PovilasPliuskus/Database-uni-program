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

void ShoppingSystem::GetRidOfProduct()
{
    PrintTable("popl8979.Produktas");
    int index = RetrieveIndex("popl8979.Produktas", "ProduktoID");
    if (index == -1)
    {
        return;
    }
    else
    {
        DeleteProduct(index);
    }
}

void ShoppingSystem::DeleteProduct(int index)
{
    if (!_conn)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return;
    }

    // Construct the SQL query to delete the product
    std::string sqlQuery = "DELETE FROM popl8979.Produktas WHERE produktoid = " + std::to_string(index) + ";";

    // Execute the SQL query
    PGresult *result = PQexec(_conn, sqlQuery.c_str());

    // Check for a successful query execution
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cerr << "Query execution failed: " << PQerrorMessage(_conn) << std::endl;
    }
    else
    {
        std::cout << "Product deleted successfully!" << std::endl;
    }

    // Clear the result object
    PQclear(result);
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
    case 4:
        AddNewProduct();
        break;
    case 5:
        ChangeProductPrice();
        break;
    case 6:
        GetRidOfProduct();
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

void ShoppingSystem::AddNewProduct()
{
    std::string productName = RetrieveProductName();
    float productPrice = RetrieveProductPrice();

    InsertProduct(productName, productPrice);
}

std::string ShoppingSystem::RetrieveProductName()
{
    std::string productName;
    std::regex latinLetters("^[a-zA-Z]+$");

    do
    {
        std::cout << "Enter product name: ";
        std::cin >> productName;

        if (!std::regex_match(productName, latinLetters))
        {
            std::cerr << "Invalid input. Product name should contain only Latin letters" << std::endl;
        }
        else
        {
            break;
        }
    } while (true);

    return productName;
}

float ShoppingSystem::RetrieveProductPrice()
{
    float productPrice;

    do
    {
        std::cout << "Enter product price: ";
        std::string input;
        std::cin >> input;

        std::istringstream iss(input);
        if (iss >> std::noskipws >> productPrice && iss.eof())
        {
            break;
        }
        else
        {
            std::cerr << "Invalid input. Please enter a valid floating-point number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (true);

    std::cout.precision(2);

    return productPrice;
}

void ShoppingSystem::InsertProduct(const std::string &productName, float productPrice)
{
    if (!_conn)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return;
    }

    const char *sql = "INSERT INTO popl8979.Produktas (Pavadinimas, Kaina) VALUES ($1, $2);";

    const char *paramValues[2] = {productName.c_str(), std::to_string(productPrice).c_str()};
    int paramLengths[2] = {static_cast<int>(productName.length()), static_cast<int>(strlen(paramValues[1]))};
    int paramFormats[2] = {0, 0}; // 0 for text

    PGresult *result = PQexecParams(_conn, sql, 2, nullptr, paramValues, paramLengths, paramFormats, 0);

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cerr << "Query execution failed: " << PQerrorMessage(_conn) << std::endl;
    }
    else
    {
        std::cout << "-------------------------------" << std::endl;
        std::cout << "New product added successfully!" << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }

    PQclear(result);
}

void ShoppingSystem::ChangeProductPrice()
{
    PrintTable("popl8979.Produktas");
    int index = RetrieveIndex("popl8979.Produktas", "ProduktoID");
    if (index == -1)
    {
        return;
    }
    else
    {
        float newPrice = RetrieveProductPrice();
        UpdateProductPrice(index, newPrice);
    }
}

void ShoppingSystem::UpdateProductPrice(int index, float newPrice)
{
    if (!_conn)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return;
    }

    // Construct the SQL query to update the price
    std::string sqlQuery = "UPDATE popl8979.Produktas SET Kaina = " + std::to_string(newPrice) + " WHERE ProduktoID = " + std::to_string(index) + ";";

    // Execute the SQL query
    PGresult *result = PQexec(_conn, sqlQuery.c_str());

    // Check for a successful query execution
    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cerr << "Query execution failed: " << PQerrorMessage(_conn) << std::endl;
    }
    else
    {
        std::cout << "Product price updated successfully!" << std::endl;
    }

    // Clear the result object
    PQclear(result);
}

int ShoppingSystem::RetrieveIndex(std::string tableName, std::string idColumnName)
{
    if (!_conn)
    {
        std::cerr << "Not connected to the database." << std::endl;
        return -1; // or some sentinel value indicating failure
    }

    // Get user input for the ID
    int userInputId;
    std::cout << "Enter the " << idColumnName << ": ";

    while (!(std::cin >> userInputId) || std::cin.peek() != '\n')
    {
        std::cerr << "Invalid input. Please enter a valid integer." << std::endl;

        // Clear the error flag and discard invalid input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Prompt the user again
        std::cout << "Enter the " << idColumnName << ": ";
    }

    // Construct the SQL query to validate the ID
    std::string sqlQuery = "SELECT COUNT(*) FROM " + tableName + " WHERE " + idColumnName + " = " + std::to_string(userInputId) + ";";

    // Execute the SQL query
    PGresult *result = PQexec(_conn, sqlQuery.c_str());

    // Check for a successful query execution
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cerr << "Query execution failed: " << PQerrorMessage(_conn) << std::endl;
        PQclear(result);
        return -1; // or some sentinel value indicating failure
    }

    // Get the count from the result
    int rowCount = std::stoi(PQgetvalue(result, 0, 0));

    // Clear the result object
    PQclear(result);

    // Check if the ID exists in the table
    if (rowCount > 0)
    {
        return userInputId;
    }
    else
    {
        std::cerr << "ID " << userInputId << " does not exist in the table." << std::endl;
        return -1; // or some sentinel value indicating failure
    }
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