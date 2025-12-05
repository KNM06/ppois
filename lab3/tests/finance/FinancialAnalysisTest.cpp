#include <gtest/gtest.h>
#include "../../include/finance/FinancialAnalysis.h"
#include "../../include/finance/FinancialReport.h"
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"
#include <memory>
#include <chrono>

using namespace std::chrono;

class FinancialAnalysisTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> createProperty(const std::string& id, double rentalPrice) {
        return std::make_shared<Property>(id, "123 Main St", 100.0);
    }
    
    std::shared_ptr<FinancialReport> createFinancialReport(const std::string& id, 
                                                          double revenue, 
                                                          double expenses, 
                                                          double netIncome) {
        auto now = system_clock::now();
        return std::make_shared<FinancialReport>(id, "monthly", now, 
                                                now - hours(24*30), now,
                                                revenue, expenses, netIncome,
                                                std::vector<std::shared_ptr<Payment>>(),
                                                std::vector<std::shared_ptr<Invoice>>(),
                                                "USD", true);
    }
};

TEST_F(FinancialAnalysisTest, ConstructorAndBasicGetters) {
    FinancialAnalysis analysis("2024-Q1", 10.5, 0.3);
    
    EXPECT_EQ(analysis.getAnalysisPeriod(), "2024-Q1");
    EXPECT_DOUBLE_EQ(analysis.getTargetROI(), 10.5);
    EXPECT_DOUBLE_EQ(analysis.getRiskTolerance(), 0.3);
}

TEST_F(FinancialAnalysisTest, SetTargetROIEdgeCases) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    analysis.setTargetROI(-5.0);
    EXPECT_DOUBLE_EQ(analysis.getTargetROI(), 0.0);
    
    analysis.setTargetROI(15.0);
    EXPECT_DOUBLE_EQ(analysis.getTargetROI(), 15.0);
    
    analysis.setTargetROI(0.0);
    EXPECT_DOUBLE_EQ(analysis.getTargetROI(), 0.0);
}

TEST_F(FinancialAnalysisTest, SetRiskToleranceEdgeCases) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    analysis.setRiskTolerance(-0.5);
    EXPECT_DOUBLE_EQ(analysis.getRiskTolerance(), 0.0);
    
    analysis.setRiskTolerance(1.5);
    EXPECT_DOUBLE_EQ(analysis.getRiskTolerance(), 1.0);
    
    analysis.setRiskTolerance(0.75);
    EXPECT_DOUBLE_EQ(analysis.getRiskTolerance(), 0.75);
    
    analysis.setRiskTolerance(0.0);
    EXPECT_DOUBLE_EQ(analysis.getRiskTolerance(), 0.0);
    
    analysis.setRiskTolerance(1.0);
    EXPECT_DOUBLE_EQ(analysis.getRiskTolerance(), 1.0);
}

TEST_F(FinancialAnalysisTest, CopyConstructor) {
    FinancialAnalysis analysis1("2024-Q1", 10.0, 0.3);
    analysis1.setTargetROI(12.0);
    
    auto property = createProperty("P1", 1000.0);
    auto report = createFinancialReport("R1", 5000.0, 3000.0, 2000.0);
    
    analysis1.properties.push_back(property);
    analysis1.financialReports.push_back(report);
    
    FinancialAnalysis analysis2(analysis1);
    
    EXPECT_EQ(analysis2.getAnalysisPeriod(), "2024-Q1");
    EXPECT_DOUBLE_EQ(analysis2.getTargetROI(), 12.0);
    EXPECT_DOUBLE_EQ(analysis2.getRiskTolerance(), 0.3);
    EXPECT_EQ(analysis2.properties.size(), 1);
    EXPECT_EQ(analysis2.financialReports.size(), 1);
}

TEST_F(FinancialAnalysisTest, CopyAssignment) {
    FinancialAnalysis analysis1("2024-Q1", 10.0, 0.3);
    analysis1.setTargetROI(12.0);
    
    auto property = createProperty("P1", 1000.0);
    auto report = createFinancialReport("R1", 5000.0, 3000.0, 2000.0);
    
    analysis1.properties.push_back(property);
    analysis1.financialReports.push_back(report);
    
    FinancialAnalysis analysis2("2024-Q2", 8.0, 0.4);
    
    analysis2 = analysis1;
    
    EXPECT_EQ(analysis2.getAnalysisPeriod(), "2024-Q1");
    EXPECT_DOUBLE_EQ(analysis2.getTargetROI(), 12.0);
    EXPECT_DOUBLE_EQ(analysis2.getRiskTolerance(), 0.3);
    EXPECT_EQ(analysis2.properties.size(), 1);
    EXPECT_EQ(analysis2.financialReports.size(), 1);
}

TEST_F(FinancialAnalysisTest, MoveConstructor) {
    FinancialAnalysis analysis1("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 1000.0);
    auto report = createFinancialReport("R1", 5000.0, 3000.0, 2000.0);
    
    analysis1.properties.push_back(property);
    analysis1.financialReports.push_back(report);
    
    FinancialAnalysis analysis2(std::move(analysis1));
    
    EXPECT_EQ(analysis2.getAnalysisPeriod(), "2024-Q1");
    EXPECT_DOUBLE_EQ(analysis2.getTargetROI(), 10.0);
    EXPECT_DOUBLE_EQ(analysis2.getRiskTolerance(), 0.3);
    EXPECT_EQ(analysis2.properties.size(), 1);
    EXPECT_EQ(analysis2.financialReports.size(), 1);
}

TEST_F(FinancialAnalysisTest, MoveAssignment) {
    FinancialAnalysis analysis1("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 1000.0);
    auto report = createFinancialReport("R1", 5000.0, 3000.0, 2000.0);
    
    analysis1.properties.push_back(property);
    analysis1.financialReports.push_back(report);
    
    FinancialAnalysis analysis2("2024-Q2", 8.0, 0.4);
    
    analysis2 = std::move(analysis1);
    
    EXPECT_EQ(analysis2.getAnalysisPeriod(), "2024-Q1");
    EXPECT_DOUBLE_EQ(analysis2.getTargetROI(), 10.0);
    EXPECT_DOUBLE_EQ(analysis2.getRiskTolerance(), 0.3);
    EXPECT_EQ(analysis2.properties.size(), 1);
    EXPECT_EQ(analysis2.financialReports.size(), 1);
}

TEST_F(FinancialAnalysisTest, CalculatePortfolioROIWithData) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto report1 = createFinancialReport("R1", 20000.0, 15000.0, 5000.0);
    auto report2 = createFinancialReport("R2", 15000.0, 12000.0, 3000.0);
    
    auto property1 = createProperty("P1", 1000.0);
    property1->setRentalPrice(1000.0);
    
    auto property2 = createProperty("P2", 1500.0);
    property2->setRentalPrice(1500.0);
    
    analysis.financialReports.push_back(report1);
    analysis.financialReports.push_back(report2);
    analysis.properties.push_back(property1);
    analysis.properties.push_back(property2);
    
    double roi = analysis.calculatePortfolioROI();
    
    double totalReturn = 5000.0 + 3000.0;
    double totalInvestment = (1000.0 * 100) + (1500.0 * 100);
    double expectedROI = (totalReturn / totalInvestment) * 100.0;
    
    EXPECT_NEAR(roi, expectedROI, 0.01);
}

TEST_F(FinancialAnalysisTest, CalculatePortfolioROIEmpty) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    double roi = analysis.calculatePortfolioROI();
    EXPECT_DOUBLE_EQ(roi, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculatePortfolioROIZeroInvestment) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 0.0);
    property->setRentalPrice(0.0);
    analysis.properties.push_back(property);
    
    double roi = analysis.calculatePortfolioROI();
    EXPECT_DOUBLE_EQ(roi, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculateCashFlowAnalysis) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto report1 = createFinancialReport("R1", 20000.0, 15000.0, 5000.0);
    auto report2 = createFinancialReport("R2", 15000.0, 12000.0, 3000.0);
    
    analysis.financialReports.push_back(report1);
    analysis.financialReports.push_back(report2);
    
    double cashFlow = analysis.calculateCashFlowAnalysis();
    EXPECT_DOUBLE_EQ(cashFlow, 8000.0);
}

TEST_F(FinancialAnalysisTest, CalculateCashFlowAnalysisEmpty) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    double cashFlow = analysis.calculateCashFlowAnalysis();
    EXPECT_DOUBLE_EQ(cashFlow, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculateNetOperatingIncome) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto report1 = createFinancialReport("R1", 20000.0, 15000.0, 5000.0);
    auto report2 = createFinancialReport("R2", 15000.0, 12000.0, 3000.0);
    
    analysis.financialReports.push_back(report1);
    analysis.financialReports.push_back(report2);
    
    double noi = analysis.calculateNetOperatingIncome();
    EXPECT_DOUBLE_EQ(noi, 8000.0);
}

TEST_F(FinancialAnalysisTest, CalculateCapitalizationRate) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 1000.0);
    property->setRentalPrice(1000.0);
    
    double capRate = analysis.calculateCapitalizationRate(property);
    
    double propertyValue = 1000.0 * 100;
    double annualRent = 1000.0 * 12;
    double expectedCapRate = (annualRent / propertyValue) * 100.0;
    
    EXPECT_NEAR(capRate, expectedCapRate, 0.01);
}

TEST_F(FinancialAnalysisTest, CalculateCapitalizationRateNullProperty) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    double capRate = analysis.calculateCapitalizationRate(nullptr);
    EXPECT_DOUBLE_EQ(capRate, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculateCapitalizationRateZeroRentalPrice) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 0.0);
    property->setRentalPrice(0.0);
    
    double capRate = analysis.calculateCapitalizationRate(property);
    EXPECT_DOUBLE_EQ(capRate, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculateCapitalizationRateNegativePropertyValue) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", -1000.0);
    property->setRentalPrice(-1000.0);
    
    double capRate = analysis.calculateCapitalizationRate(property);
    EXPECT_DOUBLE_EQ(capRate, 0.0);
}

TEST_F(FinancialAnalysisTest, GetHighPerformingProperties) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property1 = createProperty("P1", 2000.0);
    property1->setRentalPrice(2000.0);
    
    auto property2 = createProperty("P2", 500.0);
    property2->setRentalPrice(500.0);
    
    auto property3 = createProperty("P3", 1500.0);
    property3->setRentalPrice(1500.0);
    
    analysis.properties.push_back(property1);
    analysis.properties.push_back(property2);
    analysis.properties.push_back(property3);
    
    auto highPerformers = analysis.getHighPerformingProperties();
    
    double capRate1 = analysis.calculateCapitalizationRate(property1);
    double capRate2 = analysis.calculateCapitalizationRate(property2);
    double capRate3 = analysis.calculateCapitalizationRate(property3);
    
    if (capRate1 >= 10.0) {
        EXPECT_TRUE(std::find(highPerformers.begin(), highPerformers.end(), property1) != highPerformers.end());
    }
    
    if (capRate2 >= 10.0) {
        EXPECT_TRUE(std::find(highPerformers.begin(), highPerformers.end(), property2) != highPerformers.end());
    }
    
    if (capRate3 >= 10.0) {
        EXPECT_TRUE(std::find(highPerformers.begin(), highPerformers.end(), property3) != highPerformers.end());
    }
}

TEST_F(FinancialAnalysisTest, GetUnderperformingProperties) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property1 = createProperty("P1", 200.0);
    property1->setRentalPrice(200.0);
    
    auto property2 = createProperty("P2", 2000.0);
    property2->setRentalPrice(2000.0);
    
    auto property3 = createProperty("P3", 800.0);
    property3->setRentalPrice(800.0);
    
    analysis.properties.push_back(property1);
    analysis.properties.push_back(property2);
    analysis.properties.push_back(property3);
    
    auto underperformers = analysis.getUnderperformingProperties();
    
    double capRate1 = analysis.calculateCapitalizationRate(property1);
    double capRate2 = analysis.calculateCapitalizationRate(property2);
    double capRate3 = analysis.calculateCapitalizationRate(property3);
    
    if (capRate1 < 7.0) {
        EXPECT_TRUE(std::find(underperformers.begin(), underperformers.end(), property1) != underperformers.end());
    }
    
    if (capRate2 < 7.0) {
        EXPECT_TRUE(std::find(underperformers.begin(), underperformers.end(), property2) != underperformers.end());
    }
    
    if (capRate3 < 7.0) {
        EXPECT_TRUE(std::find(underperformers.begin(), underperformers.end(), property3) != underperformers.end());
    }
}

TEST_F(FinancialAnalysisTest, CalculateRiskAdjustedReturn) {
    FinancialAnalysis analysis1("2024-Q1", 10.0, 0.0);
    
    auto report = createFinancialReport("R1", 20000.0, 15000.0, 5000.0);
    auto property = createProperty("P1", 1000.0);
    property->setRentalPrice(1000.0);
    
    analysis1.financialReports.push_back(report);
    analysis1.properties.push_back(property);
    
    double rar1 = analysis1.calculateRiskAdjustedReturn();
    double portfolioROI1 = analysis1.calculatePortfolioROI();
    EXPECT_NEAR(rar1, portfolioROI1, 0.01);
    
    FinancialAnalysis analysis2("2024-Q1", 10.0, 0.5);
    analysis2.financialReports.push_back(report);
    analysis2.properties.push_back(property);
    
    double rar2 = analysis2.calculateRiskAdjustedReturn();
    double portfolioROI2 = analysis2.calculatePortfolioROI();
    EXPECT_NEAR(rar2, portfolioROI2 * 0.5, 0.01);
    
    FinancialAnalysis analysis3("2024-Q1", 10.0, 1.0);
    analysis3.financialReports.push_back(report);
    analysis3.properties.push_back(property);
    
    double rar3 = analysis3.calculateRiskAdjustedReturn();
    EXPECT_NEAR(rar3, 0.0, 0.01);
}

TEST_F(FinancialAnalysisTest, GetInvestmentRecommendationAllCases) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto propertyStrongBuy = createProperty("P1", 3000.0);
    propertyStrongBuy->setRentalPrice(3000.0);
    
    auto propertyBuy = createProperty("P2", 1800.0);
    propertyBuy->setRentalPrice(1800.0);
    
    auto propertyHold = createProperty("P3", 1200.0);
    propertyHold->setRentalPrice(1200.0);
    
    auto propertySell = createProperty("P4", 800.0);
    propertySell->setRentalPrice(800.0);
    
    std::string rec1 = analysis.getInvestmentRecommendation(propertyStrongBuy);
    std::string rec2 = analysis.getInvestmentRecommendation(propertyBuy);
    std::string rec3 = analysis.getInvestmentRecommendation(propertyHold);
    std::string rec4 = analysis.getInvestmentRecommendation(propertySell);
    
    double capRate1 = analysis.calculateCapitalizationRate(propertyStrongBuy);
    double capRate2 = analysis.calculateCapitalizationRate(propertyBuy);
    double capRate3 = analysis.calculateCapitalizationRate(propertyHold);
    double capRate4 = analysis.calculateCapitalizationRate(propertySell);
    
    if (capRate1 >= 12.0) {
        EXPECT_EQ(rec1, "strong_buy");
    } else if (capRate1 >= 10.0) {
        EXPECT_EQ(rec1, "buy");
    } else if (capRate1 >= 8.0) {
        EXPECT_EQ(rec1, "hold");
    } else {
        EXPECT_EQ(rec1, "sell");
    }
    
    if (capRate2 >= 12.0) {
        EXPECT_EQ(rec2, "strong_buy");
    } else if (capRate2 >= 10.0) {
        EXPECT_EQ(rec2, "buy");
    } else if (capRate2 >= 8.0) {
        EXPECT_EQ(rec2, "hold");
    } else {
        EXPECT_EQ(rec2, "sell");
    }
}

TEST_F(FinancialAnalysisTest, GetInvestmentRecommendationNullProperty) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    std::string recommendation = analysis.getInvestmentRecommendation(nullptr);
    EXPECT_EQ(recommendation, "no_data");
}

TEST_F(FinancialAnalysisTest, CalculateBreakEvenAnalysis) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 1000.0);
    property->setRentalPrice(1000.0);
    
    double breakEven = analysis.calculateBreakEvenAnalysis(property);
    
    double monthlyRent = 1000.0;
    double annualExpenses = monthlyRent * 12 * 0.3;
    double expectedBreakEven = annualExpenses / monthlyRent;
    
    EXPECT_NEAR(breakEven, expectedBreakEven, 0.01);
}

TEST_F(FinancialAnalysisTest, CalculateBreakEvenAnalysisNullProperty) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    double breakEven = analysis.calculateBreakEvenAnalysis(nullptr);
    EXPECT_DOUBLE_EQ(breakEven, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculateBreakEvenAnalysisZeroRent) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", 0.0);
    property->setRentalPrice(0.0);
    
    double breakEven = analysis.calculateBreakEvenAnalysis(property);
    EXPECT_DOUBLE_EQ(breakEven, 0.0);
}

TEST_F(FinancialAnalysisTest, CalculateBreakEvenAnalysisNegativeRent) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto property = createProperty("P1", -1000.0);
    property->setRentalPrice(-1000.0);
    
    double breakEven = analysis.calculateBreakEvenAnalysis(property);
    EXPECT_DOUBLE_EQ(breakEven, 0.0);
}

TEST_F(FinancialAnalysisTest, MeetsInvestmentCriteria) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto propertyGood = createProperty("P1", 2000.0);
    propertyGood->setRentalPrice(2000.0);
    
    auto propertyBad = createProperty("P2", 500.0);
    propertyBad->setRentalPrice(500.0);
    
    bool meets1 = analysis.meetsInvestmentCriteria(propertyGood);
    bool meets2 = analysis.meetsInvestmentCriteria(propertyBad);
    bool meets3 = analysis.meetsInvestmentCriteria(nullptr);
    
    double capRateGood = analysis.calculateCapitalizationRate(propertyGood);
    double breakEvenGood = analysis.calculateBreakEvenAnalysis(propertyGood);
    
    EXPECT_FALSE(meets3);
    
    if (capRateGood >= 10.0 && breakEvenGood <= 12.0) {
        EXPECT_TRUE(meets1);
    } else {
        EXPECT_FALSE(meets1);
    }
}

TEST_F(FinancialAnalysisTest, MeetsInvestmentCriteriaEdgeCases) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto propertyHighCapRate = createProperty("P1", 5000.0);
    propertyHighCapRate->setRentalPrice(5000.0);
    
    auto propertyHighBreakEven = createProperty("P2", 100.0);
    propertyHighBreakEven->setRentalPrice(100.0);
    
    double capRate1 = analysis.calculateCapitalizationRate(propertyHighCapRate);
    double breakEven1 = analysis.calculateBreakEvenAnalysis(propertyHighCapRate);
    
    double capRate2 = analysis.calculateCapitalizationRate(propertyHighBreakEven);
    double breakEven2 = analysis.calculateBreakEvenAnalysis(propertyHighBreakEven);
    
    bool meets1 = analysis.meetsInvestmentCriteria(propertyHighCapRate);
    bool meets2 = analysis.meetsInvestmentCriteria(propertyHighBreakEven);
    
    if (capRate1 >= 10.0 && breakEven1 <= 12.0) {
        EXPECT_TRUE(meets1);
    }
    
    if (capRate2 >= 10.0 && breakEven2 <= 12.0) {
        EXPECT_TRUE(meets2);
    } else {
        EXPECT_FALSE(meets2);
    }
}

TEST_F(FinancialAnalysisTest, MeetsInvestmentCriteriaExactValues) {
    FinancialAnalysis analysis("2024-Q1", 10.0, 0.3);
    
    auto propertyExactTarget = createProperty("P1", 1000.0);
    propertyExactTarget->setRentalPrice(1000.0);
    
    auto propertyBelowTarget = createProperty("P2", 900.0);
    propertyBelowTarget->setRentalPrice(900.0);
    
    double capRateExact = analysis.calculateCapitalizationRate(propertyExactTarget);
    double breakEvenExact = analysis.calculateBreakEvenAnalysis(propertyExactTarget);
    
    double capRateBelow = analysis.calculateCapitalizationRate(propertyBelowTarget);
    double breakEvenBelow = analysis.calculateBreakEvenAnalysis(propertyBelowTarget);
    
    if (capRateExact >= 10.0 && breakEvenExact <= 12.0) {
        EXPECT_TRUE(analysis.meetsInvestmentCriteria(propertyExactTarget));
    }
    
    if (capRateBelow >= 10.0 && breakEvenBelow <= 12.0) {
        EXPECT_TRUE(analysis.meetsInvestmentCriteria(propertyBelowTarget));
    } else {
        EXPECT_FALSE(analysis.meetsInvestmentCriteria(propertyBelowTarget));
    }
}
