#include "../../include/exceptions/ContractViolationException.h"

ContractViolationException::ContractViolationException(const std::string& contractId)
    : RentalException("Contract violation detected: " + contractId) {}