#ifndef SERVICE6_MONITORUPDATES_BOUNDARY_H
#define SERVICE6_MONITORUPDATES_BOUNDARY_H

#include "Service6_MonitorUpdatesControl.h"

class Service6_MonitorUpdatesBoundary {
    Service6_MonitorUpdatesControl* control;
public:
    explicit Service6_MonitorUpdatesBoundary(Service6_MonitorUpdatesControl* c);
    void execute();
};
#endif
