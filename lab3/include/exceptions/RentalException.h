#ifndef RENTALEXCEPTION_H
#define RENTALEXCEPTION_H

#include <stdexcept>
#include <string>

class RentalException : public std::runtime_error {
public:
    explicit RentalException(const std::string& message);
    virtual ~RentalException() = default;
};

#endif