#ifndef PROPERTYNOTAVAILABLEEXCEPTION_H
#define PROPERTYNOTAVAILABLEEXCEPTION_H

#include "RentalException.h"
#include <string>

class PropertyNotAvailableException : public RentalException {
public:
    explicit PropertyNotAvailableException(const std::string& propertyId);
    virtual ~PropertyNotAvailableException() = default;
};

#endif