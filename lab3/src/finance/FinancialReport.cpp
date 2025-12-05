#include "../../include/finance/FinancialReport.h"
#include <algorithm>
#include <cmath>

FinancialReport::FinancialReport(const std::string& id, const std::string& type,
                                 const std::chrono::system_clock::time_point& reportDate,
                                 const std::chrono::system_clock::time_point& periodStart,
                                 const std::chrono::system_clock::time_point& periodEnd,
                                 double revenue, double expenses, double netIncome,
                                 const std::vector<std::shared_ptr<Payment>>& payments,
                                 const std::vector<std::shared_ptr<Invoice>>& invoices,
                                 const std::string& currency, bool finalized)
    : reportId(id), reportType(type), reportDate(reportDate), periodStart(periodStart),
      periodEnd(periodEnd), totalRevenue(revenue), totalExpenses(expenses),
      netIncome(netIncome), payments(payments), invoices(invoices),
      currency(currency), isFinalized(finalized) {}

FinancialReport::FinancialReport(const FinancialReport& other)
    : reportId(other.reportId), reportType(other.reportType), reportDate(other.reportDate),
      periodStart(other.periodStart), periodEnd(other.periodEnd),
      totalRevenue(other.totalRevenue), totalExpenses(other.totalExpenses),
      netIncome(other.netIncome), payments(other.payments), invoices(other.invoices),
      currency(other.currency), isFinalized(other.isFinalized) {}

FinancialReport& FinancialReport::operator=(const FinancialReport& other) {
    if (this != &other) {
        reportId = other.reportId;
        reportType = other.reportType;
        reportDate = other.reportDate;
        periodStart = other.periodStart;
        periodEnd = other.periodEnd;
        totalRevenue = other.totalRevenue;
        totalExpenses = other.totalExpenses;
        netIncome = other.netIncome;
        payments = other.payments;
        invoices = other.invoices;
        currency = other.currency;
        isFinalized = other.isFinalized;
    }
    return *this;
}

FinancialReport::FinancialReport(FinancialReport&& other) noexcept
    : reportId(std::move(other.reportId)), reportType(std::move(other.reportType)),
      reportDate(other.reportDate), periodStart(other.periodStart), periodEnd(other.periodEnd),
      totalRevenue(other.totalRevenue), totalExpenses(other.totalExpenses),
      netIncome(other.netIncome), payments(std::move(other.payments)),
      invoices(std::move(other.invoices)), currency(std::move(other.currency)),
      isFinalized(other.isFinalized) {}

FinancialReport& FinancialReport::operator=(FinancialReport&& other) noexcept {
    if (this != &other) {
        reportId = std::move(other.reportId);
        reportType = std::move(other.reportType);
        reportDate = other.reportDate;
        periodStart = other.periodStart;
        periodEnd = other.periodEnd;
        totalRevenue = other.totalRevenue;
        totalExpenses = other.totalExpenses;
        netIncome = other.netIncome;
        payments = std::move(other.payments);
        invoices = std::move(other.invoices);
        currency = std::move(other.currency);
        isFinalized = other.isFinalized;
    }
    return *this;
}

FinancialReport::~FinancialReport() {}

std::string FinancialReport::getReportId() const { return reportId; }
std::string FinancialReport::getReportType() const { return reportType; }
std::chrono::system_clock::time_point FinancialReport::getReportDate() const { return reportDate; }
std::chrono::system_clock::time_point FinancialReport::getPeriodStart() const { return periodStart; }
std::chrono::system_clock::time_point FinancialReport::getPeriodEnd() const { return periodEnd; }
double FinancialReport::getTotalRevenue() const { return totalRevenue; }
double FinancialReport::getTotalExpenses() const { return totalExpenses; }
double FinancialReport::getNetIncome() const { return netIncome; }
std::vector<std::shared_ptr<Payment>> FinancialReport::getPayments() const { return payments; }
std::vector<std::shared_ptr<Invoice>> FinancialReport::getInvoices() const { return invoices; }
std::string FinancialReport::getCurrency() const { return currency; }
bool FinancialReport::getIsFinalized() const { return isFinalized; }

void FinancialReport::setTotalRevenue(double newRevenue) {
    totalRevenue = std::max(0.0, newRevenue);
    netIncome = totalRevenue - totalExpenses;
}

void FinancialReport::setTotalExpenses(double newExpenses) {
    totalExpenses = std::max(0.0, newExpenses);
    netIncome = totalRevenue - totalExpenses;
}

void FinancialReport::setIsFinalized(bool finalized) {
    isFinalized = finalized;
}

double FinancialReport::calculateProfitMargin() const {
    if (totalRevenue <= 0) return 0.0;
    return (netIncome / totalRevenue) * 100.0;
}

bool FinancialReport::isQuarterlyReport() const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(periodEnd - periodStart);
    int days = static_cast<int>(duration.count() / 24.0);
    return days >= 85 && days <= 95;
}

bool FinancialReport::isAnnualReport() const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(periodEnd - periodStart);
    int days = static_cast<int>(duration.count() / 24.0);
    return days >= 360 && days <= 370;
}

double FinancialReport::calculateExpenseRatio() const {
    if (totalRevenue <= 0) return 0.0;
    return (totalExpenses / totalRevenue) * 100.0;
}

bool FinancialReport::hasPositiveCashFlow() const {
    return netIncome > 0;
}

double FinancialReport::calculateAverageTransaction() const {
    int totalTransactions = payments.size() + invoices.size();
    if (totalTransactions == 0) return 0.0;
    return totalRevenue / totalTransactions;
}

std::string FinancialReport::getFinancialHealth() const {
    double profitMargin = calculateProfitMargin();
    
    if (profitMargin > 20.0) return "excellent";
    if (profitMargin > 10.0) return "good";
    if (profitMargin > 0.0) return "fair";
    if (profitMargin == 0.0) return "break_even";
    return "concerning";
}

double FinancialReport::calculateTaxLiability(double taxRate) const {
    return netIncome > 0 ? netIncome * (taxRate / 100.0) : 0.0;
}

bool FinancialReport::requiresAudit() const {
    return totalRevenue > 1000000.0 || isAnnualReport();
}