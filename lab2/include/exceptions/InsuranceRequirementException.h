#ifndef INSURANCEREQUIREMENTEXCEPTION_H
#define INSURANCEREQUIREMENTEXCEPTION_H

#include <stdexcept>
#include <string>

class InsuranceRequirementException : public std::runtime_error {
public:
    explicit InsuranceRequirementException(const std::string& message);
    virtual ~InsuranceRequirementException();
};

#endif