#ifndef SERVICE2_CLOSEACCOUNT_CONTROL_H
#define SERVICE2_CLOSEACCOUNT_CONTROL_H

#include "INetworkTransport.h"
#include <string>

class Service2_CloseAccountControl {
    INetworkTransport* transport;
public:
    explicit Service2_CloseAccountControl(INetworkTransport* t);
    void execute(const std::string& name, int32_t account_num, const std::string& password);
};
#endif
