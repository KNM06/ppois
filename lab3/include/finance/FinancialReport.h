#ifndef FINANCIALREPORT_H
#define FINANCIALREPORT_H

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "../../include/finance/Payment.h"
#include "../../include/finance/Invoice.h"

class FinancialReport {
private:
    std::string reportId;
    std::string reportType;
    std::chrono::system_clock::time_point reportDate;
    std::chrono::system_clock::time_point periodStart;
    std::chrono::system_clock::time_point periodEnd;
    double totalRevenue;
    double totalExpenses;
    double netIncome;
    std::vector<std::shared_ptr<Payment>> payments;
    std::vector<std::shared_ptr<Invoice>> invoices;
    std::string currency;
    bool isFinalized;

public:
    FinancialReport(const std::string& id, const std::string& type,
                   const std::chrono::system_clock::time_point& reportDate,
                   const std::chrono::system_clock::time_point& periodStart,
                   const std::chrono::system_clock::time_point& periodEnd,
                   double revenue, double expenses, double netIncome,
                   const std::vector<std::shared_ptr<Payment>>& payments,
                   const std::vector<std::shared_ptr<Invoice>>& invoices,
                   const std::string& currency, bool finalized);
    
    FinancialReport(const FinancialReport& other);
    FinancialReport& operator=(const FinancialReport& other);
    FinancialReport(FinancialReport&& other) noexcept;
    FinancialReport& operator=(FinancialReport&& other) noexcept;
    ~FinancialReport();

    std::string getReportId() const;
    std::string getReportType() const;
    std::chrono::system_clock::time_point getReportDate() const;
    std::chrono::system_clock::time_point getPeriodStart() const;
    std::chrono::system_clock::time_point getPeriodEnd() const;
    double getTotalRevenue() const;
    double getTotalExpenses() const;
    double getNetIncome() const;
    std::vector<std::shared_ptr<Payment>> getPayments() const;
    std::vector<std::shared_ptr<Invoice>> getInvoices() const;
    std::string getCurrency() const;
    bool getIsFinalized() const;

    void setTotalRevenue(double newRevenue);
    void setTotalExpenses(double newExpenses);
    void setIsFinalized(bool finalized);

    double calculateProfitMargin() const;
    bool isQuarterlyReport() const;
    bool isAnnualReport() const;
    double calculateExpenseRatio() const;
    bool hasPositiveCashFlow() const;
    double calculateAverageTransaction() const;
    std::string getFinancialHealth() const;
    double calculateTaxLiability(double taxRate) const;
    bool requiresAudit() const;
};

#endif