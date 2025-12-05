#include <gtest/gtest.h>
#include "../../include/exceptions/TaxCalculationException.h"

TEST(TaxCalculationExceptionTest, ConstructorSetsMessage) {
    std::string propertyId = "PROP456";
    try {
        throw TaxCalculationException(propertyId);
    } catch (const TaxCalculationException& ex) {
        EXPECT_STREQ(ex.what(), ("Tax calculation error for property: " + propertyId).c_str());
    } catch (...) {
        FAIL() << "Expected TaxCalculationException";
    }
}
