#ifndef SERVICE1_OPENACCOUNT_CONTROL_H
#define SERVICE1_OPENACCOUNT_CONTROL_H

#include "INetworkTransport.h"
#include <string>

class Service1_OpenAccountControl {
    INetworkTransport* transport;
public:
    explicit Service1_OpenAccountControl(INetworkTransport* t);
    void execute(const std::string& name, const std::string& password, Currency currency, float balance);
};
#endif
