#ifndef SHOPPINGSYSTEM_H
#define SHOPPINGSYSTEM_H

#include <string>

class ShoppingSystem
{
private:
    std::string _dbName;
    std::string _username;
    std::string _password;

public:
    ShoppingSystem(std::string dbName, std::string username, std::string password);

    std::string getDbName() const;
    std::string getUsername() const;
    std::string getPassword() const;
};

#endif // SHOPPINGSYSTEM_H