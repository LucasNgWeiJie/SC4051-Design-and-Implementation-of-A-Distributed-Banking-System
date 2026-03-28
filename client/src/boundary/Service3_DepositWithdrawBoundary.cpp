#include "Service3_DepositWithdrawBoundary.h"
#include <iostream>

Service3_DepositWithdrawBoundary::Service3_DepositWithdrawBoundary(Service3_DepositWithdrawControl* c) : control(c) {}
void Service3_DepositWithdrawBoundary::execute() {
    std::string name, pass; int32_t acc; int cur; float amt;
    std::cout << "Name: "; std::cin >> name;
    std::cout << "Account Number: "; std::cin >> acc;
    std::cout << "Password: "; std::cin >> pass;
    std::cout << "Currency (1=SGD, 2=USD, 3=EUR): "; std::cin >> cur;
    std::cout << "Amount (use native negative value to withdraw): "; std::cin >> amt;
    control->execute(name, acc, pass, static_cast<Currency>(cur), amt);
}