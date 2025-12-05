#ifndef RESERVATIONCONFLICTEXCEPTION_H
#define RESERVATIONCONFLICTEXCEPTION_H

#include <stdexcept>
#include <string>

class ReservationConflictException : public std::runtime_error {
public:
    explicit ReservationConflictException(const std::string& message);
    virtual ~ReservationConflictException();
};

#endif