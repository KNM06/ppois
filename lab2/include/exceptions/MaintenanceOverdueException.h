#ifndef MAINTENANCEOVERDUEEXCEPTION_H
#define MAINTENANCEOVERDUEEXCEPTION_H

#include <stdexcept>
#include <string>

class MaintenanceOverdueException : public std::runtime_error {
public:
    explicit MaintenanceOverdueException(const std::string& message);
    virtual ~MaintenanceOverdueException();
};

#endif