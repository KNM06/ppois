#include <gtest/gtest.h>
#include "../../include/exceptions/InvalidLeaseTermException.h"

TEST(InvalidLeaseTermExceptionTest, ConstructorSetsMessage) {
    int term = 5;
    try {
        throw InvalidLeaseTermException(term);
    } catch (const InvalidLeaseTermException& ex) {
        std::string expected = "Invalid lease term: " + std::to_string(term) + " months";
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected InvalidLeaseTermException";
    }
}
