#include <gtest/gtest.h>
#include "../../include/exceptions/PaymentProcessingException.h"

TEST(PaymentProcessingExceptionTest, ConstructorSetsMessage) {
    std::string paymentId = "PAY456";
    try {
        throw PaymentProcessingException(paymentId);
    } catch (const PaymentProcessingException& ex) {
        std::string expected = "Failed to process payment: " + paymentId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected PaymentProcessingException";
    }
}
