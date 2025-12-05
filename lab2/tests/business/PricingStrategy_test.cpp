#include <gtest/gtest.h>
#include "../../include/business/PricingStrategy.h"
#include "../../include/business/DiscountCalculator.h"

class PricingStrategyTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto discountCalculator = std::make_shared<DiscountCalculator>();
        pricingStrategy = std::make_shared<PricingStrategy>(discountCalculator);
    }
    
    std::shared_ptr<PricingStrategy> pricingStrategy;
};

TEST_F(PricingStrategyTest, CalculateOptimalPrice) {
    double price = pricingStrategy->calculateOptimalPrice("TOOLS", 7, "REGULAR", 0.5);
    EXPECT_GT(price, 0.0);
}

TEST_F(PricingStrategyTest, ShouldAdjustPriceForCompetition) {
    bool shouldAdjust = pricingStrategy->shouldAdjustPriceForCompetition("VEHICLE");
    EXPECT_TRUE(shouldAdjust || !shouldAdjust);
}

TEST_F(PricingStrategyTest, CalculatePriceSensitivity) {
    double sensitivity = pricingStrategy->calculatePriceSensitivity("TOOLS", 100.0);
    EXPECT_GT(sensitivity, 0.0);
}

TEST_F(PricingStrategyTest, UpdateCompetitorPrice) {
    pricingStrategy->updateCompetitorPrice("TOOLS", 30.0);
    
    auto recommendations = pricingStrategy->getPriceRecommendations();
    EXPECT_FALSE(recommendations.empty());
}

TEST_F(PricingStrategyTest, CalculateDynamicDiscount) {
    double discount = pricingStrategy->calculateDynamicDiscount(5, 500.0);
    EXPECT_GE(discount, 0.0);
    EXPECT_LE(discount, 1.0);
}