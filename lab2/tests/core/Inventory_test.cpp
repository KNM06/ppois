#include <gtest/gtest.h>
#include "../../include/core/Inventory.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"

class InventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        damageAssessor = std::make_shared<DamageAssessor>();
        
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        rentalItem1 = std::make_shared<RentalItem>("ITEM001", "Hammer", "TOOLS", 10.0);
        rentalItem2 = std::make_shared<RentalItem>("ITEM002", "Drill", "TOOLS", 50.0);
        rentalItem3 = std::make_shared<RentalItem>("VEH001", "Truck", "VEHICLE", 200.0);
        rentalItem4 = std::make_shared<RentalItem>("ELEC001", "Generator", "ELECTRONICS", 300.0);
        
        inventory->addItem(rentalItem1);
        inventory->addItem(rentalItem2);
        inventory->addItem(rentalItem3);
        inventory->addItem(rentalItem4);
        
        inventory->addItemWithLocation(rentalItem1, "Warehouse A");
        inventory->addItemWithLocation(rentalItem2, "Warehouse B");
    }
    
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::shared_ptr<RentalItem> rentalItem1;
    std::shared_ptr<RentalItem> rentalItem2;
    std::shared_ptr<RentalItem> rentalItem3;
    std::shared_ptr<RentalItem> rentalItem4;
};

TEST_F(InventoryTest, IsItemAvailable) {
    bool available = inventory->isItemAvailable("ITEM001");
    EXPECT_TRUE(available);
    
    available = inventory->isItemAvailable("NONEXISTENT");
    EXPECT_FALSE(available);
}

TEST_F(InventoryTest, MarkItemAsRented) {
    bool availableBefore = inventory->isItemAvailable("ITEM001");
    EXPECT_TRUE(availableBefore);
    
    inventory->markItemAsRented("ITEM001");
    
    bool availableAfter = inventory->isItemAvailable("ITEM001");
    EXPECT_FALSE(availableAfter);
}

TEST_F(InventoryTest, MarkItemAsReturned) {
    inventory->markItemAsRented("ITEM001");
    bool rented = !inventory->isItemAvailable("ITEM001");
    EXPECT_TRUE(rented);
    
    inventory->markItemAsReturned("ITEM001");
    bool returned = inventory->isItemAvailable("ITEM001");
    EXPECT_TRUE(returned);
}

TEST_F(InventoryTest, GetUtilizationByCategory) {
    inventory->markItemAsRented("ITEM001");
    inventory->markItemAsRented("VEH001");
    
    auto utilization = inventory->getUtilizationByCategory();
    EXPECT_GT(utilization.size(), 0);
    
    auto toolsUtilization = utilization["TOOLS"];
    EXPECT_GT(toolsUtilization, 0.0);
    EXPECT_LE(toolsUtilization, 1.0);
}

TEST_F(InventoryTest, AdjustCategoryCapacity) {
    inventory->adjustCategoryCapacity("TOOLS", 0.2);
    
    double avgPrice = inventory->getAveragePriceForCategory("TOOLS");
    EXPECT_GT(avgPrice, 0.0);
}

TEST_F(InventoryTest, GetAveragePriceForCategory) {
    double avgPrice = inventory->getAveragePriceForCategory("TOOLS");
    EXPECT_GT(avgPrice, 0.0);
    
    avgPrice = inventory->getAveragePriceForCategory("VEHICLE");
    EXPECT_GT(avgPrice, 0.0);
    
    avgPrice = inventory->getAveragePriceForCategory("NONEXISTENT");
    EXPECT_EQ(avgPrice, 0.0);
}

TEST_F(InventoryTest, AdjustCategoryPricing) {
    double initialPrice = inventory->getAveragePriceForCategory("TOOLS");
    
    inventory->adjustCategoryPricing("TOOLS", 1.2);
    
    double adjustedPrice = inventory->getAveragePriceForCategory("TOOLS");
    EXPECT_GT(adjustedPrice, initialPrice * 0.9);
}

TEST_F(InventoryTest, GetAllCategories) {
    auto categories = inventory->getAllCategories();
    EXPECT_GE(categories.size(), 3);
    
    bool hasTools = std::find(categories.begin(), categories.end(), "TOOLS") != categories.end();
    bool hasVehicle = std::find(categories.begin(), categories.end(), "VEHICLE") != categories.end();
    bool hasElectronics = std::find(categories.begin(), categories.end(), "ELECTRONICS") != categories.end();
    
    EXPECT_TRUE(hasTools);
    EXPECT_TRUE(hasVehicle);
    EXPECT_TRUE(hasElectronics);
}

TEST_F(InventoryTest, CalculateTotalInventoryValue) {
    double totalValue = inventory->calculateTotalInventoryValue();
    EXPECT_GT(totalValue, 0.0);
}

TEST_F(InventoryTest, FindItemsForMaintenance) {
    auto maintenanceItems = inventory->findItemsForMaintenance();
    EXPECT_GE(maintenanceItems.size(), 0);
}

TEST_F(InventoryTest, RelocateItem) {
    bool relocated = inventory->relocateItem("ITEM001", "Warehouse C");
    EXPECT_TRUE(relocated);
    
    relocated = inventory->relocateItem("NONEXISTENT", "Warehouse C");
    EXPECT_FALSE(relocated);
}

TEST_F(InventoryTest, GetMaintenanceScheduler) {
    auto retrievedScheduler = inventory->getMaintenanceScheduler();
    EXPECT_EQ(retrievedScheduler, maintenanceScheduler);
}

TEST_F(InventoryTest, GetDamageAssessor) {
    auto retrievedAssessor = inventory->getDamageAssessor();
    EXPECT_EQ(retrievedAssessor, damageAssessor);
}

TEST_F(InventoryTest, SetMaintenanceScheduler) {
    auto newScheduler = std::make_shared<MaintenanceScheduler>();
    inventory->setMaintenanceScheduler(newScheduler);
    
    auto retrievedScheduler = inventory->getMaintenanceScheduler();
    EXPECT_EQ(retrievedScheduler, newScheduler);
}

TEST_F(InventoryTest, SetDamageAssessor) {
    auto newAssessor = std::make_shared<DamageAssessor>();
    inventory->setDamageAssessor(newAssessor);
    
    auto retrievedAssessor = inventory->getDamageAssessor();
    EXPECT_EQ(retrievedAssessor, newAssessor);
}

TEST_F(InventoryTest, AddItemWithLocation) {
    auto newItem = std::make_shared<RentalItem>("ITEM005", "Saw", "TOOLS", 30.0);
    inventory->addItemWithLocation(newItem, "Warehouse D");
    
    bool available = inventory->isItemAvailable("ITEM005");
    EXPECT_TRUE(available);
}

TEST_F(InventoryTest, MarkNonExistentItemAsRented) {
    inventory->markItemAsRented("NONEXISTENT");
    
    bool available = inventory->isItemAvailable("NONEXISTENT");
    EXPECT_FALSE(available);
}

TEST_F(InventoryTest, MarkNonExistentItemAsReturned) {
    inventory->markItemAsReturned("NONEXISTENT");
    
    bool available = inventory->isItemAvailable("NONEXISTENT");
    EXPECT_FALSE(available);
}

TEST_F(InventoryTest, RelocateRentedItem) {
    inventory->markItemAsRented("ITEM001");
    
    bool relocated = inventory->relocateItem("ITEM001", "Warehouse C");
    EXPECT_FALSE(relocated);
}