#include <gtest/gtest.h>
#include "../../include/exceptions/CustomerBlacklistedException.h"

TEST(CustomerBlacklistedExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw CustomerBlacklistedException("Customer CUST001 is blacklisted");
    }, CustomerBlacklistedException);
    
    try {
        throw CustomerBlacklistedException("Test message");
    } catch (const CustomerBlacklistedException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
