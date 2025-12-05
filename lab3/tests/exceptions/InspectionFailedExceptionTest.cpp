#include <gtest/gtest.h>
#include "../../include/exceptions/InspectionFailedException.h"

TEST(InspectionFailedExceptionTest, ConstructorSetsMessage) {
    std::string propertyId = "P456";
    try {
        throw InspectionFailedException(propertyId);
    } catch (const InspectionFailedException& ex) {
        std::string expected = "Property inspection failed: " + propertyId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected InspectionFailedException";
    }
}
