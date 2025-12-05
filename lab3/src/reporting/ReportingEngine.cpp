#include "../../include/reporting/ReportingEngine.h"
#include <chrono>
#include <algorithm>
#include <cmath>

ReportingEngine::ReportingEngine(const std::string& format, bool charts, int retentionMonths)
    : reportFormat(format), includeCharts(charts), dataRetentionMonths(retentionMonths) {}

ReportingEngine::ReportingEngine(const ReportingEngine& other)
    : properties(other.properties), contracts(other.contracts), payments(other.payments),
      reportFormat(other.reportFormat), includeCharts(other.includeCharts),
      dataRetentionMonths(other.dataRetentionMonths) {}

ReportingEngine& ReportingEngine::operator=(const ReportingEngine& other) {
    if (this != &other) {
        properties = other.properties;
        contracts = other.contracts;
        payments = other.payments;
        reportFormat = other.reportFormat;
        includeCharts = other.includeCharts;
        dataRetentionMonths = other.dataRetentionMonths;
    }
    return *this;
}

ReportingEngine::ReportingEngine(ReportingEngine&& other) noexcept
    : properties(std::move(other.properties)), contracts(std::move(other.contracts)),
      payments(std::move(other.payments)), reportFormat(std::move(other.reportFormat)),
      includeCharts(other.includeCharts), dataRetentionMonths(other.dataRetentionMonths) {}

ReportingEngine& ReportingEngine::operator=(ReportingEngine&& other) noexcept {
    if (this != &other) {
        properties = std::move(other.properties);
        contracts = std::move(other.contracts);
        payments = std::move(other.payments);
        reportFormat = std::move(other.reportFormat);
        includeCharts = other.includeCharts;
        dataRetentionMonths = other.dataRetentionMonths;
    }
    return *this;
}

ReportingEngine::~ReportingEngine() {}

std::string ReportingEngine::getReportFormat() const { return reportFormat; }
bool ReportingEngine::getIncludeCharts() const { return includeCharts; }
int ReportingEngine::getDataRetentionMonths() const { return dataRetentionMonths; }

void ReportingEngine::setReportFormat(const std::string& newFormat) {
    reportFormat = newFormat;
}

void ReportingEngine::setIncludeCharts(bool include) {
    includeCharts = include;
}

void ReportingEngine::setDataRetentionMonths(int months) {
    dataRetentionMonths = std::max(1, months);
}

double ReportingEngine::generateFinancialReport(const std::chrono::system_clock::time_point& startDate,
                                               const std::chrono::system_clock::time_point& endDate) const {
    double totalRevenue = 0.0;
    
    for (const auto& payment : payments) {
        if (payment->getPaymentDate() >= startDate && payment->getPaymentDate() <= endDate &&
            payment->getStatus() == "completed") {
            totalRevenue += payment->getAmount();
        }
    }
    
    return totalRevenue;
}

double ReportingEngine::calculateVacancyRate(const std::chrono::system_clock::time_point& reportDate) const {
    int totalProperties = properties.size();
    if (totalProperties == 0) return 0.0;
    
    int occupiedProperties = 0;
    for (const auto& contract : contracts) {
        if (contract->isContractValid() && contract->getStartDate() <= reportDate &&
            contract->getEndDate() >= reportDate) {
            occupiedProperties++;
        }
    }
    
    return ((totalProperties - occupiedProperties) / static_cast<double>(totalProperties)) * 100.0;
}

double ReportingEngine::calculateCollectionRate(const std::chrono::system_clock::time_point& startDate,
                                               const std::chrono::system_clock::time_point& endDate) const {
    int totalPayments = 0;
    int collectedPayments = 0;
    
    for (const auto& payment : payments) {
        if (payment->getPaymentDate() >= startDate && payment->getPaymentDate() <= endDate) {
            totalPayments++;
            if (payment->getStatus() == "completed") {
                collectedPayments++;
            }
        }
    }
    
    if (totalPayments == 0) return 100.0;
    return (static_cast<double>(collectedPayments) / totalPayments) * 100.0;
}

std::vector<std::shared_ptr<Property>> ReportingEngine::getTopPerformingProperties(int limit) const {
    std::vector<std::pair<std::shared_ptr<Property>, double>> propertyPerformance;
    
    for (const auto& property : properties) {
        double performance = 0.0;
        for (const auto& contract : contracts) {
            if (contract->getProperty()->getId() == property->getId() && contract->isContractValid()) {
                performance += contract->getMonthlyRent();
            }
        }
        propertyPerformance.emplace_back(property, performance);
    }
    
    std::sort(propertyPerformance.begin(), propertyPerformance.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::vector<std::shared_ptr<Property>> topProperties;
    for (int i = 0; i < std::min(limit, static_cast<int>(propertyPerformance.size())); i++) {
        topProperties.push_back(propertyPerformance[i].first);
    }
    
    return topProperties;
}

double ReportingEngine::calculateMaintenanceCosts(const std::chrono::system_clock::time_point& startDate,
                                                 const std::chrono::system_clock::time_point& endDate) const {
    return 0.0;
}

std::string ReportingEngine::generatePortfolioSummary() const {
    int totalProperties = properties.size();
    int occupiedProperties = 0;
    double totalMonthlyRent = 0.0;
    
    for (const auto& contract : contracts) {
        if (contract->isContractValid()) {
            occupiedProperties++;
            totalMonthlyRent += contract->getMonthlyRent();
        }
    }
    
    double occupancyRate = (static_cast<double>(occupiedProperties) / totalProperties) * 100.0;
    
    return "Portfolio: " + std::to_string(totalProperties) + " properties, " +
           std::to_string(occupiedProperties) + " occupied (" +
           std::to_string(occupancyRate) + "%), Monthly Revenue: $" +
           std::to_string(totalMonthlyRent);
}

double ReportingEngine::calculateROI(const std::chrono::system_clock::time_point& startDate,
                                    const std::chrono::system_clock::time_point& endDate) const {
    double totalInvestment = 0.0;
    double totalReturn = generateFinancialReport(startDate, endDate);
    
    for (const auto& property : properties) {
        totalInvestment += property->getRentalPrice() * 100;
    }
    
    if (totalInvestment <= 0) return 0.0;
    return (totalReturn / totalInvestment) * 100.0;
}

std::vector<std::shared_ptr<RentalContract>> ReportingEngine::getExpiringContractsReport(int daysThreshold) const {
    std::vector<std::shared_ptr<RentalContract>> expiringContracts;
    auto now = std::chrono::system_clock::now();
    
    for (const auto& contract : contracts) {
        if (contract->isContractValid() && contract->getRemainingDays() <= daysThreshold) {
            expiringContracts.push_back(contract);
        }
    }
    
    return expiringContracts;
}

double ReportingEngine::calculateAverageRentByLocation(const std::string& location) const {
    double totalRent = 0.0;
    int count = 0;
    
    for (const auto& contract : contracts) {
        if (contract->isContractValid() && contract->getProperty()->getAddress() &&
            contract->getProperty()->getAddress()->getCity().find(location) != std::string::npos) {
            totalRent += contract->getMonthlyRent();
            count++;
        }
    }
    
    return count > 0 ? totalRent / count : 0.0;
}

std::string ReportingEngine::generateTaxReport(int fiscalYear) const {
    double totalIncome = generateFinancialReport(
        std::chrono::system_clock::time_point(),
        std::chrono::system_clock::time_point()
    );
    
    return "Tax Report FY" + std::to_string(fiscalYear) + ": Total Income: $" +
           std::to_string(totalIncome) + ", Estimated Tax: $" +
           std::to_string(totalIncome * 0.25);
}