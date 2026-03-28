#ifndef SERVICE4_CHECKBALANCE_CONTROL_H
#define SERVICE4_CHECKBALANCE_CONTROL_H

#include "INetworkTransport.h"
#include <string>

class Service4_CheckBalanceControl {
    INetworkTransport* transport;
public:
    explicit Service4_CheckBalanceControl(INetworkTransport* t);
    void execute(const std::string& name, int32_t account_num, const std::string& password);
};
#endif
