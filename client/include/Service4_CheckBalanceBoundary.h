#ifndef SERVICE4_CHECKBALANCE_BOUNDARY_H
#define SERVICE4_CHECKBALANCE_BOUNDARY_H

#include "Service4_CheckBalanceControl.h"

class Service4_CheckBalanceBoundary {
    Service4_CheckBalanceControl* control;
public:
    explicit Service4_CheckBalanceBoundary(Service4_CheckBalanceControl* c);
    void execute();
};
#endif
