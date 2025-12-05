#include <gtest/gtest.h>
#include "../../include/exceptions/ContractViolationException.h"

TEST(ContractViolationExceptionTest, ConstructorSetsMessage) {
    std::string contractId = "C456";
    try {
        throw ContractViolationException(contractId);
    } catch (const ContractViolationException& ex) {
        std::string expected = "Contract violation detected: " + contractId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected ContractViolationException";
    }
}
