#ifndef FINANCIALANALYSIS_H
#define FINANCIALANALYSIS_H

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "../../include/finance/FinancialReport.h"
#include "../../include/core/Property.h"

class FinancialAnalysis {
private:
    std::string analysisPeriod;
    double targetROI;
    double riskTolerance;


public:
    FinancialAnalysis(const std::string& period, double targetROI, double riskTolerance);
    
    FinancialAnalysis(const FinancialAnalysis& other);
    FinancialAnalysis& operator=(const FinancialAnalysis& other);
    FinancialAnalysis(FinancialAnalysis&& other) noexcept;
    FinancialAnalysis& operator=(FinancialAnalysis&& other) noexcept;
    ~FinancialAnalysis();

    std::string getAnalysisPeriod() const;
    double getTargetROI() const;
    double getRiskTolerance() const;

    void setTargetROI(double newTarget);
    void setRiskTolerance(double newTolerance);

    double calculatePortfolioROI() const;
    double calculateCashFlowAnalysis() const;
    double calculateNetOperatingIncome() const;
    double calculateCapitalizationRate(std::shared_ptr<Property> property) const;
    std::vector<std::shared_ptr<Property>> getHighPerformingProperties() const;
    std::vector<std::shared_ptr<Property>> getUnderperformingProperties() const;
    double calculateRiskAdjustedReturn() const;
    std::string getInvestmentRecommendation(std::shared_ptr<Property> property) const;
    double calculateBreakEvenAnalysis(std::shared_ptr<Property> property) const;
    bool meetsInvestmentCriteria(std::shared_ptr<Property> property) const;
    std::vector<std::shared_ptr<FinancialReport>> financialReports;
    std::vector<std::shared_ptr<Property>> properties;
};

#endif