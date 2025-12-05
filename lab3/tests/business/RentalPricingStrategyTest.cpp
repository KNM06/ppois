#include <gtest/gtest.h>
#include <memory>
#include "../../include/business/RentalPricingStrategy.h"
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class RentalPricingStrategyTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    RentalPricingStrategy strategy{"S1", "StandardStrategy", "Test strategy",
                                   10.0, 1.2, 1.5, 1.0, 12, true};

    void SetUp() override {
        auto address = std::make_unique<Address>("123 Main St", "City", "State", "12345");
        auto status = std::make_unique<PropertyStatus>(
            "AVAILABLE", "Property is available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        );
        auto type = std::make_unique<PropertyType>(
            "APT", "residential", "Apartment", 1, 5, 50.0, 200.0, false, "residential", 1.5
        );

        property = std::make_shared<Property>("P1", 100.0, 2000.0,
                                              std::move(address),
                                              std::move(status),
                                              std::move(type));
    }
};

TEST_F(RentalPricingStrategyTest, TestGetters) {
    EXPECT_EQ(strategy.getStrategyId(), "S1");
    EXPECT_EQ(strategy.getStrategyName(), "StandardStrategy");
    EXPECT_EQ(strategy.getDescription(), "Test strategy");
    EXPECT_DOUBLE_EQ(strategy.getBaseMultiplier(), 10.0);
    EXPECT_DOUBLE_EQ(strategy.getLocationFactor(), 1.2);
    EXPECT_DOUBLE_EQ(strategy.getAmenityFactor(), 1.5);
    EXPECT_DOUBLE_EQ(strategy.getMarketAdjustment(), 1.0);
    EXPECT_EQ(strategy.getMinimumLeaseTerm(), 12);
    EXPECT_TRUE(strategy.getDynamicPricingEnabled());
}

TEST_F(RentalPricingStrategyTest, TestSetters) {
    strategy.setBaseMultiplier(15.0);
    EXPECT_DOUBLE_EQ(strategy.getBaseMultiplier(), 15.0);

    strategy.setLocationFactor(2.0);
    EXPECT_DOUBLE_EQ(strategy.getLocationFactor(), 2.0);

    strategy.setMarketAdjustment(1.2);
    EXPECT_DOUBLE_EQ(strategy.getMarketAdjustment(), 1.2);
}

TEST_F(RentalPricingStrategyTest, TestCalculateRentalPrice) {
    double price = strategy.calculateRentalPrice(property);
    EXPECT_GT(price, 0.0);
    EXPECT_DOUBLE_EQ(strategy.calculateRentalPrice(nullptr), 0.0);
}

TEST_F(RentalPricingStrategyTest, TestLocationPremium) {
    EXPECT_DOUBLE_EQ(strategy.calculateLocationPremium("premium"), 1.5);
    EXPECT_DOUBLE_EQ(strategy.calculateLocationPremium("standard"), 1.0);
    EXPECT_DOUBLE_EQ(strategy.calculateLocationPremium("economy"), 0.7);
    EXPECT_DOUBLE_EQ(strategy.calculateLocationPremium("unknown"), 1.0);
}

TEST_F(RentalPricingStrategyTest, TestAmenityValue) {
    double amenity = strategy.calculateAmenityValue(property);
    EXPECT_DOUBLE_EQ(amenity, property->getRentalPrice() * 0.1 * 1.5);
    EXPECT_DOUBLE_EQ(strategy.calculateAmenityValue(nullptr), 0.0);
}

TEST_F(RentalPricingStrategyTest, TestApplyMarketConditions) {
    double base = 1000.0;
    EXPECT_DOUBLE_EQ(strategy.applyMarketConditions(base, "hot"), 1200.0);
    EXPECT_DOUBLE_EQ(strategy.applyMarketConditions(base, "stable"), 1000.0);
    EXPECT_DOUBLE_EQ(strategy.applyMarketConditions(base, "slow"), 900.0);
    EXPECT_DOUBLE_EQ(strategy.applyMarketConditions(base, "other"), base * 1.0);
}

TEST_F(RentalPricingStrategyTest, TestIsPriceCompetitive) {
    EXPECT_TRUE(strategy.isPriceCompetitive(100, 100));
    EXPECT_TRUE(strategy.isPriceCompetitive(90, 100));
    EXPECT_TRUE(strategy.isPriceCompetitive(110, 100));
    EXPECT_FALSE(strategy.isPriceCompetitive(89, 100));
    EXPECT_FALSE(strategy.isPriceCompetitive(111, 100));
}

TEST_F(RentalPricingStrategyTest, TestOptimalPriceRange) {
    double range = strategy.calculateOptimalPriceRange(property);
    EXPECT_GT(range, 0.0);
    EXPECT_DOUBLE_EQ(strategy.calculateOptimalPriceRange(nullptr), 0.0);
}

TEST_F(RentalPricingStrategyTest, TestPricingTier) {
    EXPECT_EQ(strategy.getPricingTier(500), "economy");
    EXPECT_EQ(strategy.getPricingTier(2000), "mid_range");
    EXPECT_EQ(strategy.getPricingTier(5000), "premium");
    EXPECT_EQ(strategy.getPricingTier(10000), "luxury");
}

TEST_F(RentalPricingStrategyTest, TestShouldAdjustPrice) {
    EXPECT_TRUE(strategy.shouldAdjustPrice(120, 100));
    EXPECT_FALSE(strategy.shouldAdjustPrice(105, 100));

    RentalPricingStrategy staticStrategy{"S2", "Static", "No dynamic", 10, 1, 1, 1, 12, false};
    EXPECT_FALSE(staticStrategy.shouldAdjustPrice(120, 100));
}

TEST_F(RentalPricingStrategyTest, TestPriceSensitivity) {
    EXPECT_DOUBLE_EQ(strategy.calculatePriceSensitivity(0.1), 10.0);
    EXPECT_DOUBLE_EQ(strategy.calculatePriceSensitivity(-0.2), 20.0);
}
