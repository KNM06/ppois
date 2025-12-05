#include "../../include/exceptions/InsuranceRequirementException.h"

InsuranceRequirementException::InsuranceRequirementException(const std::string& message)
    : std::runtime_error(message) {}

InsuranceRequirementException::~InsuranceRequirementException() = default;