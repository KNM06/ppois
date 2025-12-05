#include "../../include/exceptions/PaymentProcessingException.h"

PaymentProcessingException::PaymentProcessingException(const std::string& message)
    : std::runtime_error(message) {}

PaymentProcessingException::~PaymentProcessingException() = default;