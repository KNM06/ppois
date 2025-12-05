#include <gtest/gtest.h>
#include "../../include/exceptions/MaintenanceEmergencyException.h"

TEST(MaintenanceEmergencyExceptionTest, ConstructorSetsMessage) {
    std::string requestId = "REQ123";
    try {
        throw MaintenanceEmergencyException(requestId);
    } catch (const MaintenanceEmergencyException& ex) {
        std::string expected = "Emergency maintenance required for request: " + requestId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected MaintenanceEmergencyException";
    }
}
