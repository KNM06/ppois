#include "../../include/exceptions/MaintenanceOverdueException.h"

MaintenanceOverdueException::MaintenanceOverdueException(const std::string& message)
    : std::runtime_error(message) {}

MaintenanceOverdueException::~MaintenanceOverdueException() = default;