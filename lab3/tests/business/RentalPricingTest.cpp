#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/RentalPricing.h"
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class RentalPricingTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    RentalPricing pricing{10.0, 1.2, 1.5, 1.0, 1.1, 12, "standard"};

    void SetUp() override {
        auto address = std::make_unique<Address>(
            "Main St", "City", "State", "00000"
        );

        auto status = std::make_unique<PropertyStatus>(
            "AVAILABLE",
            "Available",
            true,
            false,
            false,
            std::chrono::system_clock::now(),
            "",
            0.0,
            0
        );

        auto type = std::make_unique<PropertyType>(
            "APT",
            "residential",
            "Apartment unit",
            0,
            5,
            20.0,
            500.0,
            false,
            "residential",
            1.5
        );

        property = std::make_shared<Property>(
            "P1",
            100.0,
            1000.0,
            std::move(address),
            std::move(status),
            std::move(type)
        );
    }
};


TEST_F(RentalPricingTest, TestGetters) {
    EXPECT_DOUBLE_EQ(pricing.getBasePricePerSqFt(), 10.0);
    EXPECT_DOUBLE_EQ(pricing.getLocationMultiplier(), 1.2);
    EXPECT_DOUBLE_EQ(pricing.getAmenityMultiplier(), 1.5);
    EXPECT_DOUBLE_EQ(pricing.getMarketDemandFactor(), 1.0);
    EXPECT_DOUBLE_EQ(pricing.getSeasonalityAdjustment(), 1.1);
    EXPECT_EQ(pricing.getMinimumLeaseTerm(), 12);
    EXPECT_EQ(pricing.getPricingStrategy(), "standard");
}

TEST_F(RentalPricingTest, TestSetters) {
    pricing.setBasePricePerSqFt(15.0);
    EXPECT_DOUBLE_EQ(pricing.getBasePricePerSqFt(), 15.0);

    pricing.setLocationMultiplier(2.0);
    EXPECT_DOUBLE_EQ(pricing.getLocationMultiplier(), 2.0);

    pricing.setMarketDemandFactor(1.5);
    EXPECT_DOUBLE_EQ(pricing.getMarketDemandFactor(), 1.5);

    pricing.setSeasonalityAdjustment(1.2);
    EXPECT_DOUBLE_EQ(pricing.getSeasonalityAdjustment(), 1.2);
}

TEST_F(RentalPricingTest, TestCalculateBaseRent) {
    EXPECT_DOUBLE_EQ(pricing.calculateBaseRent(property), 100.0 * 10.0);
    EXPECT_DOUBLE_EQ(pricing.calculateBaseRent(nullptr), 0.0);
}

TEST_F(RentalPricingTest, TestMarketAdjustedRent) {
    double baseRent = pricing.calculateBaseRent(property);

    EXPECT_GT(pricing.calculateMarketAdjustedRent(baseRent, "premium"), baseRent);
    EXPECT_GT(pricing.calculateMarketAdjustedRent(baseRent, "downtown"), baseRent);

    EXPECT_DOUBLE_EQ(pricing.calculateMarketAdjustedRent(baseRent, "suburban"),
                     baseRent * 1.2);

    EXPECT_LT(pricing.calculateMarketAdjustedRent(baseRent, "rural"), baseRent);
}

TEST_F(RentalPricingTest, TestAmenityPremium) {
    EXPECT_DOUBLE_EQ(
        pricing.calculateAmenityPremium(property),
        pricing.calculateBaseRent(property) * 1.5 * 0.1
    );
    EXPECT_DOUBLE_EQ(pricing.calculateAmenityPremium(nullptr), 0.0);
}

TEST_F(RentalPricingTest, TestLocationPremium) {
    EXPECT_DOUBLE_EQ(pricing.calculateLocationPremium("premium"), 0.25);
    EXPECT_DOUBLE_EQ(pricing.calculateLocationPremium("standard"), 0.0);
    EXPECT_DOUBLE_EQ(pricing.calculateLocationPremium("economy"), -0.15);
    EXPECT_DOUBLE_EQ(pricing.calculateLocationPremium("unknown"), 0.0);
}

TEST_F(RentalPricingTest, TestSeasonalAdjustment) {
    EXPECT_DOUBLE_EQ(pricing.calculateSeasonalAdjustment(6), 1.1);
    EXPECT_DOUBLE_EQ(pricing.calculateSeasonalAdjustment(12), 0.9);
    EXPECT_DOUBLE_EQ(pricing.calculateSeasonalAdjustment(3), 1.0);  
    EXPECT_DOUBLE_EQ(pricing.calculateSeasonalAdjustment(4), 1.0);
}

TEST_F(RentalPricingTest, TestLongTermDiscount) {
    EXPECT_DOUBLE_EQ(pricing.calculateLongTermDiscount(24), 0.9);
    EXPECT_DOUBLE_EQ(pricing.calculateLongTermDiscount(12), 0.95);
    EXPECT_DOUBLE_EQ(pricing.calculateLongTermDiscount(6), 1.0);
}

TEST_F(RentalPricingTest, TestIsCompetitivelyPriced) {
    EXPECT_TRUE(pricing.isCompetitivelyPriced(100, 100));
    EXPECT_TRUE(pricing.isCompetitivelyPriced(90, 100));
    EXPECT_TRUE(pricing.isCompetitivelyPriced(110, 100));
    EXPECT_FALSE(pricing.isCompetitivelyPriced(89, 100));
    EXPECT_FALSE(pricing.isCompetitivelyPriced(111, 100));
}

TEST_F(RentalPricingTest, TestOptimalRent) {
    double marketAvg = 2000.0;
    double optimal = pricing.calculateOptimalRent(property, marketAvg);

    EXPECT_GT(optimal, 0.0);
    EXPECT_DOUBLE_EQ(pricing.calculateOptimalRent(nullptr, marketAvg), 0.0);
}

TEST_F(RentalPricingTest, TestPriceSensitivity) {
    EXPECT_DOUBLE_EQ(pricing.calculatePriceSensitivity(100.0, 110.0), 10.0);
    EXPECT_DOUBLE_EQ(pricing.calculatePriceSensitivity(0.0, 100.0), 0.0);
}


TEST_F(RentalPricingTest, TestPricingRecommendation) {
    auto cheapProp = std::make_shared<Property>(
        "P2",
        50.0,
        50.0,
        std::make_unique<Address>("A", "B", "C", "00000"),
        std::make_unique<PropertyStatus>(
            "AVAILABLE", "Available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        ),
        std::make_unique<PropertyType>(
            "STUDIO", "residential", "Studio", 0, 2, 10.0, 200.0, false, "residential", 1.0
        )
    );

    EXPECT_EQ(pricing.getPricingRecommendation(cheapProp), "economy_pricing");

    auto midProp = std::make_shared<Property>(
        "P3",
        200.0,
        2000.0,
        std::make_unique<Address>("A", "B", "C", "00000"),
        std::make_unique<PropertyStatus>(
            "AVAILABLE", "Available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        ),
        std::make_unique<PropertyType>(
            "APT", "residential", "Apartment", 1, 4, 20.0, 500.0, false, "residential", 1.3
        )
    );

    EXPECT_EQ(pricing.getPricingRecommendation(midProp), "mid_range_pricing");

    auto premiumProp = std::make_shared<Property>(
        "P4",
        600.0,  
        6000.0,
        std::make_unique<Address>("A", "B", "C", "00000"),
        std::make_unique<PropertyStatus>(
            "AVAILABLE", "Available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        ),
        std::make_unique<PropertyType>(
            "LUX", "residential", "Luxury", 2, 10, 50.0, 5000.0, true, "residential", 2.0
        )
    );

    EXPECT_EQ(pricing.getPricingRecommendation(premiumProp), "premium_pricing");

    EXPECT_EQ(pricing.getPricingRecommendation(nullptr), "insufficient_data");
}