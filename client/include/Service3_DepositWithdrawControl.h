#ifndef SERVICE3_DEPOSITWITHDRAW_CONTROL_H
#define SERVICE3_DEPOSITWITHDRAW_CONTROL_H

#include "INetworkTransport.h"
#include <string>

class Service3_DepositWithdrawControl {
    INetworkTransport* transport;
public:
    explicit Service3_DepositWithdrawControl(INetworkTransport* t);
    void execute(const std::string& name, int32_t account_num, const std::string& password, Currency currency, float amount);
};
#endif
