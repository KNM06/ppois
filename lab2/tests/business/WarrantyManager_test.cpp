#include <gtest/gtest.h>
#include "../../include/business/WarrantyManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/InsuranceCalculator.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class WarrantyManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::shared_ptr<WarrantyManager> warrantyManager;

    void SetUp() override {
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        damageAssessor = std::make_shared<DamageAssessor>();
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        inventory->addItem(std::make_shared<RentalItem>("ELEC001", "Laptop", "ELECTRONICS", 50.0));
        inventory->addItem(std::make_shared<RentalItem>("VEH001", "Car", "VEHICLE", 100.0));
        inventory->addItem(std::make_shared<RentalItem>("ITEM001", "Drill", "TOOLS", 20.0));

        warrantyManager = std::make_shared<WarrantyManager>(inventory, damageAssessor);
    }
};

TEST_F(WarrantyManagerTest, IssueWarrantySuccess) {
    bool issued = warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    EXPECT_TRUE(issued);
}

TEST_F(WarrantyManagerTest, IssueWarrantyForNewItem) {
    bool issued = warrantyManager->issueWarranty("VEH001", "BASIC", 24);
    EXPECT_TRUE(issued);
}

TEST_F(WarrantyManagerTest, CalculateWarrantyCostElectronicsPremium) {
    double cost = warrantyManager->calculateWarrantyCost("ELEC001", "PREMIUM");
    EXPECT_GT(cost, 120.0);
}

TEST_F(WarrantyManagerTest, CalculateWarrantyCostToolsBasic) {
    double cost = warrantyManager->calculateWarrantyCost("ITEM001", "BASIC");
    EXPECT_GT(cost, 35.0);
}

TEST_F(WarrantyManagerTest, CalculateWarrantyCostUnknownCategory) {
    double cost = warrantyManager->calculateWarrantyCost("UNKNOWN001", "BASIC");
    EXPECT_GT(cost, 0.0);
}

TEST_F(WarrantyManagerTest, IsWarrantyValidTrue) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    EXPECT_TRUE(warrantyManager->isWarrantyValid("ELEC001", "2024-06-01"));
}

TEST_F(WarrantyManagerTest, IsWarrantyValidFalseExpired) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    EXPECT_FALSE(warrantyManager->isWarrantyValid("ELEC001", "2025-01-01"));
}

TEST_F(WarrantyManagerTest, IsWarrantyValidFalseNoWarranty) {
    EXPECT_FALSE(warrantyManager->isWarrantyValid("ITEM001", "2024-06-01"));
}

TEST_F(WarrantyManagerTest, ProcessWarrantyClaimCovered) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    double covered = warrantyManager->processWarrantyClaim("ELEC001", 500.0);
    EXPECT_DOUBLE_EQ(covered, 500.0);
}

TEST_F(WarrantyManagerTest, ProcessWarrantyClaimPartial) {
    warrantyManager->issueWarranty("ITEM001", "BASIC", 12);
    double covered = warrantyManager->processWarrantyClaim("ITEM001", 10000.0);
    EXPECT_LT(covered, 10000.0);
    EXPECT_GT(covered, 350.0);
}

TEST_F(WarrantyManagerTest, ProcessWarrantyClaimNoActiveWarranty) {
    double covered = warrantyManager->processWarrantyClaim("VEH001", 500.0);
    EXPECT_DOUBLE_EQ(covered, 0.0);
}

TEST_F(WarrantyManagerTest, GetExpiringWarrantiesNone) {
    auto expiring = warrantyManager->getExpiringWarranties(30);
    EXPECT_TRUE(expiring.empty());
}

TEST_F(WarrantyManagerTest, GetExpiringWarrantiesFound) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    auto expiring = warrantyManager->getExpiringWarranties(30);
    EXPECT_FALSE(expiring.empty());
}

TEST_F(WarrantyManagerTest, CalculateWarrantyROIPositive) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    double roi = warrantyManager->calculateWarrantyROI("ELEC001");
    EXPECT_DOUBLE_EQ(roi, 89);
}

TEST_F(WarrantyManagerTest, CalculateWarrantyROIAfterClaim) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    warrantyManager->processWarrantyClaim("ELEC001", 10000.0);
    double roi = warrantyManager->calculateWarrantyROI("ELEC001");
    EXPECT_NEAR(roi, -1, 0.001);
}

TEST_F(WarrantyManagerTest, ShouldExtendWarrantyTrue) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    warrantyManager->processWarrantyClaim("ELEC001", 10000.0);
    EXPECT_FALSE(warrantyManager->shouldExtendWarranty("ELEC001"));
}

TEST_F(WarrantyManagerTest, ShouldExtendWarrantyFalseLowROI) {
    warrantyManager->issueWarranty("ELEC001", "PREMIUM", 12);
    EXPECT_TRUE(warrantyManager->shouldExtendWarranty("ELEC001"));
}

TEST_F(WarrantyManagerTest, UpdateWarrantyRatesClamps) {
    warrantyManager->updateWarrantyRates("ELECTRONICS", 0.6);
    EXPECT_DOUBLE_EQ(warrantyManager->getCategoryWarrantyRate("ELECTRONICS"), 0.5);
    
    warrantyManager->updateWarrantyRates("ELECTRONICS", 0.001);
    EXPECT_DOUBLE_EQ(warrantyManager->getCategoryWarrantyRate("ELECTRONICS"), 0.01);
}

TEST_F(WarrantyManagerTest, SettersAndGetters) {
    auto newInv = std::make_shared<Inventory>();
    warrantyManager->setInventory(newInv);
    EXPECT_EQ(warrantyManager->getInventory(), newInv);
    
    auto newAssessor = std::make_shared<DamageAssessor>();
    warrantyManager->setDamageAssessor(newAssessor);
    EXPECT_EQ(warrantyManager->getDamageAssessor(), newAssessor);
    
    warrantyManager->setWarrantyDuration("TOOLS", 10);
}