#ifndef SERVICE3_DEPOSITWITHDRAW_BOUNDARY_H
#define SERVICE3_DEPOSITWITHDRAW_BOUNDARY_H

#include "Service3_DepositWithdrawControl.h"

class Service3_DepositWithdrawBoundary {
    Service3_DepositWithdrawControl* control;
public:
    explicit Service3_DepositWithdrawBoundary(Service3_DepositWithdrawControl* c);
    void execute();
};
#endif
