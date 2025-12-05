#include <gtest/gtest.h>
#include "../../include/finance/Commission.h"
#include "../../include/core/Agent.h"
#include "../../include/core/Address.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"

class CommissionTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto agentAddress = std::make_unique<Address>("456 Broker St", "New York", "NY", "10002", "USA", 40.7130, -74.0050);
        agent = std::make_shared<Agent>("AG001", "John", "Doe", "john.doe@agency.com", "555-0123", 
                                       "RE123456", std::move(agentAddress), 3.5, "residential", 8, true, 
                                       std::vector<std::string>{"CRS", "ABR"});
        
        auto propertyAddress = std::make_unique<Address>("123 Main St", "New York", "NY", "10001", "USA", 40.7128, -74.0060);
        auto propertyStatus = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false,
                                                              std::chrono::system_clock::now(), "", 0.0, 0);
        auto propertyType = std::make_unique<PropertyType>("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
        auto property = std::make_shared<Property>("PROP001", 85.5, 2500.0, std::move(propertyAddress), 
                                                  std::move(propertyStatus), std::move(propertyType));
        
        auto tenantAddress = std::make_unique<Address>("456 Tenant St", "New York", "NY", "10002", "USA", 40.7130, -74.0050);
        auto tenant = std::make_shared<Tenant>("T001", "John", "Doe", "john.doe@email.com", "555-0101", 
                                              std::move(tenantAddress), 750.0, 8000.0, false, 2);
        
        auto ownerAddress = std::make_unique<Address>("789 Owner Ave", "New York", "NY", "10003", "USA", 40.7140, -74.0040);
        auto owner = std::make_shared<Owner>("O001", "Robert", "Johnson", "robert.johnson@email.com", "555-0201", 
                                            std::move(ownerAddress), "123456789", 100.0, true, "direct_deposit");
        
        auto startDate = std::chrono::system_clock::now();
        auto endDate = startDate + std::chrono::hours(365 * 24);
        contract = std::make_shared<RentalContract>("CNT001", property, tenant, owner, startDate, endDate, 
                                                   2500.0, 5000.0, "Net 30 days");
        
        earnedDate = std::chrono::system_clock::now();
        paymentDate = earnedDate + std::chrono::hours(24 * 15);
    }
    
    std::shared_ptr<Agent> agent;
    std::shared_ptr<RentalContract> contract;
    std::chrono::system_clock::time_point earnedDate;
    std::chrono::system_clock::time_point paymentDate;
};

TEST_F(CommissionTest, ConstructorAndGetters) {
    Commission commission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                         "pending", "direct_deposit", 1000.0, "leasing");
    
    EXPECT_EQ(commission.getCommissionId(), "COM001");
    EXPECT_EQ(commission.getAgent(), agent.get());
    EXPECT_EQ(commission.getContract(), contract.get());
    EXPECT_DOUBLE_EQ(commission.getCommissionRate(), 3.5);
    EXPECT_DOUBLE_EQ(commission.getCommissionAmount(), 10500.0);
    EXPECT_EQ(commission.getEarnedDate(), earnedDate);
    EXPECT_EQ(commission.getPaymentDate(), paymentDate);
    EXPECT_EQ(commission.getStatus(), "pending");
    EXPECT_EQ(commission.getPaymentMethod(), "direct_deposit");
    EXPECT_DOUBLE_EQ(commission.getBonusAmount(), 1000.0);
    EXPECT_EQ(commission.getCommissionType(), "leasing");
}

TEST_F(CommissionTest, TotalCommissionCalculation) {
    Commission commission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                         "pending", "direct_deposit", 1000.0, "leasing");
    
    double totalCommission = commission.calculateTotalCommission();
    EXPECT_DOUBLE_EQ(totalCommission, 11500.0);
}

TEST_F(CommissionTest, PaidStatusCheck) {
    Commission paidCommission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                             "paid", "direct_deposit", 1000.0, "leasing");
    
    Commission processedCommission("COM002", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                                   "processed", "direct_deposit", 1000.0, "leasing");
    
    Commission pendingCommission("COM003", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                                "pending", "direct_deposit", 1000.0, "leasing");
    
    EXPECT_TRUE(paidCommission.isPaid());
    EXPECT_TRUE(processedCommission.isPaid());
    EXPECT_FALSE(pendingCommission.isPaid());
}

TEST_F(CommissionTest, OverdueStatus) {
    auto oldEarnedDate = std::chrono::system_clock::now() - std::chrono::hours(24 * 35);
    
    Commission overdueCommission("COM001", agent, contract, 3.5, 10500.0, oldEarnedDate, paymentDate, 
                                "pending", "direct_deposit", 1000.0, "leasing");
    
    Commission paidCommission("COM002", agent, contract, 3.5, 10500.0, oldEarnedDate, paymentDate, 
                             "paid", "direct_deposit", 1000.0, "leasing");
    
    Commission recentCommission("COM003", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                               "pending", "direct_deposit", 1000.0, "leasing");
    
    EXPECT_TRUE(overdueCommission.isOverdue(std::chrono::system_clock::now()));
    EXPECT_FALSE(paidCommission.isOverdue(std::chrono::system_clock::now()));
    EXPECT_FALSE(recentCommission.isOverdue(std::chrono::system_clock::now()));
}

TEST_F(CommissionTest, TaxWithholdingCalculation) {
    Commission commission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                         "pending", "direct_deposit", 1000.0, "leasing");
    
    double taxWithholding = commission.calculateTaxWithholding(25.0);
    EXPECT_DOUBLE_EQ(taxWithholding, 2875.0);
}

TEST_F(CommissionTest, LeasingCommissionIdentification) {
    Commission leasingCommission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                                "pending", "direct_deposit", 1000.0, "leasing");
    
    Commission managementCommission("COM002", agent, contract, 2.0, 6000.0, earnedDate, paymentDate, 
                                   "pending", "direct_deposit", 500.0, "property_management");
    
    EXPECT_TRUE(leasingCommission.isLeasingCommission());
    EXPECT_FALSE(managementCommission.isLeasingCommission());
}

TEST_F(CommissionTest, ManagementCommissionIdentification) {
    Commission managementCommission("COM001", agent, contract, 2.0, 6000.0, earnedDate, paymentDate, 
                                   "pending", "direct_deposit", 500.0, "management");
    
    Commission leasingCommission("COM002", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                                "pending", "direct_deposit", 1000.0, "leasing");
    
    EXPECT_TRUE(managementCommission.isManagementCommission());
    EXPECT_FALSE(leasingCommission.isManagementCommission());
}

TEST_F(CommissionTest, NetCommissionCalculation) {
    Commission commission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                         "pending", "direct_deposit", 1000.0, "leasing");
    
    double netCommission = commission.calculateNetCommission(25.0);
    EXPECT_DOUBLE_EQ(netCommission, 8625.0);
}

TEST_F(CommissionTest, BonusQualification) {
    Commission highCommission("COM001", agent, contract, 3.5, 6000.0, earnedDate, paymentDate, 
                             "pending", "direct_deposit", 1000.0, "leasing");
    
    auto highRentContract = std::make_shared<RentalContract>("CNT002", contract->getPropertySharedPtr(), 
                                                           contract->getTenantSharedPtr(), 
                                                           contract->getOwnerSharedPtr(),
                                                           earnedDate, paymentDate, 3500.0, 7000.0, "Net 30 days");
    
    Commission highRentCommission("COM002", agent, highRentContract, 3.5, 5000.0, earnedDate, paymentDate, 
                                 "pending", "direct_deposit", 0.0, "leasing");
    
    Commission lowCommission("COM003", agent, contract, 3.5, 4000.0, earnedDate, paymentDate, 
                            "pending", "direct_deposit", 0.0, "leasing");
    
    EXPECT_TRUE(highCommission.qualifiesForBonus());
    EXPECT_TRUE(highRentCommission.qualifiesForBonus());
    EXPECT_FALSE(lowCommission.qualifiesForBonus());
}

TEST_F(CommissionTest, ManagerApprovalRequirement) {
    Commission highAmountCommission("COM001", agent, contract, 3.5, 12000.0, earnedDate, paymentDate, 
                                   "pending", "direct_deposit", 0.0, "leasing");
    
    Commission highBonusCommission("COM002", agent, contract, 3.5, 8000.0, earnedDate, paymentDate, 
                                  "pending", "direct_deposit", 2500.0, "leasing");
    
    Commission standardCommission("COM003", agent, contract, 3.5, 8000.0, earnedDate, paymentDate, 
                                 "pending", "direct_deposit", 1500.0, "leasing");
    
    EXPECT_TRUE(highAmountCommission.requiresManagerApproval());
    EXPECT_TRUE(highBonusCommission.requiresManagerApproval());
    EXPECT_FALSE(standardCommission.requiresManagerApproval());
}

TEST_F(CommissionTest, CommissionTier) {
    Commission standardCommission("COM001", agent, contract, 3.5, 800.0, earnedDate, paymentDate, 
                                 "pending", "direct_deposit", 0.0, "leasing");
    
    Commission bronzeCommission("COM002", agent, contract, 3.5, 3000.0, earnedDate, paymentDate, 
                               "pending", "direct_deposit", 0.0, "leasing");
    
    Commission silverCommission("COM003", agent, contract, 3.5, 8000.0, earnedDate, paymentDate, 
                               "pending", "direct_deposit", 0.0, "leasing");
    
    Commission goldCommission("COM004", agent, contract, 3.5, 12000.0, earnedDate, paymentDate, 
                             "pending", "direct_deposit", 0.0, "leasing");
    
    Commission platinumCommission("COM005", agent, contract, 3.5, 20000.0, earnedDate, paymentDate, 
                                 "pending", "direct_deposit", 0.0, "leasing");
    
 
    EXPECT_EQ(standardCommission.getCommissionTier(), "standard");
    
    EXPECT_EQ(bronzeCommission.getCommissionTier(), "premium");
    
    EXPECT_EQ(silverCommission.getCommissionTier(), "elite");
    
    EXPECT_EQ(goldCommission.getCommissionTier(), "elite");
    
    EXPECT_EQ(platinumCommission.getCommissionTier(), "executive");
}

TEST_F(CommissionTest, SetterMethods) {
    Commission commission("COM001", agent, contract, 3.5, 10500.0, earnedDate, paymentDate, 
                         "pending", "direct_deposit", 1000.0, "leasing");
    
    auto newPaymentDate = std::chrono::system_clock::now() + std::chrono::hours(24 * 10);
    
    commission.setStatus("paid");
    commission.setPaymentDate(newPaymentDate);
    commission.setBonusAmount(1500.0);
    
    EXPECT_EQ(commission.getStatus(), "paid");
    EXPECT_EQ(commission.getPaymentDate(), newPaymentDate);
    EXPECT_DOUBLE_EQ(commission.getBonusAmount(), 1500.0);
}