#include <gtest/gtest.h>
#include "../../include/exceptions/InsuranceRequirementException.h"

TEST(InsuranceRequirementExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw InsuranceRequirementException("Insurance required for this item");
    }, InsuranceRequirementException);
    
    try {
        throw InsuranceRequirementException("Test message");
    } catch (const InsuranceRequirementException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
