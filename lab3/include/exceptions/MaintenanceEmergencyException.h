#ifndef MAINTENANCEEMERGENCYEXCEPTION_H
#define MAINTENANCEEMERGENCYEXCEPTION_H

#include "RentalException.h"
#include <string>

class MaintenanceEmergencyException : public RentalException {
public:
    explicit MaintenanceEmergencyException(const std::string& requestId);
    virtual ~MaintenanceEmergencyException() = default;
};

#endif