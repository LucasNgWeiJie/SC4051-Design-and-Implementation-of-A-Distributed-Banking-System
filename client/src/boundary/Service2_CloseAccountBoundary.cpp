#include "Service2_CloseAccountBoundary.h"
#include <iostream>

Service2_CloseAccountBoundary::Service2_CloseAccountBoundary(Service2_CloseAccountControl* c) : control(c) {}
void Service2_CloseAccountBoundary::execute() {
    std::string name, pass; int32_t acc;
    std::cout << "Name: "; std::cin >> name;
    std::cout << "Account Number: "; std::cin >> acc;
    std::cout << "Password: "; std::cin >> pass;
    control->execute(name, acc, pass);
}