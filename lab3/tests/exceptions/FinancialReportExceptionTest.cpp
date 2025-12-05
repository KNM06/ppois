#include <gtest/gtest.h>
#include "../../include/exceptions/FinancialReportException.h"

TEST(FinancialReportExceptionTest, ConstructorSetsMessage) {
    std::string reportId = "R123";
    try {
        throw FinancialReportException(reportId);
    } catch (const FinancialReportException& ex) {
        std::string expected = "Financial report generation failed: " + reportId;
        EXPECT_STREQ(ex.what(), expected.c_str());
    } catch (...) {
        FAIL() << "Expected FinancialReportException";
    }
}
