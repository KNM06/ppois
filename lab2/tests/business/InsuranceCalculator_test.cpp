#include <gtest/gtest.h>
#include "../../include/business/InsuranceCalculator.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class InsuranceCalculatorTest : public ::testing::Test {
protected:
    std::shared_ptr<InsuranceCalculator> insuranceCalculator;
    std::shared_ptr<DamageAssessor> damageAssessor;

    void SetUp() override {
        insuranceCalculator = std::make_shared<InsuranceCalculator>();
        damageAssessor = std::make_shared<DamageAssessor>(insuranceCalculator);
        insuranceCalculator->setDamageAssessor(damageAssessor);
    }
};

TEST_F(InsuranceCalculatorTest, CalculateInsurancePremiumVehicleNew) {
    double premium = insuranceCalculator->calculateInsurancePremium("VEHICLE", 10000.0, "NEW", 7);
    EXPECT_NEAR(premium, 838.35, 0.001);
}

TEST_F(InsuranceCalculatorTest, CalculateInsurancePremiumElectronicsRegular) {
    double premium = insuranceCalculator->calculateInsurancePremium("ELECTRONICS", 2000.0, "REGULAR", 1);
    EXPECT_NEAR(premium, 78.39, 0.001);
}

TEST_F(InsuranceCalculatorTest, CalculateInsurancePremiumLuxuryVIP) {
    double premium = insuranceCalculator->calculateInsurancePremium("LUXURY", 5000.0, "VIP", 10);
    EXPECT_NEAR(premium, 315.0, 0.001);
}

TEST_F(InsuranceCalculatorTest, CalculateInsurancePremiumClampedMin) {
    double premium = insuranceCalculator->calculateInsurancePremium("TOOLS", 10.0, "REGULAR", 1);
    EXPECT_DOUBLE_EQ(premium, 10.0);
}

TEST_F(InsuranceCalculatorTest, CalculateDamageDeductibleVehicle) {
    double deductible = insuranceCalculator->calculateDamageDeductible("VEHICLE", 10000.0);
    EXPECT_DOUBLE_EQ(deductible, 1000.0);
}

TEST_F(InsuranceCalculatorTest, CalculateDamageDeductibleElectronics) {
    double deductible = insuranceCalculator->calculateDamageDeductible("ELECTRONICS", 1000.0);
    EXPECT_DOUBLE_EQ(deductible, 120.0);
}

TEST_F(InsuranceCalculatorTest, CalculateDamageDeductibleTools) {
    double deductible = insuranceCalculator->calculateDamageDeductible("TOOLS", 100.0);
    EXPECT_DOUBLE_EQ(deductible, 50.0);
}

TEST_F(InsuranceCalculatorTest, RequiresAdditionalCoverageTrueValue) {
    EXPECT_TRUE(insuranceCalculator->requiresAdditionalCoverage("TOOLS", 5001.0));
}

TEST_F(InsuranceCalculatorTest, RequiresAdditionalCoverageTrueCategory) {
    EXPECT_TRUE(insuranceCalculator->requiresAdditionalCoverage("CONSTRUCTION", 100.0));
}

TEST_F(InsuranceCalculatorTest, RequiresAdditionalCoverageTrueLuxury) {
    EXPECT_TRUE(insuranceCalculator->requiresAdditionalCoverage("LUXURY", 2001.0));
}

TEST_F(InsuranceCalculatorTest, RequiresAdditionalCoverageFalse) {
    EXPECT_FALSE(insuranceCalculator->requiresAdditionalCoverage("TOOLS", 4000.0));
}

TEST_F(InsuranceCalculatorTest, CalculateTheftProtectionPremiumVehicle) {
    double premium = insuranceCalculator->calculateTheftProtectionPremium("VEHICLE", 10000.0);
    EXPECT_DOUBLE_EQ(premium, 300.0);
}

TEST_F(InsuranceCalculatorTest, CalculateTheftProtectionPremiumLow) {
    double premium = insuranceCalculator->calculateTheftProtectionPremium("TOOLS", 100.0);
    EXPECT_DOUBLE_EQ(premium, 15.0);
}

TEST_F(InsuranceCalculatorTest, CalculateLiabilityCoverageConstructionBusiness) {
    double coverage = insuranceCalculator->calculateLiabilityCoverage("CONSTRUCTION", "BUSINESS");
    EXPECT_DOUBLE_EQ(coverage, 600000.0);
}

TEST_F(InsuranceCalculatorTest, CalculateLiabilityCoverageToolsNew) {
    double coverage = insuranceCalculator->calculateLiabilityCoverage("TOOLS", "NEW");
    EXPECT_DOUBLE_EQ(coverage, 120000.0);
}

TEST_F(InsuranceCalculatorTest, ValidateInsuranceClaimValidAccidental) {
    EXPECT_TRUE(insuranceCalculator->validateInsuranceClaim(800.0, 1000.0, "ACCIDENTAL"));
}

TEST_F(InsuranceCalculatorTest, ValidateInsuranceClaimInvalidAccidental) {
    EXPECT_FALSE(insuranceCalculator->validateInsuranceClaim(801.0, 1000.0, "ACCIDENTAL"));
}

TEST_F(InsuranceCalculatorTest, ValidateInsuranceClaimValidTheft) {
    EXPECT_TRUE(insuranceCalculator->validateInsuranceClaim(1000.0, 1000.0, "THEFT"));
}

TEST_F(InsuranceCalculatorTest, ValidateInsuranceClaimInvalidWear) {
    EXPECT_FALSE(insuranceCalculator->validateInsuranceClaim(40.0, 1000.0, "NORMAL_WEAR"));
}

TEST_F(InsuranceCalculatorTest, ValidateInsuranceClaimInvalidAmount) {
    EXPECT_FALSE(insuranceCalculator->validateInsuranceClaim(1100.0, 1000.0, "THEFT"));
}

TEST_F(InsuranceCalculatorTest, CalculateLocationRiskPremiumHighCrime) {
    double premium = insuranceCalculator->calculateLocationRiskPremium("VEHICLE", "HIGH_CRIME");
    EXPECT_NEAR(premium, 81.405, 0.001);
}

TEST_F(InsuranceCalculatorTest, GetRiskAssessmentContainsKeys) {
    auto risk = insuranceCalculator->getRiskAssessment("VEHICLE");
    EXPECT_TRUE(risk.count("Base Risk"));
    EXPECT_TRUE(risk.count("Category Risk"));
    EXPECT_TRUE(risk.count("Theft Risk"));
    EXPECT_TRUE(risk.count("Damage Risk"));
}

TEST_F(InsuranceCalculatorTest, UpdateCategoryRiskFactorClamps) {
    insuranceCalculator->updateCategoryRiskFactor("NEW_CAT", 6.0);
    auto risk = insuranceCalculator->getRiskAssessment("NEW_CAT");
    EXPECT_DOUBLE_EQ(risk["Category Risk"], 5.0);
}

TEST_F(InsuranceCalculatorTest, SettersAndGetters) {
    auto newAssessor = std::make_shared<DamageAssessor>();
    insuranceCalculator->setDamageAssessor(newAssessor);
    EXPECT_EQ(insuranceCalculator->getDamageAssessor(), newAssessor);
    
    insuranceCalculator->setLocationRiskFactor("NEW_LOC", 0.1);
}