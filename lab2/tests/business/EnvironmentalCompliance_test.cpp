#include <gtest/gtest.h>
#include "../../include/business/EnvironmentalCompliance.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/core/RentalItem.h"

class EnvironmentalComplianceTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventory = std::make_shared<Inventory>();
        maintenanceScheduler = std::make_shared<MaintenanceScheduler>(inventory, nullptr);
        envCompliance = std::make_shared<EnvironmentalCompliance>(inventory, maintenanceScheduler);
        
        rentalItem1 = std::make_shared<RentalItem>("VEH001", "Truck", "VEHICLE", 200.0);
        rentalItem2 = std::make_shared<RentalItem>("CONST001", "Crane", "CONSTRUCTION", 500.0);
        rentalItem3 = std::make_shared<RentalItem>("ELEC001", "Generator", "ELECTRONICS", 300.0);
        rentalItem4 = std::make_shared<RentalItem>("BIC001", "Bicycle", "BICYCLE", 50.0);
        
        inventory->addItem(rentalItem1);
        inventory->addItem(rentalItem2);
        inventory->addItem(rentalItem3);
        inventory->addItem(rentalItem4);
    }
    
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::shared_ptr<EnvironmentalCompliance> envCompliance;
    std::shared_ptr<RentalItem> rentalItem1;
    std::shared_ptr<RentalItem> rentalItem2;
    std::shared_ptr<RentalItem> rentalItem3;
    std::shared_ptr<RentalItem> rentalItem4;
};

TEST_F(EnvironmentalComplianceTest, CalculateCarbonFootprintForVehicle) {
    double footprint = envCompliance->calculateCarbonFootprint("VEH001", 7);
    EXPECT_GT(footprint, 0.0);
}

TEST_F(EnvironmentalComplianceTest, CalculateCarbonFootprintForConstruction) {
    double footprint = envCompliance->calculateCarbonFootprint("CONST001", 14);
    EXPECT_GT(footprint, 0.0);
}

TEST_F(EnvironmentalComplianceTest, CalculateCarbonFootprintForElectronics) {
    double footprint = envCompliance->calculateCarbonFootprint("ELEC001", 30);
    EXPECT_GT(footprint, 0.0);
}

TEST_F(EnvironmentalComplianceTest, CalculateRecyclingCostForElectronics) {
    double cost = envCompliance->calculateRecyclingCost("ELECTRONICS", 1000.0);
    EXPECT_GT(cost, 0.0);
}

TEST_F(EnvironmentalComplianceTest, CalculateRecyclingCostForBatteries) {
    double cost = envCompliance->calculateRecyclingCost("BATTERIES", 500.0);
    EXPECT_GT(cost, 0.0);
}

TEST_F(EnvironmentalComplianceTest, IsEcoFriendlyCategory) {
    bool isEcoFriendly = envCompliance->isEcoFriendlyCategory("BICYCLE");
    EXPECT_TRUE(isEcoFriendly);
    
    isEcoFriendly = envCompliance->isEcoFriendlyCategory("VEHICLE");
    EXPECT_FALSE(isEcoFriendly);
}

TEST_F(EnvironmentalComplianceTest, CalculateEnvironmentalPenalty) {
    double penalty = envCompliance->calculateEnvironmentalPenalty("VEHICLE", "OIL_SPILL");
    EXPECT_GT(penalty, 0.0);
    
    penalty = envCompliance->calculateEnvironmentalPenalty("ELECTRONICS", "ELECTRONIC_WASTE");
    EXPECT_GT(penalty, 0.0);
}

TEST_F(EnvironmentalComplianceTest, GetEcoFriendlyRecommendations) {
    auto recommendations = envCompliance->getEcoFriendlyRecommendations();
    EXPECT_FALSE(recommendations.empty());
}

TEST_F(EnvironmentalComplianceTest, CalculateGreenDiscountForEcoFriendly) {
    double discount = envCompliance->calculateGreenDiscount("BICYCLE");
    EXPECT_GT(discount, 0.0);
}

TEST_F(EnvironmentalComplianceTest, CalculateGreenDiscountForNonEcoFriendly) {
    double discount = envCompliance->calculateGreenDiscount("VEHICLE");
    EXPECT_EQ(discount, 0.0);
}

TEST_F(EnvironmentalComplianceTest, RequiresEnvironmentalDisclosure) {
    bool requires = envCompliance->requiresEnvironmentalDisclosure("CHEMICALS");
    EXPECT_TRUE(requires);
    
    requires = envCompliance->requiresEnvironmentalDisclosure("BATTERIES");
    EXPECT_TRUE(requires);
    
    requires = envCompliance->requiresEnvironmentalDisclosure("ELECTRONICS");
    EXPECT_TRUE(requires);
    
    requires = envCompliance->requiresEnvironmentalDisclosure("BICYCLE");
    EXPECT_FALSE(requires);
}

TEST_F(EnvironmentalComplianceTest, CalculateWasteManagementCost) {
    double cost = envCompliance->calculateWasteManagementCost("ELEC001");
    EXPECT_GT(cost, 0.0);
    
    cost = envCompliance->calculateWasteManagementCost("BATT001");
    EXPECT_GT(cost, 0.0);
}

TEST_F(EnvironmentalComplianceTest, GetCarbonFootprintFactor) {
    double factor = envCompliance->getCarbonFootprintFactor();
    EXPECT_EQ(factor, 0.1);
}

TEST_F(EnvironmentalComplianceTest, SetCarbonFootprintFactor) {
    envCompliance->setCarbonFootprintFactor(0.2);
    double factor = envCompliance->getCarbonFootprintFactor();
    EXPECT_EQ(factor, 0.2);
}

TEST_F(EnvironmentalComplianceTest, GetInventory) {
    auto retrievedInventory = envCompliance->getInventory();
    EXPECT_EQ(retrievedInventory, inventory);
}

TEST_F(EnvironmentalComplianceTest, GetMaintenanceScheduler) {
    auto retrievedScheduler = envCompliance->getMaintenanceScheduler();
    EXPECT_EQ(retrievedScheduler, maintenanceScheduler);
}

TEST_F(EnvironmentalComplianceTest, SetInventory) {
    auto newInventory = std::make_shared<Inventory>();
    envCompliance->setInventory(newInventory);
    
    auto retrievedInventory = envCompliance->getInventory();
    EXPECT_EQ(retrievedInventory, newInventory);
}

TEST_F(EnvironmentalComplianceTest, SetMaintenanceScheduler) {
    auto newScheduler = std::make_shared<MaintenanceScheduler>();
    envCompliance->setMaintenanceScheduler(newScheduler);
    
    auto retrievedScheduler = envCompliance->getMaintenanceScheduler();
    EXPECT_EQ(retrievedScheduler, newScheduler);
}

TEST_F(EnvironmentalComplianceTest, AddEcoFriendlyCategory) {
    bool isEcoFriendly = envCompliance->isEcoFriendlyCategory("SOLAR_PANELS");
    EXPECT_FALSE(isEcoFriendly);
    
    envCompliance->addEcoFriendlyCategory("SOLAR_PANELS");
    
    isEcoFriendly = envCompliance->isEcoFriendlyCategory("SOLAR_PANELS");
    EXPECT_TRUE(isEcoFriendly);
}

TEST_F(EnvironmentalComplianceTest, RecyclingCostForUnknownCategory) {
    double cost = envCompliance->calculateRecyclingCost("UNKNOWN_CATEGORY", 100.0);
    EXPECT_GT(cost, 0.0);
}

TEST_F(EnvironmentalComplianceTest, EnvironmentalPenaltyForUnknownDamageType) {
    double penalty = envCompliance->calculateEnvironmentalPenalty("VEHICLE", "UNKNOWN_DAMAGE");
    EXPECT_EQ(penalty, 0.0);
}

TEST_F(EnvironmentalComplianceTest, GreenDiscountForLowImpactCategory) {
    double discount = envCompliance->calculateGreenDiscount("SPORTS");
    EXPECT_GT(discount, 0.0);
}

TEST_F(EnvironmentalComplianceTest, CarbonFootprintForGeneralCategory) {
    double footprint = envCompliance->calculateCarbonFootprint("TOOL001", 10);
    EXPECT_GT(footprint, 0.0);
}

TEST_F(EnvironmentalComplianceTest, EnvironmentalDisclosureForLowImpactCategory) {
    bool requires = envCompliance->requiresEnvironmentalDisclosure("SPORTS");
    EXPECT_FALSE(requires);
}