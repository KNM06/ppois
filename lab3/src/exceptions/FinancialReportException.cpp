#include "../../include/exceptions/FinancialReportException.h"

FinancialReportException::FinancialReportException(const std::string& reportId)
    : RentalException("Financial report generation failed: " + reportId) {}