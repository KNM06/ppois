#ifndef TAXCALCULATIONEXCEPTION_H
#define TAXCALCULATIONEXCEPTION_H

#include "RentalException.h"
#include <string>

class TaxCalculationException : public RentalException {
public:
    explicit TaxCalculationException(const std::string& propertyId);
    virtual ~TaxCalculationException() = default;
};

#endif