#include <gtest/gtest.h>
#include "../../include/exceptions/MaintenanceOverdueException.h"

TEST(MaintenanceOverdueExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw MaintenanceOverdueException("Maintenance overdue for equipment");
    }, MaintenanceOverdueException);
    
    try {
        throw MaintenanceOverdueException("Test message");
    } catch (const MaintenanceOverdueException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
