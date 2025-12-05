#ifndef AGERESTRICTIONEXCEPTION_H
#define AGERESTRICTIONEXCEPTION_H

#include <stdexcept>
#include <string>

class AgeRestrictionException : public std::runtime_error {
public:
    explicit AgeRestrictionException(const std::string& message);
    virtual ~AgeRestrictionException();
};

#endif