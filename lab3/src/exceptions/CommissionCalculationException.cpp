#include "../../include/exceptions/CommissionCalculationException.h"

CommissionCalculationException::CommissionCalculationException(const std::string& agentId)
    : RentalException("Commission calculation error for agent: " + agentId) {}