#include "Service5_TransferBoundary.h"
#include <iostream>

Service5_TransferBoundary::Service5_TransferBoundary(Service5_TransferControl* c) : control(c) {}
void Service5_TransferBoundary::execute() {
    std::string name, pass; int32_t from_acc, to_acc; float amt;
    std::cout << "Name: "; std::cin >> name;
    std::cout << "Source Account: "; std::cin >> from_acc;
    std::cout << "Password: "; std::cin >> pass;
    std::cout << "Dest Account: "; std::cin >> to_acc;
    std::cout << "Amount: "; std::cin >> amt;
    control->execute(name, from_acc, pass, to_acc, amt);
}