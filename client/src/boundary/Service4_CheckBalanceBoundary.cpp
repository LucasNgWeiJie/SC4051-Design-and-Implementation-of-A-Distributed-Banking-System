#include "Service4_CheckBalanceBoundary.h"
#include <iostream>

Service4_CheckBalanceBoundary::Service4_CheckBalanceBoundary(Service4_CheckBalanceControl* c) : control(c) {}
void Service4_CheckBalanceBoundary::execute() {
    std::string name, pass; int32_t acc;
    std::cout << "Name: "; std::cin >> name;
    std::cout << "Account Number: "; std::cin >> acc;
    std::cout << "Password: "; std::cin >> pass;
    control->execute(name, acc, pass);
}