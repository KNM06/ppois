#ifndef FINANCIALREPORTEXCEPTION_H
#define FINANCIALREPORTEXCEPTION_H

#include "RentalException.h"
#include <string>

class FinancialReportException : public RentalException {
public:
    explicit FinancialReportException(const std::string& reportId);
    virtual ~FinancialReportException() = default;
};

#endif