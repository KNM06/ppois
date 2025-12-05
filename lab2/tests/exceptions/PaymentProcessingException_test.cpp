#include <gtest/gtest.h>
#include "../../include/exceptions/PaymentProcessingException.h"

TEST(PaymentProcessingExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw PaymentProcessingException("Payment processing failed");
    }, PaymentProcessingException);
    
    try {
        throw PaymentProcessingException("Test message");
    } catch (const PaymentProcessingException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
