#ifndef SERVICE5_TRANSFER_CONTROL_H
#define SERVICE5_TRANSFER_CONTROL_H

#include "INetworkTransport.h"
#include <string>

class Service5_TransferControl {
    INetworkTransport* transport;
public:
    explicit Service5_TransferControl(INetworkTransport* t);
    void execute(const std::string& name, int32_t from_account, const std::string& password, int32_t to_account, float amount);
};
#endif
