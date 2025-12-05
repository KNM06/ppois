#include <gtest/gtest.h>
#include "../../include/core/RentalPricing.h"

class RentalPricingTest : public ::testing::Test {
protected:
    void SetUp() override {
        rentalPricing = std::make_shared<RentalPricing>();
    }
    
    std::shared_ptr<RentalPricing> rentalPricing;
};

TEST_F(RentalPricingTest, CalculateDynamicPrice) {
    double price = rentalPricing->calculateDynamicPrice("TOOLS", 7, "SUMMER", "REGULAR");
    EXPECT_GT(price, 0.0);
}

TEST_F(RentalPricingTest, ApplyLoyaltyDiscount) {
    double discounted = rentalPricing->applyLoyaltyDiscount(100.0, 500);
    EXPECT_GT(discounted, 0.0);
    EXPECT_LE(discounted, 100.0);
}

TEST_F(RentalPricingTest, CalculateBulkDiscount) {
    double discounted = rentalPricing->calculateBulkDiscount(5, 500.0);
    EXPECT_GT(discounted, 0.0);
}

TEST_F(RentalPricingTest, GetSeasonalMultiplier) {
    double multiplier = rentalPricing->getSeasonalMultiplier("SUMMER");
    EXPECT_GT(multiplier, 0.0);
}

TEST_F(RentalPricingTest, GetCustomerDiscount) {
    double discount = rentalPricing->getCustomerDiscount("VIP");
    EXPECT_GE(discount, 0.0);
}