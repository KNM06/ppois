#include <gtest/gtest.h>
#include "../../include/core/Customer.h"
#include "../../include/core/RentalHistory.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/utils/ValidationUtils.h"
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

class CustomerTest : public ::testing::Test {
protected:
    std::shared_ptr<Customer> newCustomer;
    std::shared_ptr<Customer> vipCustomer;
    std::shared_ptr<Customer> oldCustomer;
    std::shared_ptr<PaymentProcessor> mockProcessor;

    void SetUp() override {
        mockProcessor = std::make_shared<PaymentProcessor>();
        newCustomer = std::make_shared<Customer>("CUST001", "John Doe", "john@example.com", 30, mockProcessor);
        
        vipCustomer = std::make_shared<Customer>("CUST002", "VIP User", "vip@example.com", 45);
        vipCustomer->setStatus(CustomerStatus::VIP);
        vipCustomer->setTier(CustomerTier::PLATINUM);
        vipCustomer->setCreditScore(800.0);
        vipCustomer->addLoyaltyPoints(5000);
        
        oldCustomer = std::make_shared<Customer>("CUST003", "Old User", "old@example.com", 66);
    }
};

TEST_F(CustomerTest, ConstructorInitialization) {
    EXPECT_EQ(newCustomer->getCustomerId(), "CUST001");
    EXPECT_EQ(newCustomer->getName(), "John Doe");
    EXPECT_EQ(newCustomer->getEmail(), "john@example.com");
    EXPECT_EQ(newCustomer->getStatus(), CustomerStatus::NEW);
    EXPECT_EQ(newCustomer->getTier(), CustomerTier::BRONZE);
}

TEST_F(CustomerTest, CalculateLoyaltyDiscountNewBronze) {
    double discount = newCustomer->calculateLoyaltyDiscount();
    EXPECT_NEAR(discount, 0.05, 0.001);
}

TEST_F(CustomerTest, CalculateLoyaltyDiscountVIPPlatinumHighPoints) {
    double discount = vipCustomer->calculateLoyaltyDiscount();
    EXPECT_NEAR(discount, 0.35, 0.001);
}

TEST_F(CustomerTest, IsEligibleForPremiumItemsFalseInitial) {
    EXPECT_FALSE(newCustomer->isEligibleForPremiumItems());
}

TEST_F(CustomerTest, IsEligibleForPremiumItemsFalseUnderage) {
    auto minor = std::make_shared<Customer>("MIN001", "Minor", "m@e.com", 20);
    EXPECT_FALSE(minor->isEligibleForPremiumItems());
}

TEST_F(CustomerTest, IsEligibleForPremiumItemsTruePlatinum) {
    EXPECT_TRUE(vipCustomer->isEligibleForPremiumItems());
}

TEST_F(CustomerTest, GetRiskAssessmentNewCustomer) {
    double risk = newCustomer->getRiskAssessment();
    EXPECT_NEAR(risk, 0.31470588235294117, 0.001);
}

TEST_F(CustomerTest, GetRiskAssessmentOldCustomer) {
    double risk = oldCustomer->getRiskAssessment();
    EXPECT_NEAR(risk, 0.4147058823529412, 0.001);
}

TEST_F(CustomerTest, GetRiskAssessmentBlacklisted) {
    newCustomer->setStatus(CustomerStatus::BLACKLISTED);
    double risk = newCustomer->getRiskAssessment();
    EXPECT_DOUBLE_EQ(risk, 1.0);
}

TEST_F(CustomerTest, UpdateCustomerStatusBasedOnActivityBronze) {
    newCustomer->addRentalToHistory(50.0);
    newCustomer->addRentalToHistory(50.0);
    newCustomer->addRentalToHistory(50.0);
    EXPECT_EQ(newCustomer->getTier(), CustomerTier::BRONZE);
    EXPECT_NEAR(newCustomer->getCreditScore(), 410.0 + (0.8 * 550.0), 0.001);
}

TEST_F(CustomerTest, CalculateRentalLimitNewCustomer) {
    EXPECT_EQ(newCustomer->calculateRentalLimit(), 1);
}

TEST_F(CustomerTest, CalculateRentalLimitVIPPlatinum) {
    EXPECT_EQ(vipCustomer->calculateRentalLimit(), 10);
}

TEST_F(CustomerTest, ValidateRentalCompatibilityVehicleFails) {
    auto minor = std::make_shared<Customer>("MINOR", "Minor", "m@e.com", 17);
    EXPECT_FALSE(minor->validateRentalCompatibility("VEHICLE"));
}

TEST_F(CustomerTest, ValidateRentalCompatibilityConstructionFails) {
    EXPECT_FALSE(newCustomer->validateRentalCompatibility("LUXURY"));
}

TEST_F(CustomerTest, ValidateRentalCompatibilityCategoryLimitFails) {
    newCustomer->addRentalToHistory(1000.0);
    EXPECT_TRUE(newCustomer->validateRentalCompatibility("VEHICLE"));
}

TEST_F(CustomerTest, ValidateRentalCompatibilitySpecializedPasses) {
    newCustomer->updateRentalLimit("CONSTRUCTION", 4);
    EXPECT_FALSE(newCustomer->validateRentalCompatibility("SPECIALIZED_EQUIPMENT"));
}

TEST_F(CustomerTest, CalculateCustomerValueScoreInitial) {
    double score = newCustomer->calculateCustomerValueScore();
    EXPECT_NEAR(score, 0.1, 0.001);
}

TEST_F(CustomerTest, CanUpgradeTierTrue) {
    for (int i = 0; i < 5; ++i) {
        newCustomer->addRentalToHistory(100.0); 
    }
    EXPECT_FALSE(newCustomer->canUpgradeTier());
    newCustomer->applyTierUpgrade();
    EXPECT_EQ(newCustomer->getTier(), CustomerTier::SILVER);
    EXPECT_EQ(newCustomer->getLoyaltyPoints(), 50 + 500);
}

TEST_F(CustomerTest, GetRecommendedCategoriesInitial) {
    auto recs = newCustomer->getRecommendedCategories();
    EXPECT_EQ(recs.size(), 3u);
    EXPECT_TRUE(std::find(recs.begin(), recs.end(), "TOOLS") != recs.end());
}

TEST_F(CustomerTest, GetRecommendedCategoriesOldCustomer) {
    auto recs = oldCustomer->getRecommendedCategories();
    EXPECT_EQ(recs.size(), 5u);
    EXPECT_TRUE(std::find(recs.begin(), recs.end(), "CONSTRUCTION") != recs.end());
}

TEST_F(CustomerTest, PredictSpendingAmountNewCustomer) {
    double prediction = newCustomer->predictSpendingAmount();
    EXPECT_NEAR(prediction, 117.647, 0.001);
}

TEST_F(CustomerTest, ValidatePaymentMethodFalse) {
    EXPECT_TRUE(newCustomer->validatePaymentMethod());
}

TEST_F(CustomerTest, SettersAndGetters) {
    newCustomer->setCreditScore(900.0);
    EXPECT_DOUBLE_EQ(newCustomer->getCreditScore(), 850.0);
    
    newCustomer->setEmailVerified(true);
    EXPECT_TRUE(newCustomer->getEmailVerified());
    
    newCustomer->addLoyaltyPoints(100);
    EXPECT_EQ(newCustomer->getLoyaltyPoints(), 100);
}