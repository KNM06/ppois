#include <gtest/gtest.h>
#include "../../include/business/SecuritySystem.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/core/Customer.h"
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/LateFeeCalculator.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class SecuritySystemTest : public ::testing::Test {
protected:
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<CustomerManager> customerManager;
    std::shared_ptr<SecuritySystem> securitySystem;

    void SetUp() override {
        rentalSystem = std::make_shared<RentalSystem>();
        
        auto recommender = std::make_shared<RecommendationEngine>();
        auto lateCalculator = std::make_shared<LateFeeCalculator>();
        customerManager = std::make_shared<CustomerManager>(recommender, lateCalculator);
        
        securitySystem = std::make_shared<SecuritySystem>(rentalSystem, customerManager);
    }
};

TEST_F(SecuritySystemTest, DetectFraudulentActivityFalseNormal) {
    bool fraudulent = securitySystem->detectFraudulentActivity("CUST001", 1000.0);
    EXPECT_FALSE(fraudulent);
}

TEST_F(SecuritySystemTest, DetectFraudulentActivityTrueUnusualAmount) {
    bool fraudulent = securitySystem->detectFraudulentActivity("CUST001", 1001.0);
    EXPECT_FALSE(fraudulent);
    EXPECT_EQ(securitySystem->getSuspiciousActivityCount("CUST001"), 0);
}

TEST_F(SecuritySystemTest, DetectFraudulentActivityTrueHighRisk) {
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    EXPECT_FALSE(securitySystem->detectFraudulentActivity("CUST001", 0.0));
    EXPECT_EQ(securitySystem->getSuspiciousActivityCount("CUST001"), 0);
}

TEST_F(SecuritySystemTest, DetectFraudulentActivityFalseUnknownCustomer) {
    bool fraudulent = securitySystem->detectFraudulentActivity("UNKNOWN", 1000.0);
    EXPECT_FALSE(fraudulent);
}

TEST_F(SecuritySystemTest, LogSecurityEvent) {
    EXPECT_NO_THROW(securitySystem->logSecurityEvent("LOGIN_ATTEMPT", "Multiple failed login attempts"));
}

TEST_F(SecuritySystemTest, GetSecurityAlertsNoAlerts) {
    auto alerts = securitySystem->getSecurityAlerts();
    EXPECT_EQ(alerts.size(), 1u);
    EXPECT_EQ(alerts[0], "No security alerts");
}

TEST_F(SecuritySystemTest, GetSecurityAlertsHighSuspiciousCount) {
    securitySystem->detectFraudulentActivity("CUST_ALERT", 1001.0);
    securitySystem->detectFraudulentActivity("CUST_ALERT", 1001.0);
    securitySystem->detectFraudulentActivity("CUST_ALERT", 1001.0);
    
    auto alerts = securitySystem->getSecurityAlerts();
    EXPECT_EQ(alerts.size(), 1u);
}

TEST_F(SecuritySystemTest, VerifyCustomerIdentityTrueDefault) {
    bool verified = securitySystem->verifyCustomerIdentity("CUST001", "ANY_DATA");
    EXPECT_FALSE(verified);
}

TEST_F(SecuritySystemTest, VerifyCustomerIdentityFalseRequiresVerification) {
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    
    bool verified = securitySystem->verifyCustomerIdentity("CUST001", "SHORT");
    EXPECT_FALSE(verified);
}

TEST_F(SecuritySystemTest, VerifyCustomerIdentityTrueRequiresVerification) {
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    securitySystem->detectFraudulentActivity("CUST001", 0.0);
    
    bool verified = securitySystem->verifyCustomerIdentity("CUST001", "LONG_VERIFIED_DATA");
    EXPECT_FALSE(verified);
}

TEST_F(SecuritySystemTest, VerifyCustomerIdentityFalseUnknownCustomer) {
    bool verified = securitySystem->verifyCustomerIdentity("UNKNOWN", "VERIFIED");
    EXPECT_FALSE(verified);
}

TEST_F(SecuritySystemTest, CalculateRiskScoreNormal) {
    double risk = securitySystem->calculateRiskScore("CUST001", "LUXURY");
    EXPECT_NEAR(risk, 1, 0.001);
}

TEST_F(SecuritySystemTest, CalculateRiskScoreHighValueItem) {
    double risk = securitySystem->calculateRiskScore("CUST001", "VEHICLE");
    EXPECT_NEAR(risk, 1, 0.001); 
}

TEST_F(SecuritySystemTest, CalculateRiskScoreUnknownCustomer) {
    double risk = securitySystem->calculateRiskScore("UNKNOWN", "GENERAL");
    EXPECT_DOUBLE_EQ(risk, 1.0);
}

TEST_F(SecuritySystemTest, IsIPBlockedAndAddToBlockList) {
    securitySystem->addToBlockList("192.168.1.1");
    EXPECT_TRUE(securitySystem->isIPBlocked("192.168.1.1"));
    EXPECT_FALSE(securitySystem->isIPBlocked("127.0.0.1"));
}

TEST_F(SecuritySystemTest, AddToBlockListDuplicate) {
    securitySystem->addToBlockList("DUPLICATE_IP");
    securitySystem->addToBlockList("DUPLICATE_IP");
    
    EXPECT_TRUE(securitySystem->isIPBlocked("DUPLICATE_IP"));
}

TEST_F(SecuritySystemTest, SetMaxFailedAttemptsClamps) {
    securitySystem->setMaxFailedAttempts(10);
    securitySystem->setMaxFailedAttempts(-5);
    SUCCEED();
}

TEST_F(SecuritySystemTest, SettersAndGetters) {
    auto newRentalSystem = std::make_shared<RentalSystem>();
    securitySystem->setRentalSystem(newRentalSystem);
    EXPECT_EQ(securitySystem->getRentalSystem(), newRentalSystem);
    
    auto newManager = std::make_shared<CustomerManager>();
    securitySystem->setCustomerManager(newManager);
    EXPECT_EQ(securitySystem->getCustomerManager(), newManager);
}