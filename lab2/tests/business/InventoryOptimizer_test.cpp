#include <gtest/gtest.h>
#include "../../include/business/InventoryOptimizer.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/AnalyticsEngine.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/ReportGenerator.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/core/RentalAgreement.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/LateFeeCalculator.h"
#include "../../include/business/DiscountCalculator.h"
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

class InventoryOptimizerTest : public ::testing::Test {
protected:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<AnalyticsEngine> analyticsEngine;
    std::shared_ptr<InventoryOptimizer> inventoryOptimizer;
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<ReportGenerator> reportGenerator;
    std::shared_ptr<CustomerManager> customerManager; 

    void SetUp() override {
        auto recommender = std::make_shared<RecommendationEngine>();
        auto lateCalculator = std::make_shared<LateFeeCalculator>();
        customerManager = std::make_shared<CustomerManager>(recommender, lateCalculator); 
        auto customerManager_up = std::make_unique<CustomerManager>(*customerManager);

        auto damageAssessor = std::make_shared<DamageAssessor>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);

        inventory->addItem(std::make_shared<RentalItem>("VEH001", "Truck", "VEHICLE", 100.0));
        inventory->addItem(std::make_shared<RentalItem>("VEH002", "Car", "VEHICLE", 100.0));
        inventory->addItem(std::make_shared<RentalItem>("ITEM001", "Drill", "TOOLS", 20.0));
        inventory->addItem(std::make_shared<RentalItem>("ITEM002", "Saw", "TOOLS", 20.0));
        inventory->addItem(std::make_shared<RentalItem>("ITEM003", "Hammer", "TOOLS", 20.0));
        inventory->addItem(std::make_shared<RentalItem>("SLOW001", "Decoration", "DECO", 5.0));
        
        inventory->markItemAsRented("VEH001"); 
        inventory->markItemAsReturned("VEH001");
        inventory->markItemAsRented("VEH001");

        inventory->markItemAsRented("ITEM001");

        auto resSys = std::make_shared<ReservationSystem>();
        auto pricingStrategy = std::make_unique<PricingStrategy>();
        auto discountCalculator = std::make_shared<DiscountCalculator>();
        auto payProc = std::make_unique<PaymentProcessor>(std::make_unique<PricingStrategy>(), discountCalculator);
        
        rentalSystem = std::make_shared<RentalSystem>(
            std::make_unique<Inventory>(*inventory),
            std::move(customerManager_up),
            std::make_unique<ReservationSystem>(*resSys),
            std::move(payProc),
            std::move(pricingStrategy)
        );
        
        reportGenerator = std::make_shared<ReportGenerator>(inventory, rentalSystem, customerManager);

        analyticsEngine = std::make_shared<AnalyticsEngine>(rentalSystem, resSys, reportGenerator);
        inventoryOptimizer = std::make_shared<InventoryOptimizer>(inventory, analyticsEngine);
    }
};

TEST_F(InventoryOptimizerTest, CalculateOptimalInventoryLevelForHighDemandCategory) {
    double level = inventoryOptimizer->calculateOptimalInventoryLevel("ELECTRONICS");
    EXPECT_NEAR(level, 0.936, 0.001);
}

TEST_F(InventoryOptimizerTest, CalculateOptimalInventoryLevelForUnknownCategoryClamps) {
    double level = inventoryOptimizer->calculateOptimalInventoryLevel("UNKNOWN");
    EXPECT_NEAR(level, 0.84, 0.001);
}

TEST_F(InventoryOptimizerTest, CalculateOptimalInventoryLevelForWinter) {
    inventoryOptimizer->updateOptimalLevels("TEST_CAT", 0.5);
    double level = inventoryOptimizer->calculateOptimalInventoryLevel("TEST_CAT");
    EXPECT_NEAR(level, 0.6, 0.001);
}

TEST_F(InventoryOptimizerTest, IdentifySlowMovingItemsReturnsLowUtilCategory) {
    auto slowMoving = inventoryOptimizer->identifySlowMovingItems(0.1);
    EXPECT_TRUE(std::find(slowMoving.begin(), slowMoving.end(), "DECO") != slowMoving.end());
}

TEST_F(InventoryOptimizerTest, IdentifySlowMovingItemsReturnsLowOverallUtilization) {
    auto slowMoving = inventoryOptimizer->identifySlowMovingItems(0.5);
    EXPECT_TRUE(std::find(slowMoving.begin(), slowMoving.end(), "SYSTEM_WIDE: Low overall utilization") != slowMoving.end());
}

TEST_F(InventoryOptimizerTest, ShouldReorderItemForLowStock) {
    EXPECT_TRUE(inventoryOptimizer->shouldReorderItem("VEH003", 5));
}

TEST_F(InventoryOptimizerTest, ShouldReorderItemForHighStockoutRisk) {
    double risk = inventoryOptimizer->calculateStockoutRisk("VEH001");
    EXPECT_GT(risk, 0.3);
    EXPECT_TRUE(inventoryOptimizer->shouldReorderItem("VEH001", 50));
}

TEST_F(InventoryOptimizerTest, ShouldReorderItemReturnsFalseForHighStockLowRisk) {
    inventoryOptimizer->updateOptimalLevels("TOOL_CAT", 0.1);
    EXPECT_FALSE(inventoryOptimizer->shouldReorderItem("TOOL005", 50));
}

TEST_F(InventoryOptimizerTest, CalculateCarryingCostForElectronics) {
    double cost = inventoryOptimizer->calculateCarryingCost("ELECTRONICS", 50);
    EXPECT_NEAR(cost, 135.0, 0.001);
}

TEST_F(InventoryOptimizerTest, CalculateCarryingCostForGeneral) {
    double cost = inventoryOptimizer->calculateCarryingCost("GENERAL", 10);
    EXPECT_NEAR(cost, 18.0, 0.001);
}

TEST_F(InventoryOptimizerTest, OptimizeCategoryDistributionReturnsAllRecommendations) {
    inventoryOptimizer->calculateCarryingCost("VEHICLE", 500);
    inventoryOptimizer->calculateCarryingCost("TOOLS", 500);

    auto recommendations = inventoryOptimizer->optimizeCategoryDistribution();
    
    EXPECT_TRUE(std::find(recommendations.begin(), recommendations.end(), "Reduce inventory for DECO - slow moving") != recommendations.end());
    
    inventory->markItemAsRented("VEH002"); 

    inventoryOptimizer->addHighDemandCategory("VEHICLE");

    auto highDemandRecs = inventoryOptimizer->optimizeCategoryDistribution();
    EXPECT_TRUE(std::find(highDemandRecs.begin(), highDemandRecs.end(), "Increase inventory for VEHICLE - high demand") != highDemandRecs.end());
}

TEST_F(InventoryOptimizerTest, IsOverstockedReturnsTrue) {
    EXPECT_TRUE(inventoryOptimizer->isOverstocked("TOOLS", 150));
}

TEST_F(InventoryOptimizerTest, IsOverstockedReturnsFalse) {
    EXPECT_FALSE(inventoryOptimizer->isOverstocked("TOOLS", 100));
}

TEST_F(InventoryOptimizerTest, UpdateOptimalLevelsClampsValue) {
    inventoryOptimizer->updateOptimalLevels("NEW_CAT", 2.0);
    EXPECT_DOUBLE_EQ(inventoryOptimizer->getCategoryOptimalLevel("NEW_CAT"), 1.0);
    
    inventoryOptimizer->updateOptimalLevels("NEW_CAT_2", 0.0);
    EXPECT_DOUBLE_EQ(inventoryOptimizer->getCategoryOptimalLevel("NEW_CAT_2"), 0.1);
}

TEST_F(InventoryOptimizerTest, SetAndGetInventory) {
    auto newInventory = std::make_shared<Inventory>(std::make_shared<MaintenanceScheduler>(), std::make_shared<DamageAssessor>());
    inventoryOptimizer->setInventory(newInventory);
    EXPECT_EQ(inventoryOptimizer->getInventory(), newInventory);
}

TEST_F(InventoryOptimizerTest, SetAndGetAnalyticsEngine) {
    auto newAnalyticsEngine = std::make_shared<AnalyticsEngine>(rentalSystem, std::make_shared<ReservationSystem>(), reportGenerator);
    inventoryOptimizer->setAnalyticsEngine(newAnalyticsEngine);
    EXPECT_EQ(inventoryOptimizer->getAnalyticsEngine(), newAnalyticsEngine);
}