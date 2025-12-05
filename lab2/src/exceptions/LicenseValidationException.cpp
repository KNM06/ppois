#include "../../include/exceptions/LicenseValidationException.h"

LicenseValidationException::LicenseValidationException(const std::string& message)
    : std::runtime_error(message) {}

LicenseValidationException::~LicenseValidationException() = default;