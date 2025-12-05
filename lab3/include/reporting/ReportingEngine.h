#ifndef REPORTINGENGINE_H
#define REPORTINGENGINE_H

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/finance/Payment.h"

class ReportingEngine {
private:
    std::vector<std::shared_ptr<Property>> properties;
    std::vector<std::shared_ptr<RentalContract>> contracts;
    std::vector<std::shared_ptr<Payment>> payments;
    std::string reportFormat;
    bool includeCharts;
    int dataRetentionMonths;

public:
    ReportingEngine(const std::string& format, bool charts, int retentionMonths);
    
    ReportingEngine(const ReportingEngine& other);
    ReportingEngine& operator=(const ReportingEngine& other);
    ReportingEngine(ReportingEngine&& other) noexcept;
    ReportingEngine& operator=(ReportingEngine&& other) noexcept;
    ~ReportingEngine();

    std::string getReportFormat() const;
    bool getIncludeCharts() const;
    int getDataRetentionMonths() const;

    void setReportFormat(const std::string& newFormat);
    void setIncludeCharts(bool include);
    void setDataRetentionMonths(int months);

    double generateFinancialReport(const std::chrono::system_clock::time_point& startDate,
                                  const std::chrono::system_clock::time_point& endDate) const;
    double calculateVacancyRate(const std::chrono::system_clock::time_point& reportDate) const;
    double calculateCollectionRate(const std::chrono::system_clock::time_point& startDate,
                                  const std::chrono::system_clock::time_point& endDate) const;
    std::vector<std::shared_ptr<Property>> getTopPerformingProperties(int limit) const;
    double calculateMaintenanceCosts(const std::chrono::system_clock::time_point& startDate,
                                    const std::chrono::system_clock::time_point& endDate) const;
    std::string generatePortfolioSummary() const;
    double calculateROI(const std::chrono::system_clock::time_point& startDate,
                       const std::chrono::system_clock::time_point& endDate) const;
    std::vector<std::shared_ptr<RentalContract>> getExpiringContractsReport(int daysThreshold) const;
    double calculateAverageRentByLocation(const std::string& location) const;
    std::string generateTaxReport(int fiscalYear) const;
};

#endif