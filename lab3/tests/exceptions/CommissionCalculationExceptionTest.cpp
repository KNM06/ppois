#include <gtest/gtest.h>
#include "../../include/exceptions/CommissionCalculationException.h"

TEST(CommissionCalculationExceptionTest, ConstructorSetsMessage) {
    std::string agentId = "A123";
    try {
        throw CommissionCalculationException(agentId);
    } catch (const CommissionCalculationException& ex) {
        std::string expected = "Commission calculation error for agent: " + agentId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected CommissionCalculationException";
    }
}
