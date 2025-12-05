#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/ContractManager.h"
#include "../../include/core/Agent.h"
#include "../../include/core/Address.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class ContractManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        address = std::make_unique<Address>("Main St", "City", "State", "11111");
        status = std::make_unique<PropertyStatus>(
            "AVAILABLE", "Available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        );
        type = std::make_unique<PropertyType>(
            "TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5
        );
        ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "22222");
    }

    std::unique_ptr<Address> address;
    std::unique_ptr<PropertyStatus> status;
    std::unique_ptr<PropertyType> type;
    std::unique_ptr<Address> ownerAddress;
};

TEST_F(ContractManagerTest, ConstructorAndGetters) {
    ContractManager manager(30, 1.5, 24, {"Clause1", "Clause2"});
    
    EXPECT_EQ(manager.getAutoRenewalNoticeDays(), 30);
    EXPECT_DOUBLE_EQ(manager.getSecurityDepositMultiplier(), 1.5);
    EXPECT_EQ(manager.getMaxLeaseTermMonths(), 24);
    
    auto clauses = manager.getStandardClauses();
    ASSERT_EQ(clauses.size(), 2);
    EXPECT_EQ(clauses[0], "Clause1");
    EXPECT_EQ(clauses[1], "Clause2");
}

TEST_F(ContractManagerTest, Setters) {
    ContractManager manager(30, 1.5, 24, {});
    
    manager.setAutoRenewalNoticeDays(45);
    EXPECT_EQ(manager.getAutoRenewalNoticeDays(), 45);
    
    manager.setSecurityDepositMultiplier(2.0);
    EXPECT_DOUBLE_EQ(manager.getSecurityDepositMultiplier(), 2.0);
    
    manager.setMaxLeaseTermMonths(36);
    EXPECT_EQ(manager.getMaxLeaseTermMonths(), 36);
    
    manager.addStandardClause("New Clause");
    auto clauses = manager.getStandardClauses();
    ASSERT_EQ(clauses.size(), 1);
    EXPECT_EQ(clauses[0], "New Clause");
}

TEST_F(ContractManagerTest, CreateContractWithNullPointers) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto property = std::make_shared<Property>(
        "P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type)
    );
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_FALSE(manager.createRentalContract(nullptr, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    EXPECT_FALSE(manager.createRentalContract(property, nullptr, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    EXPECT_FALSE(manager.createRentalContract(property, tenant, nullptr, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
}

TEST_F(ContractManagerTest, CreateContractWithInvalidLeaseTerm) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_FALSE(manager.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 0, 2000.0, "monthly"));
    EXPECT_FALSE(manager.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 25, 2000.0, "monthly"));
}

TEST_F(ContractManagerTest, CreateDuplicateContract) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto address2 = std::make_unique<Address>("Main St", "City", "State", "11111");
    auto status2 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type2 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    EXPECT_FALSE(manager.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
}

TEST_F(ContractManagerTest, FindNonExistentContract) {
    ContractManager manager(30, 1.5, 24, {});
    
    EXPECT_EQ(manager.findContractByProperty("NON_EXISTENT"), nullptr);
}

TEST_F(ContractManagerTest, TerminateNonExistentContract) {
    ContractManager manager(30, 1.5, 24, {});
    
    EXPECT_FALSE(manager.terminateContract("NON_EXISTENT", "Reason"));
}

TEST_F(ContractManagerTest, RenewNonExistentContract) {
    ContractManager manager(30, 1.5, 24, {});
    
    EXPECT_FALSE(manager.renewContract("NON_EXISTENT", 12, 2000.0));
}

TEST_F(ContractManagerTest, RenewInactiveContract) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    
    auto contract = manager.findContractByProperty("P1");
    ASSERT_NE(contract, nullptr);
    
    EXPECT_TRUE(manager.terminateContract(contract->getContractId(), "Test"));
    EXPECT_FALSE(manager.renewContract(contract->getContractId(), 12, 2000.0));
}

TEST_F(ContractManagerTest, CalculateRequiredSecurityDeposit) {
    ContractManager manager(30, 1.5, 24, {});
    
    EXPECT_DOUBLE_EQ(manager.calculateRequiredSecurityDeposit(1000.0), 1500.0);
    EXPECT_DOUBLE_EQ(manager.calculateRequiredSecurityDeposit(2000.0), 3000.0);
    
    manager.setSecurityDepositMultiplier(2.0);
    EXPECT_DOUBLE_EQ(manager.calculateRequiredSecurityDeposit(1000.0), 2000.0);
}

TEST_F(ContractManagerTest, IsLeaseTermValid) {
    ContractManager manager(30, 1.5, 24, {});
    
    EXPECT_FALSE(manager.isLeaseTermValid(0));
    EXPECT_TRUE(manager.isLeaseTermValid(1));
    EXPECT_TRUE(manager.isLeaseTermValid(12));
    EXPECT_TRUE(manager.isLeaseTermValid(24));
    EXPECT_FALSE(manager.isLeaseTermValid(25));
    
    manager.setMaxLeaseTermMonths(36);
    EXPECT_TRUE(manager.isLeaseTermValid(36));
    EXPECT_FALSE(manager.isLeaseTermValid(37));
}

TEST_F(ContractManagerTest, GetContractsByTenant) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto address1 = std::make_unique<Address>("St1", "City", "State", "11111");
    auto status1 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type1 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto address2 = std::make_unique<Address>("St2", "City", "State", "22222");
    auto status2 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type2 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "33333");
    
    auto property1 = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address1), std::move(status1), std::move(type1));
    auto property2 = std::make_shared<Property>("P2", 100.0, 1200.0, std::move(address2), std::move(status2), std::move(type2));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    EXPECT_TRUE(manager.createRentalContract(property2, tenant, owner, std::chrono::system_clock::now(), 6, 1500.0, "monthly"));
    
    auto contracts = manager.getContractsByTenant("T1");
    EXPECT_EQ(contracts.size(), 2);
    
    auto nonExistentContracts = manager.getContractsByTenant("NON_EXISTENT");
    EXPECT_TRUE(nonExistentContracts.empty());
}

TEST_F(ContractManagerTest, HasContractViolations) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    
    EXPECT_FALSE(manager.hasContractViolations("T1"));
    EXPECT_FALSE(manager.hasContractViolations("NON_EXISTENT"));
}

TEST_F(ContractManagerTest, CanTenantRentAnotherProperty) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto address1 = std::make_unique<Address>("St1", "City", "State", "11111");
    auto status1 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type1 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto address2 = std::make_unique<Address>("St2", "City", "State", "22222");
    auto status2 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type2 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto address3 = std::make_unique<Address>("St3", "City", "State", "33333");
    auto status3 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type3 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "33333");
    
    auto property1 = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address1), std::move(status1), std::move(type1));
    auto property2 = std::make_shared<Property>("P2", 100.0, 1200.0, std::move(address2), std::move(status2), std::move(type2));
    auto property3 = std::make_shared<Property>("P3", 80.0, 1000.0, std::move(address3), std::move(status3), std::move(type3));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager.canTenantRentAnotherProperty("T1"));
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    EXPECT_TRUE(manager.canTenantRentAnotherProperty("T1"));
    
    EXPECT_TRUE(manager.createRentalContract(property2, tenant, owner, std::chrono::system_clock::now(), 6, 1500.0, "monthly"));
    EXPECT_FALSE(manager.canTenantRentAnotherProperty("T1"));
    
    EXPECT_TRUE(manager.terminateContract(manager.findContractByProperty("P1")->getContractId(), "Termination"));
    EXPECT_TRUE(manager.canTenantRentAnotherProperty("T1"));
}

TEST_F(ContractManagerTest, CalculateTotalMonthlyRent) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto address1 = std::make_unique<Address>("St1", "City", "State", "11111");
    auto status1 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type1 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto address2 = std::make_unique<Address>("St2", "City", "State", "22222");
    auto status2 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type2 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    
    auto ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "33333");
    
    auto property1 = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address1), std::move(status1), std::move(type1));
    auto property2 = std::make_shared<Property>("P2", 100.0, 1200.0, std::move(address2), std::move(status2), std::move(type2));
    auto tenant1 = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto tenant2 = std::make_shared<Tenant>("T2", "Jane", "Smith", "jane@email.com", "0987654321", nullptr, 600, 2500, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRent(), 0.0);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRent(), 2000.0);
    
    EXPECT_TRUE(manager.createRentalContract(property2, tenant2, owner, std::chrono::system_clock::now(), 6, 1500.0, "monthly"));
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRent(), 3500.0);
    
    EXPECT_TRUE(manager.terminateContract(manager.findContractByProperty("P1")->getContractId(), "Termination"));
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRent(), 1500.0);
}

TEST_F(ContractManagerTest, GetExpiringContracts) {
    ContractManager manager(30, 1.5, 24, {});
    
    auto now = std::chrono::system_clock::now();
    auto future = now + std::chrono::hours(15 * 24);
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager.createRentalContract(property, tenant, owner, now, 1, 2000.0, "monthly"));
    
    auto expiring = manager.getExpiringContracts(30);
    EXPECT_EQ(expiring.size(), 1);
    
    auto notExpiring = manager.getExpiringContracts(5);
    EXPECT_EQ(notExpiring.size(), 0);
}

TEST_F(ContractManagerTest, CopyConstructorAndAssignment) {
    ContractManager manager1(30, 1.5, 24, {"Clause1"});
    
    auto address1 = std::make_unique<Address>("St1", "City", "State", "11111");
    auto status1 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type1 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    auto ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "33333");
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address1), std::move(status1), std::move(type1));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager1.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    
    ContractManager manager2 = manager1;
    
    EXPECT_EQ(manager2.getAutoRenewalNoticeDays(), 30);
    EXPECT_DOUBLE_EQ(manager2.getSecurityDepositMultiplier(), 1.5);
    EXPECT_EQ(manager2.getMaxLeaseTermMonths(), 24);
    
    auto contract = manager2.findContractByProperty("P1");
    EXPECT_NE(contract, nullptr);
    
    ContractManager manager3(15, 2.0, 12, {});
    manager3 = manager1;
    
    EXPECT_EQ(manager3.getAutoRenewalNoticeDays(), 30);
    EXPECT_DOUBLE_EQ(manager3.getSecurityDepositMultiplier(), 1.5);
}

TEST_F(ContractManagerTest, MoveConstructorAndAssignment) {
    ContractManager manager1(30, 1.5, 24, {"Clause1"});
    
    auto address1 = std::make_unique<Address>("St1", "City", "State", "11111");
    auto status1 = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);
    auto type1 = std::make_unique<PropertyType>("TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5);
    auto ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "33333");
    
    auto property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address1), std::move(status1), std::move(type1));
    auto tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@email.com", "1234567890", nullptr, 500, 2000, false, 1);
    auto owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
    
    EXPECT_TRUE(manager1.createRentalContract(property, tenant, owner, std::chrono::system_clock::now(), 12, 2000.0, "monthly"));
    
    ContractManager manager2 = std::move(manager1);
    
    EXPECT_EQ(manager2.getAutoRenewalNoticeDays(), 30);
    EXPECT_DOUBLE_EQ(manager2.getSecurityDepositMultiplier(), 1.5);
    EXPECT_EQ(manager2.getMaxLeaseTermMonths(), 24);
    
    auto contract = manager2.findContractByProperty("P1");
    EXPECT_NE(contract, nullptr);
    
    ContractManager manager3(15, 2.0, 12, {});
    manager3 = std::move(manager2);
    
    EXPECT_EQ(manager3.getAutoRenewalNoticeDays(), 30);
    EXPECT_DOUBLE_EQ(manager3.getSecurityDepositMultiplier(), 1.5);
    
    contract = manager3.findContractByProperty("P1");
    EXPECT_NE(contract, nullptr);
}