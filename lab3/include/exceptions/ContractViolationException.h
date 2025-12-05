#ifndef CONTRACTVIOLATIONEXCEPTION_H
#define CONTRACTVIOLATIONEXCEPTION_H

#include "RentalException.h"
#include <string>

class ContractViolationException : public RentalException {
public:
    explicit ContractViolationException(const std::string& contractId);
    virtual ~ContractViolationException() = default;
};

#endif