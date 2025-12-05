#include <gtest/gtest.h>
#include "../../include/business/CustomerManager.h"
#include "../../include/core/Customer.h"
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/LateFeeCalculator.h"

class CustomerManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto recommendationEngine = std::make_shared<RecommendationEngine>();
        auto lateFeeCalculator = std::make_shared<LateFeeCalculator>();
        customerManager = std::make_shared<CustomerManager>(recommendationEngine, lateFeeCalculator);
        
        auto customer = std::make_shared<Customer>("CUST001", "John Doe", "john@example.com", 30);
        customerManager->addCustomer(customer);
    }
    
    std::shared_ptr<CustomerManager> customerManager;
};

TEST_F(CustomerManagerTest, IsCustomerEligible) {
    bool eligible = customerManager->isCustomerEligible("CUST001");
    EXPECT_TRUE(eligible);
}

TEST_F(CustomerManagerTest, AddToBlacklist) {
    customerManager->addToBlacklist("CUST001");
    bool eligible = customerManager->isCustomerEligible("CUST001");
    EXPECT_FALSE(eligible);
}

TEST_F(CustomerManagerTest, RemoveFromBlacklist) {
    customerManager->addToBlacklist("CUST001");
    customerManager->removeFromBlacklist("CUST001");
    bool eligible = customerManager->isCustomerEligible("CUST001");
    EXPECT_TRUE(eligible);
}

TEST_F(CustomerManagerTest, CalculateCustomerLifetimeValue) {
    double ltv = customerManager->calculateCustomerLifetimeValue("CUST001");
    EXPECT_GE(ltv, 0.0);
}

TEST_F(CustomerManagerTest, GetVIPCustomers) {
    auto vipCustomers = customerManager->getVIPCustomers();
    EXPECT_TRUE(vipCustomers.empty());
}

TEST_F(CustomerManagerTest, PredictCustomerRetention) {
    double retention = customerManager->predictCustomerRetention("CUST001");
    EXPECT_GE(retention, 0.0);
    EXPECT_LE(retention, 1.0);
}

TEST_F(CustomerManagerTest, CalculateCustomerRiskScore) {
    double risk = customerManager->calculateCustomerRiskScore("CUST001");
    EXPECT_GE(risk, 0.0);
    EXPECT_LE(risk, 1.0);
}

TEST_F(CustomerManagerTest, GetCustomersForPromotion) {
    auto customers = customerManager->getCustomersForPromotion();
    EXPECT_TRUE(customers.empty());
}

TEST_F(CustomerManagerTest, ApplyLoyaltyBonus) {
    EXPECT_NO_THROW(customerManager->applyLoyaltyBonus("CUST001", 100.0));
}

TEST_F(CustomerManagerTest, GetCustomer) {
    auto customer = customerManager->getCustomer("CUST001");
    EXPECT_NE(customer, nullptr);
    EXPECT_EQ(customer->getCustomerId(), "CUST001");
}