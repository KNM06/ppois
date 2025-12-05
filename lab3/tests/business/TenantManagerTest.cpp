#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/TenantManager.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/finance/Payment.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"

class TenantManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        tenant1 = std::make_shared<Tenant>("TEN001", "John", "Doe", "john@example.com",
            "1234567890", std::make_unique<Address>("123 Main St", "City", "State", "12345"),
            720.0, 6000.0, false, 2);
        
        tenant2 = std::make_shared<Tenant>("TEN002", "Jane", "Smith", "jane@example.com",
            "0987654321", std::make_unique<Address>("456 Oak Ave", "City", "State", "67890"),
            680.0, 5000.0, true, 1);
        
        tenant3 = std::make_shared<Tenant>("TEN003", "Bob", "Johnson", "bob@example.com",
            "5555555555", std::make_unique<Address>("789 Pine Rd", "City", "State", "13579"),
            580.0, 4000.0, false, 1);
        
        auto now = std::chrono::system_clock::now();
        
        payment1 = std::make_shared<Payment>("PAY001", nullptr, nullptr, 1500.0, now,
            "bank_transfer", "completed", "TX001", 0.0, "rent");
        
        payment2 = std::make_shared<Payment>("PAY002", nullptr, nullptr, 1500.0, 
            now - std::chrono::hours(24*35), "bank_transfer", "completed", "TX002", 50.0, "rent");
        
        payment3 = std::make_shared<Payment>("PAY003", nullptr, nullptr, 200.0, now,
            "paypal", "pending", "TX003", 0.0, "fee");
        
        payment4 = std::make_shared<Payment>("PAY004", nullptr, nullptr, 1200.0,
            now - std::chrono::hours(24*10), "bank_transfer", "completed", "TX004", 0.0, "rent");
    }
    
    std::shared_ptr<Tenant> tenant1;
    std::shared_ptr<Tenant> tenant2;
    std::shared_ptr<Tenant> tenant3;
    std::shared_ptr<Payment> payment1;
    std::shared_ptr<Payment> payment2;
    std::shared_ptr<Payment> payment3;
    std::shared_ptr<Payment> payment4;
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
};

TEST_F(TenantManagerTest, ConstructorAndGetters) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 650.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.3);
}

TEST_F(TenantManagerTest, SetMinimumCreditScore) {
    TenantManager manager(650.0, 0.3);
    
    manager.setMinimumCreditScore(700.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 700.0);
    
    manager.setMinimumCreditScore(300.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 300.0);
    
    manager.setMinimumCreditScore(850.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 850.0);
    
    manager.setMinimumCreditScore(200.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 300.0);
    
    manager.setMinimumCreditScore(900.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 850.0);
    
    manager.setMinimumCreditScore(720.5);
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 720.5);
}

TEST_F(TenantManagerTest, SetMinimumIncomeToRentRatio) {
    TenantManager manager(650.0, 0.3);
    
    manager.setMinimumIncomeToRentRatio(0.25);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.25);
    
    manager.setMinimumIncomeToRentRatio(0.1);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.1);
    
    manager.setMinimumIncomeToRentRatio(0.5);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.5);
    
    manager.setMinimumIncomeToRentRatio(0.05);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.1);
    
    manager.setMinimumIncomeToRentRatio(-0.1);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.1);
    
    manager.setMinimumIncomeToRentRatio(0.33);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.33);
}

TEST_F(TenantManagerTest, RegisterTenant) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    EXPECT_TRUE(manager.registerTenant(tenant3));
    
    EXPECT_FALSE(manager.registerTenant(nullptr));
}

TEST_F(TenantManagerTest, RegisterDuplicateTenant) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant1));
}

TEST_F(TenantManagerTest, ApproveTenantApplication) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_FALSE(manager.approveTenantApplication(nullptr, 2000.0));
    
    EXPECT_TRUE(manager.approveTenantApplication(tenant1, 2000.0));
    
    EXPECT_TRUE(manager.approveTenantApplication(tenant2, 2000.0));
    
    EXPECT_FALSE(manager.approveTenantApplication(tenant3, 2000.0));
    
    manager.setMinimumCreditScore(700.0);
    EXPECT_TRUE(manager.approveTenantApplication(tenant1, 2000.0));
    
    manager.setMinimumCreditScore(650.0);
    manager.setMinimumIncomeToRentRatio(0.5);
    EXPECT_TRUE(manager.approveTenantApplication(tenant1, 2000.0));
}

TEST_F(TenantManagerTest, ApproveTenantApplicationWithPaymentHistory) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    
    EXPECT_TRUE(manager.approveTenantApplication(tenant1, 2000.0));
    
    EXPECT_TRUE(manager.hasGoodPaymentHistory(tenant1->getTenantId()));
}

TEST_F(TenantManagerTest, GetTenantPaymentHistory) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    
    auto history = manager.getTenantPaymentHistory("TEN001");
    EXPECT_TRUE(history.empty());
    
    history = manager.getTenantPaymentHistory("NON_EXISTENT");
    EXPECT_TRUE(history.empty());
}

TEST_F(TenantManagerTest, CalculateTenantPaymentScore) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("TEN001"), 100.0);
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("TEN002"), 100.0);
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("NON_EXISTENT"), 100.0);
}

TEST_F(TenantManagerTest, HasGoodPaymentHistory) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    
    EXPECT_TRUE(manager.hasGoodPaymentHistory("TEN001"));
    EXPECT_TRUE(manager.hasGoodPaymentHistory("TEN002"));
    EXPECT_TRUE(manager.hasGoodPaymentHistory("NON_EXISTENT"));
}

TEST_F(TenantManagerTest, GetTenantsWithLatePayments) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    EXPECT_TRUE(manager.registerTenant(tenant3));
    
    auto latePayers = manager.getTenantsWithLatePayments();
    EXPECT_TRUE(latePayers.empty());
}

TEST_F(TenantManagerTest, CanTenantRenewLease) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    
    EXPECT_TRUE(manager.canTenantRenewLease("TEN001"));
    EXPECT_TRUE(manager.canTenantRenewLease("TEN002"));
    EXPECT_TRUE(manager.canTenantRenewLease("NON_EXISTENT"));
}

TEST_F(TenantManagerTest, CalculateAverageRentPayment) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("TEN001"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("TEN002"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("NON_EXISTENT"), 0.0);
}

TEST_F(TenantManagerTest, SearchTenantsByCriteria) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    EXPECT_TRUE(manager.registerTenant(tenant3));
    
    auto results1 = manager.searchTenantsByCriteria(700.0, 5500.0);
    EXPECT_EQ(results1.size(), 1);
    EXPECT_EQ(results1[0]->getTenantId(), "TEN001");
    
    auto results2 = manager.searchTenantsByCriteria(650.0, 4500.0);
    EXPECT_EQ(results2.size(), 2);
    
    auto results3 = manager.searchTenantsByCriteria(600.0, 3000.0);
    EXPECT_EQ(results3.size(), 2);
    
    auto results4 = manager.searchTenantsByCriteria(800.0, 10000.0);
    EXPECT_TRUE(results4.empty());
    
    auto results5 = manager.searchTenantsByCriteria(300.0, 0.0);
    EXPECT_EQ(results5.size(), 3);
    
    auto results6 = manager.searchTenantsByCriteria(720.0, 6000.0);
    EXPECT_EQ(results6.size(), 1);
}

TEST_F(TenantManagerTest, IsTenantEligibleForUpgrade) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    EXPECT_TRUE(manager.registerTenant(tenant3));
    
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN001", 2500.0));
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN002", 2500.0));
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN003", 2500.0));
    EXPECT_FALSE(manager.isTenantEligibleForUpgrade("NON_EXISTENT", 2500.0));
    
    manager.setMinimumIncomeToRentRatio(0.5);
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN001", 2500.0));
    
    manager.setMinimumIncomeToRentRatio(0.3);
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN001", 2500.0));
}

TEST_F(TenantManagerTest, IsTenantEligibleForUpgradeBoundaryCases) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN001", 20000.0));
    
    auto lowIncomeTenant = std::make_shared<Tenant>("TEN004", "Low", "Income", "low@example.com",
        "1111111111", nullptr, 700.0, 3000.0, false, 1);
    
    EXPECT_TRUE(manager.registerTenant(lowIncomeTenant));
    
    EXPECT_FALSE(manager.isTenantEligibleForUpgrade("TEN004", 20000.0));
}

TEST_F(TenantManagerTest, CopyConstructor) {
    TenantManager original(700.0, 0.35);
    
    original.registerTenant(tenant1);
    original.registerTenant(tenant2);
    
    TenantManager copy(original);
    
    EXPECT_DOUBLE_EQ(copy.getMinimumCreditScore(), 700.0);
    EXPECT_DOUBLE_EQ(copy.getMinimumIncomeToRentRatio(), 0.35);
    
    auto results = copy.searchTenantsByCriteria(600.0, 0.0);
    EXPECT_EQ(results.size(), 2);
}

TEST_F(TenantManagerTest, CopyAssignment) {
    TenantManager original(700.0, 0.35);
    original.registerTenant(tenant1);
    
    TenantManager assigned(650.0, 0.3);
    assigned.registerTenant(tenant2);
    
    assigned = original;
    
    EXPECT_DOUBLE_EQ(assigned.getMinimumCreditScore(), 700.0);
    EXPECT_DOUBLE_EQ(assigned.getMinimumIncomeToRentRatio(), 0.35);
    
    auto results = assigned.searchTenantsByCriteria(600.0, 0.0);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0]->getTenantId(), "TEN001");
}

TEST_F(TenantManagerTest, MoveConstructor) {
    TenantManager original(700.0, 0.35);
    original.registerTenant(tenant1);
    original.registerTenant(tenant2);
    
    TenantManager moved(std::move(original));
    
    EXPECT_DOUBLE_EQ(moved.getMinimumCreditScore(), 700.0);
    EXPECT_DOUBLE_EQ(moved.getMinimumIncomeToRentRatio(), 0.35);
    
    auto results = moved.searchTenantsByCriteria(600.0, 0.0);
    EXPECT_EQ(results.size(), 2);
}

TEST_F(TenantManagerTest, MoveAssignment) {
    TenantManager original(700.0, 0.35);
    original.registerTenant(tenant1);
    
    TenantManager moved(650.0, 0.3);
    moved.registerTenant(tenant2);
    
    moved = std::move(original);
    
    EXPECT_DOUBLE_EQ(moved.getMinimumCreditScore(), 700.0);
    EXPECT_DOUBLE_EQ(moved.getMinimumIncomeToRentRatio(), 0.35);
    
    auto results = moved.searchTenantsByCriteria(600.0, 0.0);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0]->getTenantId(), "TEN001");
}

TEST_F(TenantManagerTest, SelfAssignment) {
    TenantManager manager(650.0, 0.3);
    manager.registerTenant(tenant1);
    manager.registerTenant(tenant2);
    
    manager = manager;
    
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 650.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.3);
    
    auto results = manager.searchTenantsByCriteria(600.0, 0.0);
    EXPECT_EQ(results.size(), 2);
}

TEST_F(TenantManagerTest, MixedOperationsComplexScenario) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_TRUE(manager.registerTenant(tenant1));
    EXPECT_TRUE(manager.registerTenant(tenant2));
    EXPECT_TRUE(manager.registerTenant(tenant3));
    
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 650.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.3);
    
    EXPECT_TRUE(manager.approveTenantApplication(tenant1, 2000.0));
    EXPECT_TRUE(manager.approveTenantApplication(tenant2, 1800.0));
    EXPECT_FALSE(manager.approveTenantApplication(tenant3, 1500.0));
    
    auto qualifiedTenants = manager.searchTenantsByCriteria(650.0, 5000.0);
    EXPECT_EQ(qualifiedTenants.size(), 2);
    
    auto allTenants = manager.searchTenantsByCriteria(300.0, 0.0);
    EXPECT_EQ(allTenants.size(), 3);
    
    EXPECT_TRUE(manager.hasGoodPaymentHistory("TEN001"));
    EXPECT_TRUE(manager.hasGoodPaymentHistory("TEN002"));
    EXPECT_TRUE(manager.hasGoodPaymentHistory("TEN003"));
    
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("TEN001"), 100.0);
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("TEN002"), 100.0);
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("TEN003"), 100.0);
    
    EXPECT_TRUE(manager.canTenantRenewLease("TEN001"));
    EXPECT_TRUE(manager.canTenantRenewLease("TEN002"));
    EXPECT_TRUE(manager.canTenantRenewLease("TEN003"));
    
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN001", 2500.0));
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN002", 2500.0));
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("TEN003", 2500.0));
    
    auto latePayers = manager.getTenantsWithLatePayments();
    EXPECT_TRUE(latePayers.empty());
    
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("TEN001"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("TEN002"), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("TEN003"), 0.0);
}

TEST_F(TenantManagerTest, EdgeCases) {
    TenantManager manager(300.0, 0.1);
    
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 300.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.1);
    
    auto tenant = std::make_shared<Tenant>("EDGE", "Edge", "Case", "edge@example.com",
        "9999999999", nullptr, 300.0, 100.0, false, 0);
    
    EXPECT_TRUE(manager.registerTenant(tenant));
    
    EXPECT_TRUE(manager.approveTenantApplication(tenant, 1000.0));
    
    EXPECT_TRUE(manager.hasGoodPaymentHistory("EDGE"));
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("EDGE"), 100.0);
    EXPECT_TRUE(manager.canTenantRenewLease("EDGE"));
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("EDGE"), 0.0);
    EXPECT_TRUE(manager.isTenantEligibleForUpgrade("EDGE", 1000.0));
    
    auto results = manager.searchTenantsByCriteria(300.0, 100.0);
    EXPECT_EQ(results.size(), 1);
    
    results = manager.searchTenantsByCriteria(850.0, 10000.0);
    EXPECT_TRUE(results.empty());
}

TEST_F(TenantManagerTest, TenantWithExtremeValues) {
    TenantManager manager(850.0, 1.0);
    
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 850.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 1.0);
    
    auto perfectTenant = std::make_shared<Tenant>("PERFECT", "Perfect", "Tenant", "perfect@example.com",
        "8888888888", nullptr, 850.0, 10000.0, false, 10);
    
    EXPECT_TRUE(manager.registerTenant(perfectTenant));
    
    EXPECT_TRUE(manager.approveTenantApplication(perfectTenant, 10000.0));
    
    auto poorTenant = std::make_shared<Tenant>("POOR", "Poor", "Tenant", "poor@example.com",
        "7777777777", nullptr, 300.0, 500.0, true, 1);
    
    EXPECT_TRUE(manager.registerTenant(poorTenant));
    
    EXPECT_FALSE(manager.approveTenantApplication(poorTenant, 1000.0));
}

TEST_F(TenantManagerTest, EmptyManagerOperations) {
    TenantManager manager(650.0, 0.3);
    
    EXPECT_DOUBLE_EQ(manager.getMinimumCreditScore(), 650.0);
    EXPECT_DOUBLE_EQ(manager.getMinimumIncomeToRentRatio(), 0.3);
    
    EXPECT_TRUE(manager.getTenantsWithLatePayments().empty());
    EXPECT_TRUE(manager.searchTenantsByCriteria(600.0, 3000.0).empty());
    EXPECT_TRUE(manager.getTenantPaymentHistory("ANY").empty());
    
    EXPECT_DOUBLE_EQ(manager.calculateTenantPaymentScore("ANY"), 100.0);
    EXPECT_TRUE(manager.hasGoodPaymentHistory("ANY"));
    EXPECT_TRUE(manager.canTenantRenewLease("ANY"));
    EXPECT_DOUBLE_EQ(manager.calculateAverageRentPayment("ANY"), 0.0);
    EXPECT_FALSE(manager.isTenantEligibleForUpgrade("ANY", 2000.0));
    
    auto tenant = std::make_shared<Tenant>("TEST", "Test", "Tenant", "test@example.com",
        "1231231231", nullptr, 700.0, 5000.0, false, 1);
    
    EXPECT_TRUE(manager.approveTenantApplication(tenant, 2000.0));
}