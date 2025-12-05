#include "../../include/exceptions/ReservationConflictException.h"

ReservationConflictException::ReservationConflictException(const std::string& message)
    : std::runtime_error(message) {}

ReservationConflictException::~ReservationConflictException() = default;