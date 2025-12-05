#include <gtest/gtest.h>
#include "../../include/exceptions/SeasonalDemandException.h"

TEST(SeasonalDemandExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw SeasonalDemandException("High seasonal demand - item unavailable");
    }, SeasonalDemandException);
    
    try {
        throw SeasonalDemandException("Test message");
    } catch (const SeasonalDemandException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
