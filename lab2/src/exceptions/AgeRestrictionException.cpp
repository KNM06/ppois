#include "../../include/exceptions/AgeRestrictionException.h"

AgeRestrictionException::AgeRestrictionException(const std::string& message)
    : std::runtime_error(message) {}

AgeRestrictionException::~AgeRestrictionException() = default;