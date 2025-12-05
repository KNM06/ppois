#include "../../include/exceptions/MaintenanceEmergencyException.h"

MaintenanceEmergencyException::MaintenanceEmergencyException(const std::string& requestId)
    : RentalException("Emergency maintenance required for request: " + requestId) {}