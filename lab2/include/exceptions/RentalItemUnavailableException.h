#ifndef RENTALITEMUNAVAILABLEEXCEPTION_H
#define RENTALITEMUNAVAILABLEEXCEPTION_H

#include <stdexcept>
#include <string>

class RentalItemUnavailableException : public std::runtime_error {
public:
    explicit RentalItemUnavailableException(const std::string& message);
    virtual ~RentalItemUnavailableException() = default;
};

#endif