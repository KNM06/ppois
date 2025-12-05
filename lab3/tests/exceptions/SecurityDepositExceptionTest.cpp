#include <gtest/gtest.h>
#include "../../include/exceptions/SecurityDepositException.h"

TEST(SecurityDepositExceptionTest, ConstructorSetsMessage) {
    std::string depositId = "DEP123";
    try {
        throw SecurityDepositException(depositId);
    } catch (const SecurityDepositException& ex) {
        EXPECT_STREQ(ex.what(), ("Security deposit issue: " + depositId).c_str());
    } catch (...) {
        FAIL() << "Expected SecurityDepositException";
    }
}
