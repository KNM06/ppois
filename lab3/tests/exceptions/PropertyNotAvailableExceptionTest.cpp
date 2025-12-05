#include <gtest/gtest.h>
#include "../../include/exceptions/PropertyNotAvailableException.h"

TEST(PropertyNotAvailableExceptionTest, ConstructorSetsMessage) {
    std::string propertyId = "PROP123";
    try {
        throw PropertyNotAvailableException(propertyId);
    } catch (const PropertyNotAvailableException& ex) {
        std::string expected = "Property " + propertyId + " is not available for rent";
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected PropertyNotAvailableException";
    }
}
