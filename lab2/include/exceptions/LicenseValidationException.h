#ifndef LICENSEVALIDATIONEXCEPTION_H
#define LICENSEVALIDATIONEXCEPTION_H

#include <stdexcept>
#include <string>

class LicenseValidationException : public std::runtime_error {
public:
    explicit LicenseValidationException(const std::string& message);
    virtual ~LicenseValidationException();
};

#endif