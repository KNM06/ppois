#include <gtest/gtest.h>
#include "../../include/exceptions/AgeRestrictionException.h"

TEST(AgeRestrictionExceptionTest, ConstructorAndMessage) {
    try {
        throw AgeRestrictionException("Customer is underage for this item");
        FAIL() << "Expected AgeRestrictionException";
    } catch (const AgeRestrictionException& e) {
        EXPECT_STREQ(e.what(), "Customer is underage for this item");
    }
    
    try {
        throw AgeRestrictionException("Test message");
        FAIL() << "Expected AgeRestrictionException";
    } catch (const AgeRestrictionException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
