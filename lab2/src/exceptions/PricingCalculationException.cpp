#include "../../include/exceptions/PricingCalculationException.h"

PricingCalculationException::PricingCalculationException(const std::string& message)
    : std::runtime_error(message) {}

PricingCalculationException::~PricingCalculationException() = default;