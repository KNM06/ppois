#include <gtest/gtest.h>
#include "../../include/items/ConstructionEquipment.h"
#include "../../include/core/RentalPricing.h"
#include "../../include/core/RentalItem.h"
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

class ConstructionEquipmentTest : public ::testing::Test {
protected:
    std::shared_ptr<ConstructionEquipment> heavyEquipment;
    std::shared_ptr<ConstructionEquipment> lightEquipment;
    std::shared_ptr<ConstructionEquipment> craneEquipment;
    std::shared_ptr<ConstructionEquipment> highWeightEquipment;

    void SetUp() override {
        heavyEquipment = std::make_shared<ConstructionEquipment>("CONST001", "Excavator", "HEAVY", 5000.0);
        craneEquipment = std::make_shared<ConstructionEquipment>("CONST002", "Mobile Crane", "CRANE", 15000.0);
        lightEquipment = std::make_shared<ConstructionEquipment>("CONST003", "Cement Mixer", "MIXER", 999.0);
        highWeightEquipment = std::make_shared<ConstructionEquipment>("CONST004", "Heavy Loader", "LOADER", 4000.0);
        
        heavyEquipment->updateMaintenanceStatus(300);
    }
};

TEST_F(ConstructionEquipmentTest, ConstructorInitializationHeavy) {
    EXPECT_EQ(heavyEquipment->getItemId(), "CONST001");
    EXPECT_EQ(heavyEquipment->getName(), "Excavator");
    EXPECT_EQ(heavyEquipment->getEquipmentClass(), "HEAVY");
    EXPECT_EQ(heavyEquipment->getWeight(), 5000.0);
    EXPECT_TRUE(heavyEquipment->getRequiresSpecialLicense());
    EXPECT_TRUE(heavyEquipment->requiresCertifiedOperator());
}

TEST_F(ConstructionEquipmentTest, ConstructorInitializationLight) {
    EXPECT_EQ(lightEquipment->getEquipmentClass(), "MIXER");
    EXPECT_FALSE(lightEquipment->getRequiresSpecialLicense());
    EXPECT_FALSE(lightEquipment->requiresCertifiedOperator());
}

TEST_F(ConstructionEquipmentTest, CalculateTransportationCostLightWeight) {
    double cost = lightEquipment->calculateTransportationCost(100.0);
    EXPECT_NEAR(cost, 130.0, 0.001);
}

TEST_F(ConstructionEquipmentTest, CalculateTransportationCostMediumWeight) {
    double cost = highWeightEquipment->calculateTransportationCost(100.0);
    EXPECT_NEAR(cost, 410.0, 0.001);
}

TEST_F(ConstructionEquipmentTest, CalculateTransportationCostHeavyWeight) {
    double cost = craneEquipment->calculateTransportationCost(100.0);
    EXPECT_NEAR(cost, 490.0, 0.001);
}

TEST_F(ConstructionEquipmentTest, ValidateOperatorLicenseReturnsTrueForCorrectLicense) {
    EXPECT_TRUE(heavyEquipment->validateOperatorLicense("HEAVY_EQUIPMENT"));
}

TEST_F(ConstructionEquipmentTest, ValidateOperatorLicenseReturnsFalseForWrongLicense) {
    EXPECT_FALSE(heavyEquipment->validateOperatorLicense("CRANE_OPERATOR"));
}

TEST_F(ConstructionEquipmentTest, ValidateOperatorLicenseReturnsTrueIfNoSpecialLicenseRequired) {
    EXPECT_TRUE(lightEquipment->validateOperatorLicense("ANY_LICENSE"));
}

TEST_F(ConstructionEquipmentTest, CalculateFuelConsumptionDiesel) {
    double consumption = heavyEquipment->calculateFuelConsumption(8);
    EXPECT_DOUBLE_EQ(consumption, 36.0);
}

TEST_F(ConstructionEquipmentTest, GetSafetyInspectionIntervalForCRANE) {
    EXPECT_EQ(craneEquipment->getSafetyInspectionInterval(), 30);
}

TEST_F(ConstructionEquipmentTest, GetSafetyInspectionIntervalForHEAVY) {
    EXPECT_EQ(heavyEquipment->getSafetyInspectionInterval(), 42);
}

TEST_F(ConstructionEquipmentTest, GetSafetyInspectionIntervalWithHighUsage) {
    lightEquipment->updateMaintenanceStatus(700);
    EXPECT_EQ(lightEquipment->getSafetyInspectionInterval(), 62);
}

TEST_F(ConstructionEquipmentTest, GetSafetyInspectionIntervalClampsToMinimum) {
    auto pressureEquipment = std::make_shared<ConstructionEquipment>("PRESS01", "Compressor", "PRESSURE", 100.0);
    pressureEquipment->updateMaintenanceStatus(1000);
    EXPECT_EQ(pressureEquipment->getSafetyInspectionInterval(), 31);
}

TEST_F(ConstructionEquipmentTest, CalculateUsageBasedDepreciationForHeavy) {
    double depreciation = heavyEquipment->calculateUsageBasedDepreciation(8);
    EXPECT_NEAR(depreciation, 26.75835616438356, 0.001);
}

TEST_F(ConstructionEquipmentTest, CalculateUsageBasedDepreciationForGeneral) {
    double baseDepr = lightEquipment->calculateDepreciation();
    double depreciation = lightEquipment->calculateUsageBasedDepreciation(5);
    EXPECT_NEAR(depreciation, baseDepr + 0.06, 0.001);
}