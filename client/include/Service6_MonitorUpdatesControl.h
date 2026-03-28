#ifndef SERVICE6_MONITORUPDATES_CONTROL_H
#define SERVICE6_MONITORUPDATES_CONTROL_H

#include "INetworkTransport.h"
#include <string>

class Service6_MonitorUpdatesControl {
    INetworkTransport* transport;
public:
    explicit Service6_MonitorUpdatesControl(INetworkTransport* t);
    void execute(int32_t interval_seconds);
};
#endif
