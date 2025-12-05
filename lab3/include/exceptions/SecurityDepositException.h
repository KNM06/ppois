#ifndef SECURITYDEPOSITEXCEPTION_H
#define SECURITYDEPOSITEXCEPTION_H

#include "RentalException.h"
#include <string>

class SecurityDepositException : public RentalException {
public:
    explicit SecurityDepositException(const std::string& depositId);
    virtual ~SecurityDepositException() = default;
};

#endif