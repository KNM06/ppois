#include <gtest/gtest.h>
#include "../../include/exceptions/LicenseValidationException.h"

TEST(LicenseValidationExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw LicenseValidationException("Invalid license for equipment operation");
    }, LicenseValidationException);
    
    try {
        throw LicenseValidationException("Test message");
    } catch (const LicenseValidationException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
