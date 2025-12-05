#include "../../include/exceptions/SecurityDepositException.h"

SecurityDepositException::SecurityDepositException(const std::string& depositId)
    : RentalException("Security deposit issue: " + depositId) {}