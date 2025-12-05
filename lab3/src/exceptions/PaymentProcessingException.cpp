#include "../../include/exceptions/PaymentProcessingException.h"

PaymentProcessingException::PaymentProcessingException(const std::string& paymentId)
    : RentalException("Failed to process payment: " + paymentId) {}