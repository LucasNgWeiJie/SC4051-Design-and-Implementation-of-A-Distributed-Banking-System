#ifndef SERVICE5_TRANSFER_BOUNDARY_H
#define SERVICE5_TRANSFER_BOUNDARY_H

#include "Service5_TransferControl.h"

class Service5_TransferBoundary {
    Service5_TransferControl* control;
public:
    explicit Service5_TransferBoundary(Service5_TransferControl* c);
    void execute();
};
#endif
