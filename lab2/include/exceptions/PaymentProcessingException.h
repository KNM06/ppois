#ifndef PAYMENTPROCESSINGEXCEPTION_H
#define PAYMENTPROCESSINGEXCEPTION_H

#include <stdexcept>
#include <string>

class PaymentProcessingException : public std::runtime_error {
public:
    explicit PaymentProcessingException(const std::string& message);
    virtual ~PaymentProcessingException();
};

#endif