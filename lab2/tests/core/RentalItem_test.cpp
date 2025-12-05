#include <gtest/gtest.h>
#include "../../include/core/RentalItem.h"

class RentalItemTest : public ::testing::Test {
protected:
    void SetUp() override {
        rentalItem = std::make_shared<RentalItem>("ITEM001", "Test Tool", "TOOLS", 25.0);
    }
    
    std::shared_ptr<RentalItem> rentalItem;
};

TEST_F(RentalItemTest, ConstructorInitialization) {
    EXPECT_EQ(rentalItem->getItemId(), "ITEM001");
    EXPECT_EQ(rentalItem->getName(), "Test Tool");
    EXPECT_EQ(rentalItem->getCategory(), "TOOLS");
    EXPECT_TRUE(rentalItem->getIsAvailable());
}

TEST_F(RentalItemTest, CalculateRentalPrice) {
    double price = rentalItem->calculateRentalPrice(7, "REGULAR");
    EXPECT_GT(price, 0.0);
}

TEST_F(RentalItemTest, RequiresMaintenance) {
    bool needsMaintenance = rentalItem->requiresMaintenance();
    EXPECT_FALSE(needsMaintenance);
}

TEST_F(RentalItemTest, CalculateDepreciation) {
    double depreciation = rentalItem->calculateDepreciation();
    EXPECT_GE(depreciation, 0.0);
}

TEST_F(RentalItemTest, CalculateInsurancePremium) {
    double premium = rentalItem->calculateInsurancePremium();
    EXPECT_GE(premium, 0.0);
}

TEST_F(RentalItemTest, GetRemainingUsefulLife) {
    int life = rentalItem->getRemainingUsefulLife();
    EXPECT_GE(life, 0);
}

TEST_F(RentalItemTest, ValidateRentalDuration) {
    bool valid = rentalItem->validateRentalDuration(7);
    EXPECT_TRUE(valid);
    
    bool invalid = rentalItem->validateRentalDuration(400);
    EXPECT_FALSE(invalid);
}

TEST_F(RentalItemTest, MarkAsRentedAndReturned) {
    rentalItem->markAsRented();
    EXPECT_FALSE(rentalItem->getIsAvailable());
    
    rentalItem->markAsReturned();
    EXPECT_TRUE(rentalItem->getIsAvailable());
}

TEST_F(RentalItemTest, UpdateMaintenanceStatus) {
    EXPECT_NO_THROW(rentalItem->updateMaintenanceStatus(30));
}

TEST_F(RentalItemTest, SetAndGetCondition) {
    rentalItem->setCondition(ItemCondition::GOOD);
    EXPECT_EQ(rentalItem->getCondition(), ItemCondition::GOOD);
    
    rentalItem->setCondition(ItemCondition::POOR);
    EXPECT_EQ(rentalItem->getCondition(), ItemCondition::POOR);
}