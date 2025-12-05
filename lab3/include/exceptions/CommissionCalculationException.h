#ifndef COMMISSIONCALCULATIONEXCEPTION_H
#define COMMISSIONCALCULATIONEXCEPTION_H

#include "RentalException.h"
#include <string>

class CommissionCalculationException : public RentalException {
public:
    explicit CommissionCalculationException(const std::string& agentId);
    virtual ~CommissionCalculationException() = default;
};

#endif