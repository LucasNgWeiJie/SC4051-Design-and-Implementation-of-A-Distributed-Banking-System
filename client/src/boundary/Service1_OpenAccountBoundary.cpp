#include "Service1_OpenAccountBoundary.h"
#include <iostream>

Service1_OpenAccountBoundary::Service1_OpenAccountBoundary(Service1_OpenAccountControl* c) : control(c) {}
void Service1_OpenAccountBoundary::execute() {
    std::string name, pass; int cur; float bal;
    
    // Prompt the user for account creation information
    std::cout << "Name: "; std::cin >> name;
    std::cout << "Password: "; std::cin >> pass;
    std::cout << "Currency (1=SGD, 2=USD, 3=EUR): "; std::cin >> cur;
    std::cout << "Initial Balance: "; std::cin >> bal;
    
    // Pass the parsed input to the Control layer for marshalling
    control->execute(name, pass, static_cast<Currency>(cur), bal);
}