#include <gtest/gtest.h>
#include "../../include/exceptions/EquipmentDamageException.h"

TEST(EquipmentDamageExceptionTest, ConstructorAndMessage) {
    EXPECT_THROW({
        throw EquipmentDamageException("Equipment damaged during rental");
    }, EquipmentDamageException);
    
    try {
        throw EquipmentDamageException("Test message");
    } catch (const EquipmentDamageException& e) {
        EXPECT_STREQ(e.what(), "Test message");
    }
}
