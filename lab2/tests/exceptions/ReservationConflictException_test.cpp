#include <gtest/gtest.h>
#include "../../include/exceptions/ReservationConflictException.h"

TEST(ReservationConflictExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw ReservationConflictException("Reservation conflict detected");
    }, ReservationConflictException);
    
    try {
        throw ReservationConflictException("Test message");
    } catch (const ReservationConflictException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
