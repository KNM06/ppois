#include <gtest/gtest.h>
#include "../../include/exceptions/RentalException.h"

TEST(RentalExceptionTest, ConstructorSetsMessage) {
    std::string msg = "Base rental exception occurred";
    try {
        throw RentalException(msg);
    } catch (const RentalException& ex) {
        EXPECT_STREQ(ex.what(), msg.c_str());
    } catch (...) {
        FAIL() << "Expected RentalException";
    }
}
