#include "Service6_MonitorUpdatesBoundary.h"
#include <iostream>

Service6_MonitorUpdatesBoundary::Service6_MonitorUpdatesBoundary(Service6_MonitorUpdatesControl* c) : control(c) {}
void Service6_MonitorUpdatesBoundary::execute() {
    int32_t interval;
    std::cout << "Interval (seconds): "; std::cin >> interval;
    control->execute(interval);
}