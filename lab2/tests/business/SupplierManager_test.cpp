#include <gtest/gtest.h>
#include "../../include/business/SupplierManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/InsuranceCalculator.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class SupplierManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<SupplierManager> supplierManager;
    std::shared_ptr<Inventory> inventory;

    void SetUp() override {
        auto insuranceCalculator = std::make_shared<InsuranceCalculator>();
        auto damageAssessor = std::make_shared<DamageAssessor>(insuranceCalculator);
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        supplierManager = std::make_shared<SupplierManager>(inventory);
        
        Supplier supplier;
        supplier.supplierId = "SUP001";
        supplier.name = "Tools Inc";
        supplier.contactInfo = "contact@tools.com";
        supplier.providedCategories = {"TOOLS", "CONSTRUCTION"};
        supplier.reliabilityScore = 0.9;
        supplier.deliveryTimeDays = 3.0;
        supplierManager->addSupplier(supplier);
    }
};

TEST_F(SupplierManagerTest, AddSupplier) {
    Supplier newSupplier;
    newSupplier.supplierId = "SUP002";
    newSupplier.providedCategories = {"TOOLS"};
    newSupplier.reliabilityScore = 0.85;
    
    supplierManager->addSupplier(newSupplier);
    
    auto reliable = supplierManager->findReliableSuppliers("TOOLS", 0.8);
    EXPECT_EQ(reliable.size(), 2u);
}

TEST_F(SupplierManagerTest, AssignSupplierToItemSuccess) {
    bool success = supplierManager->assignSupplierToItem("ITEM001", "SUP001");
    EXPECT_TRUE(success);
}

TEST_F(SupplierManagerTest, AssignSupplierToItemFailureUnknownSupplier) {
    bool success = supplierManager->assignSupplierToItem("ITEM001", "UNKNOWN_SUP");
    EXPECT_FALSE(success);
}

TEST_F(SupplierManagerTest, CalculateProcurementCostNoSupplierAssigned) {
    double cost = supplierManager->calculateProcurementCost("ITEM_NO_SUP", 10);
    EXPECT_DOUBLE_EQ(cost, 0.0);
}

TEST_F(SupplierManagerTest, CalculateProcurementCostBulkDiscountHigh) {
    supplierManager->assignSupplierToItem("ITEM001", "SUP001");
    
    double cost = supplierManager->calculateProcurementCost("ITEM001", 11);
    EXPECT_NEAR(cost, 850.85, 0.001);
}

TEST_F(SupplierManagerTest, CalculateProcurementCostBulkDiscountMedium) {
    supplierManager->assignSupplierToItem("ITEM001", "SUP001");
    double cost = supplierManager->calculateProcurementCost("ITEM001", 6);
    EXPECT_NEAR(cost, 491.4, 0.001);
}

TEST_F(SupplierManagerTest, CalculateProcurementCostBulkDiscountLow) {
    supplierManager->assignSupplierToItem("ITEM001", "SUP001");
    double cost = supplierManager->calculateProcurementCost("ITEM001", 5);
    EXPECT_NEAR(cost, 432.25, 0.001);
}

TEST_F(SupplierManagerTest, FindReliableSuppliersReturnsCorrectSuppliers) {
    auto reliable = supplierManager->findReliableSuppliers("CONSTRUCTION", 0.8);
    EXPECT_EQ(reliable.size(), 1u);
    EXPECT_EQ(reliable[0], "SUP001");
}

TEST_F(SupplierManagerTest, FindReliableSuppliersReturnsEmptyForHighThreshold) {
    auto reliable = supplierManager->findReliableSuppliers("TOOLS", 0.95);
    EXPECT_TRUE(reliable.empty());
}

TEST_F(SupplierManagerTest, FindReliableSuppliersReturnsEmptyForUnknownCategory) {
    auto reliable = supplierManager->findReliableSuppliers("UNKNOWN_CAT", 0.1);
    EXPECT_TRUE(reliable.empty());
}

TEST_F(SupplierManagerTest, PredictDeliveryTimeDefault) {
    double time = supplierManager->predictDeliveryTime("UNASSIGNED_ITEM");
    EXPECT_DOUBLE_EQ(time, 7.0);
}

TEST_F(SupplierManagerTest, PredictDeliveryTimeAssigned) {
    supplierManager->assignSupplierToItem("ITEM001", "SUP001");
    double time = supplierManager->predictDeliveryTime("ITEM001");
    EXPECT_NEAR(time, 2.73, 0.001);
}

TEST_F(SupplierManagerTest, UpdateSupplierPerformanceUpdatesScore) {
    supplierManager->updateSupplierPerformance("SUP001", 0.5);
    
    auto reliableOldThreshold = supplierManager->findReliableSuppliers("TOOLS", 0.8);
    EXPECT_TRUE(reliableOldThreshold.empty());
    
    auto reliableNewThreshold = supplierManager->findReliableSuppliers("TOOLS", 0.4);
    EXPECT_FALSE(reliableNewThreshold.empty());
}

TEST_F(SupplierManagerTest, UpdateSupplierPerformanceUnknownSupplier) {
    supplierManager->updateSupplierPerformance("UNKNOWN", 1.0);
    SUCCEED();
}