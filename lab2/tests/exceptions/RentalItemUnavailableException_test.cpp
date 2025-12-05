#include <gtest/gtest.h>
#include "../../include/exceptions/RentalItemUnavailableException.h"

TEST(RentalItemUnavailableExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw RentalItemUnavailableException("Item ITEM001 is unavailable");
    }, RentalItemUnavailableException);
    
    try {
        throw RentalItemUnavailableException("Test message");
    } catch (const RentalItemUnavailableException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
