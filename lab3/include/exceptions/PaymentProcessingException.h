#ifndef PAYMENTPROCESSINGEXCEPTION_H
#define PAYMENTPROCESSINGEXCEPTION_H

#include "RentalException.h"
#include <string>

class PaymentProcessingException : public RentalException {
public:
    explicit PaymentProcessingException(const std::string& paymentId);
    virtual ~PaymentProcessingException() = default;
};

#endif