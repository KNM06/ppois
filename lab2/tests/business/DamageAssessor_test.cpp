#include <gtest/gtest.h>
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/InsuranceCalculator.h"

class DamageAssessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        damageAssessor = std::make_shared<DamageAssessor>();
    }
    
    std::shared_ptr<DamageAssessor> damageAssessor;
};

TEST_F(DamageAssessorTest, AssessDamageCostForVehicleMinorScratch) {
    double cost = damageAssessor->assessDamageCost("VEHICLE", 10000.0, "MINOR_SCRATCH", "MINOR");
    EXPECT_GT(cost, 0.0);
    EXPECT_LT(cost, 10000.0 * 0.8);
}

TEST_F(DamageAssessorTest, AssessDamageCostForElectronicsWaterDamage) {
    double cost = damageAssessor->assessDamageCost("ELECTRONICS", 2000.0, "WATER_DAMAGE", "SEVERE");
    EXPECT_GT(cost, 0.0);
    EXPECT_LT(cost, 2000.0 * 0.8);
}

TEST_F(DamageAssessorTest, AssessDamageCostForConstructionDamage) {
    double cost = damageAssessor->assessDamageCost("CONSTRUCTION", 5000.0, "CRACK", "MODERATE");
    EXPECT_GT(cost, 0.0);
    EXPECT_LT(cost, 5000.0 * 0.8);
}

TEST_F(DamageAssessorTest, AssessDamageCostForTotalLoss) {
    double cost = damageAssessor->assessDamageCost("VEHICLE", 10000.0, "THEFT", "TOTAL");
    EXPECT_EQ(cost, 10000.0);
}

TEST_F(DamageAssessorTest, IsDamageRepairable) {
    bool repairable1 = damageAssessor->isDamageRepairable("VEHICLE", "MINOR_SCRATCH");
    EXPECT_TRUE(repairable1);
    
    bool repairable2 = damageAssessor->isDamageRepairable("ELECTRONICS", "THEFT");
    EXPECT_FALSE(repairable2);
    
    bool repairable3 = damageAssessor->isDamageRepairable("ELECTRONICS", "WATER_DAMAGE");
    EXPECT_FALSE(repairable3);
}

TEST_F(DamageAssessorTest, CalculateDepreciationAfterDamage) {
    double depreciation = damageAssessor->calculateDepreciationAfterDamage(10000.0, 500.0, "MINOR_SCRATCH");
    EXPECT_GT(depreciation, 0.0);
    EXPECT_LT(depreciation, 10000.0 * 0.8);
}

TEST_F(DamageAssessorTest, DetermineDamageResponsibility) {
    std::string responsibility1 = damageAssessor->determineDamageResponsibility("GOOD", "NORMAL_WEAR");
    EXPECT_EQ(responsibility1, "COMPANY");
    
    std::string responsibility2 = damageAssessor->determineDamageResponsibility("NEGLIGENT", "CRACK");
    EXPECT_EQ(responsibility2, "CUSTOMER");
    
    std::string responsibility3 = damageAssessor->determineDamageResponsibility("GOOD", "ACCIDENT");
    EXPECT_EQ(responsibility3, "SHARED");
}

TEST_F(DamageAssessorTest, CalculateEnvironmentalCleanupCost) {
    double cost1 = damageAssessor->calculateEnvironmentalCleanupCost("VEHICLE", "OIL_LEAK");
    EXPECT_GT(cost1, 0.0);
    
    double cost2 = damageAssessor->calculateEnvironmentalCleanupCost("CONSTRUCTION", "CHEMICAL_SPILL");
    EXPECT_GT(cost2, 0.0);
}

TEST_F(DamageAssessorTest, RequiresProfessionalAssessment) {
    bool requires1 = damageAssessor->requiresProfessionalAssessment("VEHICLE", 600.0);
    EXPECT_TRUE(requires1);
    
    bool requires2 = damageAssessor->requiresProfessionalAssessment("LUXURY", 200.0);
    EXPECT_TRUE(requires2);
    
    bool requires3 = damageAssessor->requiresProfessionalAssessment("TOOLS", 150.0);
    EXPECT_FALSE(requires3);
}

TEST_F(DamageAssessorTest, GetRecommendedRepairVendors) {
    auto vendors1 = damageAssessor->getRecommendedRepairVendors("VEHICLE");
    EXPECT_FALSE(vendors1.empty());
    
    auto vendors2 = damageAssessor->getRecommendedRepairVendors("ELECTRONICS");
    EXPECT_FALSE(vendors2.empty());
    
    auto vendors3 = damageAssessor->getRecommendedRepairVendors("UNKNOWN");
    EXPECT_FALSE(vendors3.empty());
}

TEST_F(DamageAssessorTest, CalculateTotalLossThreshold) {
    double threshold1 = damageAssessor->calculateTotalLossThreshold("VEHICLE", 10000.0);
    EXPECT_EQ(threshold1, 10000.0 * 0.7);
    
    double threshold2 = damageAssessor->calculateTotalLossThreshold("ELECTRONICS", 2000.0);
    EXPECT_EQ(threshold2, 2000.0 * 0.6);
    
    double threshold3 = damageAssessor->calculateTotalLossThreshold("LUXURY", 5000.0);
    EXPECT_EQ(threshold3, 5000.0 * 0.5);
}

TEST_F(DamageAssessorTest, UpdateDamageRateMultiplier) {
    damageAssessor->updateDamageRateMultiplier("MINOR_SCRATCH", 0.05);
    
    double costBefore = damageAssessor->assessDamageCost("VEHICLE", 10000.0, "MINOR_SCRATCH", "MODERATE");
    
    damageAssessor->updateDamageRateMultiplier("MINOR_SCRATCH", 0.1);
    double costAfter = damageAssessor->assessDamageCost("VEHICLE", 10000.0, "MINOR_SCRATCH", "MODERATE");
    
    EXPECT_GT(costAfter, costBefore * 0.9);
}

TEST_F(DamageAssessorTest, AddRepairVendor) {
    damageAssessor->addRepairVendor("VEHICLE", "New Auto Repair");
    
    auto vendors = damageAssessor->getRecommendedRepairVendors("VEHICLE");
    bool found = false;
    for (const auto& vendor : vendors) {
        if (vendor == "New Auto Repair") {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(DamageAssessorTest, DamageCostForUnknownDamageType) {
    double cost = damageAssessor->assessDamageCost("VEHICLE", 10000.0, "UNKNOWN_DAMAGE", "MODERATE");
    EXPECT_GT(cost, 0.0);
}

TEST_F(DamageAssessorTest, CalculateDepreciationForSevereDamage) {
    double depreciation = damageAssessor->calculateDepreciationAfterDamage(10000.0, 3000.0, "STRUCTURAL_FAILURE");
    EXPECT_GT(depreciation, 0.0);
}

TEST_F(DamageAssessorTest, EnvironmentalCleanupForNoDamage) {
    double cost = damageAssessor->calculateEnvironmentalCleanupCost("VEHICLE", "MINOR_SCRATCH");
    EXPECT_EQ(cost, 0.0);
}

TEST_F(DamageAssessorTest, DamageAssessmentWithInsuranceCalculator) {
    auto insuranceCalculator = std::make_shared<InsuranceCalculator>();
    auto assessorWithInsurance = std::make_shared<DamageAssessor>(insuranceCalculator);
    
    double cost = assessorWithInsurance->assessDamageCost("VEHICLE", 10000.0, "MINOR_SCRATCH", "MINOR");
    EXPECT_GT(cost, 0.0);
}

TEST_F(DamageAssessorTest, RepairVendorsForNewCategory) {
    damageAssessor->addRepairVendor("NEW_CATEGORY", "Specialized Vendor");
    
    auto vendors = damageAssessor->getRecommendedRepairVendors("NEW_CATEGORY");
    EXPECT_FALSE(vendors.empty());
    EXPECT_EQ(vendors[0], "Specialized Vendor");
}