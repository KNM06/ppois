#include <gtest/gtest.h>
#include "../../include/exceptions/InventoryMismatchException.h"

TEST(InventoryMismatchExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw InventoryMismatchException("Inventory count mismatch detected");
    }, InventoryMismatchException);
    
    try {
        throw InventoryMismatchException("Test message");
    } catch (const InventoryMismatchException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}

