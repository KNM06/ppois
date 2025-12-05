#include "../../include/exceptions/RentalItemUnavailableException.h"

RentalItemUnavailableException::RentalItemUnavailableException(const std::string& message)
    : std::runtime_error(message) {}