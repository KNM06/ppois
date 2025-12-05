#include "../../include/finance/FinancialAnalysis.h"
#include <algorithm>
#include <cmath>

FinancialAnalysis::FinancialAnalysis(const std::string& period, double targetROI, double riskTolerance)
    : analysisPeriod(period), targetROI(targetROI), riskTolerance(riskTolerance) {}

FinancialAnalysis::FinancialAnalysis(const FinancialAnalysis& other)
    : financialReports(other.financialReports), properties(other.properties),
      analysisPeriod(other.analysisPeriod), targetROI(other.targetROI),
      riskTolerance(other.riskTolerance) {}

FinancialAnalysis& FinancialAnalysis::operator=(const FinancialAnalysis& other) {
    if (this != &other) {
        financialReports = other.financialReports;
        properties = other.properties;
        analysisPeriod = other.analysisPeriod;
        targetROI = other.targetROI;
        riskTolerance = other.riskTolerance;
    }
    return *this;
}

FinancialAnalysis::FinancialAnalysis(FinancialAnalysis&& other) noexcept
    : financialReports(std::move(other.financialReports)), properties(std::move(other.properties)),
      analysisPeriod(std::move(other.analysisPeriod)), targetROI(other.targetROI),
      riskTolerance(other.riskTolerance) {}

FinancialAnalysis& FinancialAnalysis::operator=(FinancialAnalysis&& other) noexcept {
    if (this != &other) {
        financialReports = std::move(other.financialReports);
        properties = std::move(other.properties);
        analysisPeriod = std::move(other.analysisPeriod);
        targetROI = other.targetROI;
        riskTolerance = other.riskTolerance;
    }
    return *this;
}

FinancialAnalysis::~FinancialAnalysis() {}

std::string FinancialAnalysis::getAnalysisPeriod() const { return analysisPeriod; }
double FinancialAnalysis::getTargetROI() const { return targetROI; }
double FinancialAnalysis::getRiskTolerance() const { return riskTolerance; }

void FinancialAnalysis::setTargetROI(double newTarget) {
    targetROI = std::max(0.0, newTarget);
}

void FinancialAnalysis::setRiskTolerance(double newTolerance) {
    riskTolerance = std::max(0.0, std::min(1.0, newTolerance));
}

double FinancialAnalysis::calculatePortfolioROI() const {
    double totalInvestment = 0.0;
    double totalReturn = 0.0;
    
    for (const auto& report : financialReports) {
        totalReturn += report->getNetIncome();
    }
    
    for (const auto& property : properties) {
        totalInvestment += property->getRentalPrice() * 100;
    }
    
    if (totalInvestment <= 0) return 0.0;
    return (totalReturn / totalInvestment) * 100.0;
}

double FinancialAnalysis::calculateCashFlowAnalysis() const {
    double totalCashFlow = 0.0;
    
    for (const auto& report : financialReports) {
        totalCashFlow += report->getNetIncome();
    }
    
    return totalCashFlow;
}

double FinancialAnalysis::calculateNetOperatingIncome() const {
    double totalNOI = 0.0;
    
    for (const auto& report : financialReports) {
        totalNOI += report->getTotalRevenue() - report->getTotalExpenses();
    }
    
    return totalNOI;
}

double FinancialAnalysis::calculateCapitalizationRate(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    
    double propertyValue = property->getRentalPrice() * 100;
    double annualRent = property->getRentalPrice() * 12;
    
    if (propertyValue <= 0) return 0.0;
    return (annualRent / propertyValue) * 100.0;
}

std::vector<std::shared_ptr<Property>> FinancialAnalysis::getHighPerformingProperties() const {
    std::vector<std::shared_ptr<Property>> highPerformers;
    
    for (const auto& property : properties) {
        double capRate = calculateCapitalizationRate(property);
        if (capRate >= targetROI) {
            highPerformers.push_back(property);
        }
    }
    
    return highPerformers;
}

std::vector<std::shared_ptr<Property>> FinancialAnalysis::getUnderperformingProperties() const {
    std::vector<std::shared_ptr<Property>> underperformers;
    
    for (const auto& property : properties) {
        double capRate = calculateCapitalizationRate(property);
        if (capRate < targetROI * 0.7) {
            underperformers.push_back(property);
        }
    }
    
    return underperformers;
}

double FinancialAnalysis::calculateRiskAdjustedReturn() const {
    double portfolioROI = calculatePortfolioROI();
    return portfolioROI * (1.0 - riskTolerance);
}

std::string FinancialAnalysis::getInvestmentRecommendation(std::shared_ptr<Property> property) const {
    if (!property) return "no_data";
    
    double capRate = calculateCapitalizationRate(property);
    
    if (capRate >= targetROI * 1.2) return "strong_buy";
    if (capRate >= targetROI) return "buy";
    if (capRate >= targetROI * 0.8) return "hold";
    return "sell";
}

double FinancialAnalysis::calculateBreakEvenAnalysis(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    
    double monthlyRent = property->getRentalPrice();
    double annualExpenses = monthlyRent * 12 * 0.3;
    double propertyValue = property->getRentalPrice() * 100;
    
    if (monthlyRent <= 0) return 0.0;
    return annualExpenses / monthlyRent;
}

bool FinancialAnalysis::meetsInvestmentCriteria(std::shared_ptr<Property> property) const {
    if (!property) return false;
    
    double capRate = calculateCapitalizationRate(property);
    double breakEven = calculateBreakEvenAnalysis(property);
    
    return capRate >= targetROI && breakEven <= 12.0;
}