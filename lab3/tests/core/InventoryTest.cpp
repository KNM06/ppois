#include <gtest/gtest.h>
#include "../../include/core/Inventory.h"
#include "../../include/core/Property.h"
#include <chrono>

class InventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        property = std::make_shared<Property>("test_property", 100.0, 1000.0, nullptr, nullptr, nullptr);
        currentTime = std::chrono::system_clock::now();
        pastTime = currentTime - std::chrono::hours(24 * 30 * 3);
    }
    
    std::shared_ptr<Property> property;
    std::chrono::system_clock::time_point currentTime;
    std::chrono::system_clock::time_point pastTime;
};

TEST_F(InventoryTest, ConstructorAndGetters) {
    Inventory inv("inv1", property, "Test Item", "Category", 20, 5, 10.5, pastTime,
                  "Storage A", "Good", false, "Supplier X");
    
    EXPECT_EQ(inv.getInventoryId(), "inv1");
    EXPECT_EQ(inv.getItemName(), "Test Item");
    EXPECT_EQ(inv.getCategory(), "Category");
    EXPECT_EQ(inv.getQuantity(), 20);
    EXPECT_EQ(inv.getMinimumStock(), 5);
    EXPECT_EQ(inv.getUnitCost(), 10.5);
    EXPECT_EQ(inv.getStorageLocation(), "Storage A");
    EXPECT_EQ(inv.getCondition(), "Good");
    EXPECT_EQ(inv.getIsConsumable(), false);
    EXPECT_EQ(inv.getSupplier(), "Supplier X");
    EXPECT_EQ(inv.getProperty(), property.get());
}

TEST_F(InventoryTest, CopyConstructor) {
    Inventory original("inv2", property, "Original Item", "Cat", 15, 3, 8.0, currentTime,
                       "Loc", "New", true, "Sup");
    
    Inventory copy(original);
    
    EXPECT_EQ(copy.getInventoryId(), "inv2");
    EXPECT_EQ(copy.getItemName(), "Original Item");
    EXPECT_EQ(copy.getQuantity(), 15);
    EXPECT_EQ(copy.getUnitCost(), 8.0);
    EXPECT_EQ(copy.getIsConsumable(), true);
}

TEST_F(InventoryTest, CopyAssignmentOperator) {
    Inventory original("inv3", property, "Item A", "Cat1", 10, 2, 5.0, currentTime,
                       "Loc1", "Good", false, "Sup1");
    
    Inventory assigned("temp", nullptr, "Temp", "Temp", 0, 0, 0.0, pastTime,
                       "Temp", "Temp", true, "Temp");
    
    assigned = original;
    
    EXPECT_EQ(assigned.getInventoryId(), "inv3");
    EXPECT_EQ(assigned.getItemName(), "Item A");
    EXPECT_EQ(assigned.getQuantity(), 10);
    EXPECT_EQ(assigned.getUnitCost(), 5.0);
    EXPECT_EQ(assigned.getIsConsumable(), false);
}

TEST_F(InventoryTest, MoveConstructor) {
    Inventory original("inv4", property, "Movable Item", "Category", 8, 2, 12.0, currentTime,
                       "Location", "Good", true, "Supplier");
    
    Inventory moved(std::move(original));
    
    EXPECT_EQ(moved.getInventoryId(), "inv4");
    EXPECT_EQ(moved.getItemName(), "Movable Item");
    EXPECT_EQ(moved.getQuantity(), 8);
    EXPECT_EQ(moved.getUnitCost(), 12.0);
    EXPECT_EQ(moved.getIsConsumable(), true);
}

TEST_F(InventoryTest, MoveAssignmentOperator) {
    Inventory original("inv5", property, "Another Item", "Cat", 25, 10, 3.5, currentTime,
                       "Storage", "Excellent", false, "Vendor");
    
    Inventory moved("temp", nullptr, "Temp", "Temp", 0, 0, 0.0, pastTime,
                    "Temp", "Temp", true, "Temp");
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getInventoryId(), "inv5");
    EXPECT_EQ(moved.getItemName(), "Another Item");
    EXPECT_EQ(moved.getQuantity(), 25);
    EXPECT_EQ(moved.getUnitCost(), 3.5);
    EXPECT_EQ(moved.getIsConsumable(), false);
}

TEST_F(InventoryTest, SetQuantityPositive) {
    Inventory inv("inv6", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    inv.setQuantity(20);
    EXPECT_EQ(inv.getQuantity(), 20);
}

TEST_F(InventoryTest, SetQuantityZero) {
    Inventory inv("inv7", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    inv.setQuantity(0);
    EXPECT_EQ(inv.getQuantity(), 0);
}

TEST_F(InventoryTest, SetQuantityNegative) {
    Inventory inv("inv8", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    inv.setQuantity(-5);
    EXPECT_EQ(inv.getQuantity(), 0);
}

TEST_F(InventoryTest, SetUnitCost) {
    Inventory inv("inv9", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    inv.setUnitCost(15.5);
    EXPECT_EQ(inv.getUnitCost(), 15.5);
}

TEST_F(InventoryTest, SetUnitCostNegative) {
    Inventory inv("inv10", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    inv.setUnitCost(-5.0);
    EXPECT_EQ(inv.getUnitCost(), 0.0); 
}

TEST_F(InventoryTest, NeedsRestockingTrue) {
    Inventory inv("inv11", property, "Test", "Cat", 3, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_TRUE(inv.needsRestocking());
}

TEST_F(InventoryTest, NeedsRestockingFalse) {
    Inventory inv("inv12", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_FALSE(inv.needsRestocking());
}

TEST_F(InventoryTest, NeedsRestockingBoundary) {
    Inventory inv("inv13", property, "Test", "Cat", 5, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_TRUE(inv.needsRestocking());
}

TEST_F(InventoryTest, CalculateTotalValue) {
    Inventory inv("inv14", property, "Test", "Cat", 10, 5, 3.5, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_DOUBLE_EQ(inv.calculateTotalValue(), 35.0); 
}

TEST_F(InventoryTest, IsLowStockTrue) {
    Inventory inv("inv15", property, "Test", "Cat", 5, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_TRUE(inv.isLowStock()); 
}

TEST_F(InventoryTest, IsLowStockFalse) {
    Inventory inv("inv16", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_FALSE(inv.isLowStock()); 
}

TEST_F(InventoryTest, RequiresMaintenanceDueToCondition) {
    Inventory inv("inv17", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "poor", true, "Sup");
    
    EXPECT_TRUE(inv.requiresMaintenance());
}

TEST_F(InventoryTest, RequiresMaintenanceDueToAge) {
    auto threeYearsAgo = currentTime - std::chrono::hours(24 * 30 * 36);
    Inventory inv("inv18", property, "Test", "Cat", 10, 5, 1.0, threeYearsAgo,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_TRUE(inv.requiresMaintenance()); 
}

TEST_F(InventoryTest, RequiresMaintenanceFalse) {
    Inventory inv("inv19", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_FALSE(inv.requiresMaintenance());
}

TEST_F(InventoryTest, CalculateDepreciationNonConsumable) {
    Inventory inv("inv20", property, "Test", "Cat", 10, 5, 100.0, pastTime,
                  "Loc", "Good", false, "Sup");
    
    double depreciation = inv.calculateDepreciation();
   
    EXPECT_NEAR(depreciation, 5.0, 0.01);
}

TEST_F(InventoryTest, CalculateDepreciationConsumable) {
    Inventory inv("inv21", property, "Test", "Cat", 10, 5, 100.0, pastTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_DOUBLE_EQ(inv.calculateDepreciation(), 0.0);
}

TEST_F(InventoryTest, IsCriticalItemTrue) {
    std::vector<std::string> criticalCategories = {"safety", "security", "electrical", "plumbing"};
    
    for (const auto& category : criticalCategories) {
        Inventory inv("inv22", property, "Test", category, 10, 5, 1.0, currentTime,
                      "Loc", "Good", true, "Sup");
        EXPECT_TRUE(inv.isCriticalItem()) << "Category: " << category << " should be critical";
    }
}

TEST_F(InventoryTest, IsCriticalItemFalse) {
    Inventory inv("inv23", property, "Test", "furniture", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_FALSE(inv.isCriticalItem());
}

TEST_F(InventoryTest, CalculateReplacementCost) {
    Inventory inv("inv24", property, "Test", "Cat", 3, 10, 5.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_DOUBLE_EQ(inv.calculateReplacementCost(), 35.0);
}

TEST_F(InventoryTest, CalculateReplacementCostNoReplacementNeeded) {
    Inventory inv("inv25", property, "Test", "Cat", 15, 10, 5.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    double cost = inv.calculateReplacementCost();
    EXPECT_DOUBLE_EQ(cost, -25.0); 
}

TEST_F(InventoryTest, GetInventoryStatusOutOfStock) {
    Inventory inv("inv26", property, "Test", "Cat", 0, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_EQ(inv.getInventoryStatus(), "out_of_stock");
}

TEST_F(InventoryTest, GetInventoryStatusLowStock) {
    Inventory inv("inv27", property, "Test", "Cat", 3, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_EQ(inv.getInventoryStatus(), "low_stock"); 
}

TEST_F(InventoryTest, GetInventoryStatusAdequate) {
    Inventory inv("inv28", property, "Test", "Cat", 6, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    EXPECT_EQ(inv.getInventoryStatus(), "adequate"); 
}

TEST_F(InventoryTest, GetInventoryStatusWellStocked) {
    Inventory inv("inv29", property, "Test", "Cat", 10, 5, 1.0, currentTime,
                  "Loc", "Good", true, "Sup");
    
    EXPECT_EQ(inv.getInventoryStatus(), "well_stocked"); 
}

TEST_F(InventoryTest, CalculateMonthsSinceLastRestock) {
    auto specificPast = currentTime - std::chrono::hours(24 * 30 * 6); 
    Inventory inv("inv30", property, "Test", "Cat", 10, 5, 1.0, specificPast,
                  "Loc", "Good", true, "Sup");
    
    int months = inv.calculateMonthsSinceLastRestock(currentTime);
    EXPECT_EQ(months, 6);
}

TEST_F(InventoryTest, EdgeCases) {
    Inventory inv1("inv31", property, "Test", "Cat", 0, 0, 0.0, currentTime,
                   "Loc", "Good", true, "Sup");
    
    EXPECT_TRUE(inv1.needsRestocking());
    EXPECT_DOUBLE_EQ(inv1.calculateTotalValue(), 0.0);
    EXPECT_EQ(inv1.getInventoryStatus(), "out_of_stock");
    
    Inventory inv2("inv32", property, "Test", "Cat", 10, -5, 1.0, currentTime,
                   "Loc", "Good", true, "Sup");
    
    inv2.setQuantity(-10);
    EXPECT_EQ(inv2.getQuantity(), 0);
}
