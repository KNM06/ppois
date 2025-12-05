#include <gtest/gtest.h>
#include "../../include/business/DiscountCalculator.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalPricing.h"
#include "../../include/utils/DateUtils.h"
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

class DiscountCalculatorTest : public ::testing::Test {
protected:
    std::shared_ptr<PricingStrategy> pricingStrategy;
    std::shared_ptr<DiscountCalculator> discountCalculator;

    void SetUp() override {
        pricingStrategy = std::make_shared<PricingStrategy>();
        discountCalculator = std::make_shared<DiscountCalculator>(pricingStrategy);
    }
};

TEST_F(DiscountCalculatorTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(discountCalculator->getPricingStrategy(), pricingStrategy);
}

TEST_F(DiscountCalculatorTest, CalculateSeasonalDiscountReturnsBaseDiscountForKnownSeason) {
    double discount = discountCalculator->calculateSeasonalDiscount("WINTER", "GENERAL");
    EXPECT_DOUBLE_EQ(discount, 0.15);
}

TEST_F(DiscountCalculatorTest, CalculateSeasonalDiscountReturnsZeroForUnknownSeason) {
    double unknownDiscount = discountCalculator->calculateSeasonalDiscount("APOCALYPSE", "GENERAL");
    EXPECT_DOUBLE_EQ(unknownDiscount, 0.0);
}

TEST_F(DiscountCalculatorTest, CalculateSeasonalDiscountAppliesWINTER_SPORTSMultiplier) {
    double winterSportsDiscount = discountCalculator->calculateSeasonalDiscount("WINTER", "WINTER_SPORTS");
    EXPECT_DOUBLE_EQ(winterSportsDiscount, 0.075);
}

TEST_F(DiscountCalculatorTest, CalculateSeasonalDiscountAppliesGARDENMultiplier) {
    double gardenDiscount = discountCalculator->calculateSeasonalDiscount("SPRING", "GARDEN");
    EXPECT_DOUBLE_EQ(gardenDiscount, 0.07);
}

TEST_F(DiscountCalculatorTest, CalculateSeasonalDiscountAppliesOFF_SEASONMultiplier) {
    double offSeasonDiscount = discountCalculator->calculateSeasonalDiscount("SUMMER", "OFF_SEASON");
    EXPECT_DOUBLE_EQ(offSeasonDiscount, 0.075);
}

TEST_F(DiscountCalculatorTest, CalculateMembershipDiscountForPREMIUMWithHighLoyalty) {
    double discount = discountCalculator->calculateMembershipDiscount("PREMIUM", 2000);
    EXPECT_NEAR(discount, 0.25, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateMembershipDiscountForBASICWithNoLoyalty) {
    double discount = discountCalculator->calculateMembershipDiscount("BASIC", 0);
    EXPECT_NEAR(discount, 0.05, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateMembershipDiscountClampsMaxDiscount) {
    double discount = discountCalculator->calculateMembershipDiscount("BUSINESS", 1000);
    EXPECT_NEAR(discount, 0.3, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateMembershipDiscountForUnknownType) {
    double discount = discountCalculator->calculateMembershipDiscount("UNKNOWN", 500);
    EXPECT_NEAR(discount, 0.05, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBulkDiscountForItemCount20AndHighAmount) {
    double discount = discountCalculator->calculateBulkDiscount(20, 1001.0);
    EXPECT_NEAR(discount, 0.30, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBulkDiscountForItemCount5AndMediumAmount) {
    double discount = discountCalculator->calculateBulkDiscount(5, 500.0);
    EXPECT_NEAR(discount, 0.12, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBulkDiscountForItemCount3AndLowAmount) {
    double discount = discountCalculator->calculateBulkDiscount(3, 199.0);
    EXPECT_NEAR(discount, 0.05, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBulkDiscountForItemCount0) {
    double discount = discountCalculator->calculateBulkDiscount(0, 1000.0);
    EXPECT_NEAR(discount, 0.03, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBulkDiscountClampsMaxDiscount) {
    double discount = discountCalculator->calculateBulkDiscount(25, 2000.0);
    EXPECT_LE(discount, 0.40);
}

TEST_F(DiscountCalculatorTest, CalculateEarlyBookingDiscountFor30Days) {
    double discount = discountCalculator->calculateEarlyBookingDiscount(30);
    EXPECT_DOUBLE_EQ(discount, 0.20);
}

TEST_F(DiscountCalculatorTest, CalculateEarlyBookingDiscountFor14Days) {
    double discount = discountCalculator->calculateEarlyBookingDiscount(14);
    EXPECT_DOUBLE_EQ(discount, 0.15);
}

TEST_F(DiscountCalculatorTest, CalculateEarlyBookingDiscountFor3Days) {
    double discount = discountCalculator->calculateEarlyBookingDiscount(3);
    EXPECT_DOUBLE_EQ(discount, 0.05);
}

TEST_F(DiscountCalculatorTest, CalculateEarlyBookingDiscountFor0Days) {
    double discount = discountCalculator->calculateEarlyBookingDiscount(0);
    EXPECT_DOUBLE_EQ(discount, 0.0);
}

TEST_F(DiscountCalculatorTest, CalculateLongTermDiscountFor90Days) {
    double discount = discountCalculator->calculateLongTermDiscount(90);
    EXPECT_DOUBLE_EQ(discount, 0.35);
}

TEST_F(DiscountCalculatorTest, CalculateLongTermDiscountFor30Days) {
    double discount = discountCalculator->calculateLongTermDiscount(30);
    EXPECT_DOUBLE_EQ(discount, 0.15);
}

TEST_F(DiscountCalculatorTest, CalculateLongTermDiscountFor7Days) {
    double discount = discountCalculator->calculateLongTermDiscount(7);
    EXPECT_DOUBLE_EQ(discount, 0.05);
}

TEST_F(DiscountCalculatorTest, CalculateLongTermDiscountFor6Days) {
    double discount = discountCalculator->calculateLongTermDiscount(6);
    EXPECT_DOUBLE_EQ(discount, 0.0);
}

TEST_F(DiscountCalculatorTest, CalculateBundleDiscountForTwoCompatibleCategories) {
    std::vector<std::string> categories = {"CAMERA", "TRIPOD"};
    double discount = discountCalculator->calculateBundleDiscount(categories);
    EXPECT_NEAR(discount, 0.07, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBundleDiscountForFiveCategories) {
    std::vector<std::string> categories = {"CAMERA", "TRIPOD", "CONSTRUCTION", "TOOLS", "SPORTS"};
    double discount = discountCalculator->calculateBundleDiscount(categories);
    EXPECT_NEAR(discount, 0.17, 0.001);
}

TEST_F(DiscountCalculatorTest, CalculateBundleDiscountForEmptyCategories) {
    std::vector<std::string> categories = {};
    double discount = discountCalculator->calculateBundleDiscount(categories);
    EXPECT_DOUBLE_EQ(discount, 0.0);
}

TEST_F(DiscountCalculatorTest, IsPromotionApplicableForSUMMERCode) {
    EXPECT_TRUE(discountCalculator->isPromotionApplicable("SUMMER25", 100.0));
}

TEST_F(DiscountCalculatorTest, IsPromotionApplicableForVIPCodeWithHighAmount) {
    EXPECT_TRUE(discountCalculator->isPromotionApplicable("VIPDISCOUNT", 101.0));
}

TEST_F(DiscountCalculatorTest, IsPromotionApplicableForGenericPromoWithLowAmount) {
    EXPECT_FALSE(discountCalculator->isPromotionApplicable("WELCOME10", 49.0));
}

TEST_F(DiscountCalculatorTest, IsPromotionApplicableForUnknownPromoCodeLength) {
    EXPECT_TRUE(discountCalculator->isPromotionApplicable("LONGCODE", 100.0));
    EXPECT_FALSE(discountCalculator->isPromotionApplicable("SHORT", 100.0));
}

TEST_F(DiscountCalculatorTest, CalculateReferralDiscountFor10Referrals) {
    double discount = discountCalculator->calculateReferralDiscount(10);
    EXPECT_DOUBLE_EQ(discount, 0.20);
}

TEST_F(DiscountCalculatorTest, CalculateReferralDiscountFor1Referral) {
    double discount = discountCalculator->calculateReferralDiscount(1);
    EXPECT_DOUBLE_EQ(discount, 0.05);
}

TEST_F(DiscountCalculatorTest, UpdateSeasonalDiscountClampsValue) {
    discountCalculator->updateSeasonalDiscount("WINTER", 0.6);
    EXPECT_DOUBLE_EQ(discountCalculator->calculateSeasonalDiscount("WINTER", "GENERAL"), 0.5);
    
    discountCalculator->updateSeasonalDiscount("WINTER", -0.1);
    EXPECT_DOUBLE_EQ(discountCalculator->calculateSeasonalDiscount("WINTER", "GENERAL"), 0.0);
}

TEST_F(DiscountCalculatorTest, AddPromotionalDiscountClampsValue) {
    discountCalculator->addPromotionalDiscount("NEWCODE", 1.0);
    auto discounts = discountCalculator->getAvailableDiscounts("REGULAR");
    EXPECT_DOUBLE_EQ(discounts.at("NEWCODE"), 0.5);
}

TEST_F(DiscountCalculatorTest, SetAndGetPricingStrategy) {
    auto newPricingStrategy = std::make_shared<PricingStrategy>();
    discountCalculator->setPricingStrategy(newPricingStrategy);
    EXPECT_EQ(discountCalculator->getPricingStrategy(), newPricingStrategy);
}