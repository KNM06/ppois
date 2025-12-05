#ifndef PRICINGCALCULATIONEXCEPTION_H
#define PRICINGCALCULATIONEXCEPTION_H

#include <stdexcept>
#include <string>

class PricingCalculationException : public std::runtime_error {
public:
    explicit PricingCalculationException(const std::string& message);
    virtual ~PricingCalculationException();
};

#endif