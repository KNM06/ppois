#include <gtest/gtest.h>
#include "../../include/exceptions/PricingCalculationException.h"

TEST(PricingCalculationExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw PricingCalculationException("Pricing calculation error");
    }, PricingCalculationException);
    
    try {
        throw PricingCalculationException("Test message");
    } catch (const PricingCalculationException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
