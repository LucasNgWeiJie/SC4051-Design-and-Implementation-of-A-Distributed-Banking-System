#ifndef SERVICE1_OPENACCOUNT_BOUNDARY_H
#define SERVICE1_OPENACCOUNT_BOUNDARY_H

#include "Service1_OpenAccountControl.h"

class Service1_OpenAccountBoundary {
    Service1_OpenAccountControl* control;
public:
    explicit Service1_OpenAccountBoundary(Service1_OpenAccountControl* c);
    void execute();
};
#endif
