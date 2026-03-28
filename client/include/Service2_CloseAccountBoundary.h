#ifndef SERVICE2_CLOSEACCOUNT_BOUNDARY_H
#define SERVICE2_CLOSEACCOUNT_BOUNDARY_H

#include "Service2_CloseAccountControl.h"

class Service2_CloseAccountBoundary {
    Service2_CloseAccountControl* control;
public:
    explicit Service2_CloseAccountBoundary(Service2_CloseAccountControl* c);
    void execute();
};
#endif
