#include <iostream>
// Custom Libraries
#include "ShoppingSystem/HeaderFiles/ShoppingSystem.h"

int main()
{

    ShoppingSystem shoppingSystem("dbvs", "postgres", "password");
    shoppingSystem.Start();

    return 0;
}