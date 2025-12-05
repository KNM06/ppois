#include <gtest/gtest.h>
#include "../../include/items/Tools.h"
#include "../../include/core/RentalItem.h"
#include "../../include/core/RentalPricing.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class ToolsTest : public ::testing::Test {
protected:
    std::shared_ptr<Tools> powerDrill;
    std::shared_ptr<Tools> precisionTool;
    std::shared_ptr<Tools> cuttingTool;
    std::shared_ptr<Tools> heavyPowerTool;

    void SetUp() override {
        powerDrill = std::make_shared<Tools>("TOOL001", "Power Drill", "POWER_DRILL", "STEEL");
        precisionTool = std::make_shared<Tools>("TOOL002", "Calipers", "PRECISION", "SENSITIVE");
        cuttingTool = std::make_shared<Tools>("TOOL003", "Axe", "CUTTING", "CARBON_STEEL");
        heavyPowerTool = std::make_shared<Tools>("TOOL004", "Heavy Saw", "POWER_SAW", "STEEL");
        
        powerDrill->updateMaintenanceStatus(200);
        cuttingTool->updateMaintenanceStatus(100);
    }
};

TEST_F(ToolsTest, ConstructorInitialization) {
    EXPECT_EQ(powerDrill->getItemId(), "TOOL001");
    EXPECT_EQ(powerDrill->getName(), "Power Drill");
    EXPECT_EQ(powerDrill->getToolType(), "POWER_DRILL");
    EXPECT_EQ(powerDrill->getMaterial(), "STEEL");
    EXPECT_TRUE(powerDrill->getIsPowerTool());
    EXPECT_EQ(powerDrill->getDurabilityScore(), 80);
}

TEST_F(ToolsTest, CalculateSharpeningCostForCuttingTool) {
    auto tempCuttingTool = std::make_shared<Tools>("TOOL003", "Axe", "CUTTING", "CARBON_STEEL");
    double cost = tempCuttingTool->calculateSharpeningCost();
    EXPECT_NEAR(cost, 18, 0.001);
}

TEST_F(ToolsTest, CalculateSharpeningCostForNonSharpeningTool) {
    double cost = powerDrill->calculateSharpeningCost();
    EXPECT_DOUBLE_EQ(cost, 0.0);
}

TEST_F(ToolsTest, CalculateSharpeningCostDiamondMaterial) {
    auto diamondTool = std::make_shared<Tools>("TOOL005", "Cutter", "CUTTING", "DIAMOND");
    double cost = diamondTool->calculateSharpeningCost();
    EXPECT_NEAR(cost, 37.5, 0.001);
}

TEST_F(ToolsTest, RequiresSpecialStorageForPrecision) {
    EXPECT_TRUE(precisionTool->requiresSpecialStorage());
}

TEST_F(ToolsTest, RequiresSpecialStorageForPowerToolBattery) {
    EXPECT_TRUE(powerDrill->requiresSpecialStorage());
}

TEST_F(ToolsTest, RequiresSpecialStorageReturnsFalse) {
    EXPECT_FALSE(cuttingTool->requiresSpecialStorage());
}

TEST_F(ToolsTest, CalculateWearAndTearPrecisionTool) {
    double wear = precisionTool->calculateWearAndTear(10);
    EXPECT_NEAR(wear, 88.0, 0.001);
}

TEST_F(ToolsTest, CalculateWearAndTearGardenTool) {
    auto gardenTool = std::make_shared<Tools>("TOOL005", "Rake", "GARDEN", "STEEL");
    double wear = gardenTool->calculateWearAndTear(5);
    EXPECT_NEAR(wear, 9.75, 0.001);
}

TEST_F(ToolsTest, CalculateWearAndTearHighFactorPlastic) {
    auto plasticTool = std::make_shared<Tools>("TOOL006", "Plastic Tool", "POWER_DRILL", "PLASTIC");
    double wear = plasticTool->calculateWearAndTear(1);
    EXPECT_NEAR(wear, 6.3, 0.001);
}

TEST_F(ToolsTest, GetRecommendedMaintenanceIntervalPowerTool) {
    EXPECT_EQ(powerDrill->getRecommendedMaintenanceInterval(), 62);
}

TEST_F(ToolsTest, GetRecommendedMaintenanceIntervalPrecisionTool) {
    EXPECT_EQ(precisionTool->getRecommendedMaintenanceInterval(), 60);
}

TEST_F(ToolsTest, GetRecommendedMaintenanceIntervalLowDurability) {
    auto fragileTool = std::make_shared<Tools>("TOOL007", "Fragile", "POWER_DRILL", "STEEL");
    EXPECT_EQ(fragileTool->getRecommendedMaintenanceInterval(), 90);
}

TEST_F(ToolsTest, ValidateUserExperienceBeginnerFails) {
    EXPECT_FALSE(heavyPowerTool->validateUserExperience("BEGINNER"));
}

TEST_F(ToolsTest, ValidateUserExperienceBeginnerPasses) {
    EXPECT_TRUE(powerDrill->validateUserExperience("BEGINNER"));
}

TEST_F(ToolsTest, ValidateUserExperienceExpert) {
    EXPECT_TRUE(precisionTool->validateUserExperience("EXPERT"));
}

TEST_F(ToolsTest, CalculateAccessoryCompatibilityCostCompatible) {
    double cost = powerDrill->calculateAccessoryCompatibilityCost("Battery");
    EXPECT_DOUBLE_EQ(cost, 20.0);
}

TEST_F(ToolsTest, CalculateAccessoryCompatibilityCostIncompatible) {
    double cost = powerDrill->calculateAccessoryCompatibilityCost("Sharpening_Kit");
    EXPECT_DOUBLE_EQ(cost, 0.0);
}