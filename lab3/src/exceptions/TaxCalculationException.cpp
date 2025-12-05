#include "../../include/exceptions/TaxCalculationException.h"

TaxCalculationException::TaxCalculationException(const std::string& propertyId)
    : RentalException("Tax calculation error for property: " + propertyId) {}