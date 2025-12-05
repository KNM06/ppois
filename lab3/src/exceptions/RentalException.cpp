#include "../../include/exceptions/RentalException.h"

RentalException::RentalException(const std::string& message) 
    : std::runtime_error(message) {}