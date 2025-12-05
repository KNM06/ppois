#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/RentalManager.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/core/Address.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class RentalManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto address1 = std::make_unique<Address>("123 Main St", "City", "State", "12345");
        auto status1 = std::make_unique<PropertyStatus>("AVAILABLE");
        auto type1 = std::make_unique<PropertyType>("Apartment");
        
        auto address2 = std::make_unique<Address>("456 Oak Ave", "City", "State", "67890");
        auto status2 = std::make_unique<PropertyStatus>("AVAILABLE");
        auto type2 = std::make_unique<PropertyType>("House");
        
        auto address3 = std::make_unique<Address>("789 Pine Rd", "City", "State", "13579");
        auto status3 = std::make_unique<PropertyStatus>("AVAILABLE");
        auto type3 = std::make_unique<PropertyType>("Condo");
        
        property1 = std::make_shared<Property>("PROP001", 120.0, 1500.0,
            std::move(address1), std::move(status1), std::move(type1));
        
        property2 = std::make_shared<Property>("PROP002", 200.0, 2500.0,
            std::move(address2), std::move(status2), std::move(type2));
        
        property3 = std::make_shared<Property>("PROP003", 180.0, 2000.0,
            std::move(address3), std::move(status3), std::move(type3));
        
        tenant1 = std::make_shared<Tenant>("TEN001", "John", "Doe", "john@example.com",
            "1234567890", std::make_unique<Address>("100 Tenant St", "City", "State", "11111"),
            700.0, 5000.0, false, 1);
        
        tenant2 = std::make_shared<Tenant>("TEN002", "Jane", "Smith", "jane@example.com",
            "0987654321", std::make_unique<Address>("200 Tenant St", "City", "State", "22222"),
            900.0, 8000.0, false, 2);
        
        owner1 = std::make_shared<Owner>("OWN001", "Bob", "Owner", "bob@example.com",
            "5555555555", std::make_unique<Address>("300 Owner St", "City", "State", "33333"),
            "TAX001", 100.0, true, "bank_transfer");
        
        owner2 = std::make_shared<Owner>("OWN002", "Alice", "Landlord", "alice@example.com",
            "4444444444", std::make_unique<Address>("400 Owner St", "City", "State", "44444"),
            "TAX002", 100.0, true, "paypal");
    }
    
    std::shared_ptr<Property> property1;
    std::shared_ptr<Property> property2;
    std::shared_ptr<Property> property3;
    std::shared_ptr<Tenant> tenant1;
    std::shared_ptr<Tenant> tenant2;
    std::shared_ptr<Owner> owner1;
    std::shared_ptr<Owner> owner2;
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
};

TEST_F(RentalManagerTest, ConstructorAndGetters) {
    RentalManager manager(7.5, 45);
    
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 7.5);
    EXPECT_EQ(manager.getLeaseRenewalNoticeDays(), 45);
}

TEST_F(RentalManagerTest, SetManagementFeePercentage) {
    RentalManager manager(5.0, 30);
    
    manager.setManagementFeePercentage(10.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 10.0);
    
    manager.setManagementFeePercentage(0.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 0.0);
    
    manager.setManagementFeePercentage(100.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 100.0);
    
    manager.setManagementFeePercentage(-10.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 0.0);
    
    manager.setManagementFeePercentage(150.0);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 100.0);
    
    manager.setManagementFeePercentage(12.5);
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 12.5);
}

TEST_F(RentalManagerTest, SetLeaseRenewalNoticeDays) {
    RentalManager manager(5.0, 30);
    
    manager.setLeaseRenewalNoticeDays(60);
    EXPECT_EQ(manager.getLeaseRenewalNoticeDays(), 60);
    
    manager.setLeaseRenewalNoticeDays(0);
    EXPECT_EQ(manager.getLeaseRenewalNoticeDays(), 0);
    
    manager.setLeaseRenewalNoticeDays(365);
    EXPECT_EQ(manager.getLeaseRenewalNoticeDays(), 365);
    
    manager.setLeaseRenewalNoticeDays(-10);
    EXPECT_EQ(manager.getLeaseRenewalNoticeDays(), 0);
}

TEST_F(RentalManagerTest, AddProperty) {
    RentalManager manager(5.0, 30);
    
    EXPECT_TRUE(manager.getAvailableProperties().empty());
    
    manager.addProperty(property1);
    EXPECT_EQ(manager.getAvailableProperties().size(), 1);
    
    manager.addProperty(property2);
    manager.addProperty(property3);
    EXPECT_EQ(manager.getAvailableProperties().size(), 3);
    
}

TEST_F(RentalManagerTest, AddTenant) {
    RentalManager manager(5.0, 30);
    
    EXPECT_TRUE(manager.getActiveTenants().empty());    
}

TEST_F(RentalManagerTest, CreateRentalContract) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_FALSE(manager.isPropertyAvailable(property1->getId()));
    EXPECT_EQ(manager.getActiveTenants().size(), 1);
    
    auto contract = manager.findContractByProperty(property1->getId());
    EXPECT_NE(contract, nullptr);
    EXPECT_EQ(contract->getMonthlyRent(), 1500.0);
    EXPECT_EQ(contract->getSecurityDeposit(), 750.0);
}

TEST_F(RentalManagerTest, CreateRentalContractNullPointers) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_FALSE(manager.createRentalContract(nullptr, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_FALSE(manager.createRentalContract(property1, nullptr, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_FALSE(manager.createRentalContract(property1, tenant1, nullptr,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_TRUE(manager.isPropertyAvailable(property1->getId()));
}

TEST_F(RentalManagerTest, CreateRentalContractPropertyNotAvailable) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    manager.addTenant(tenant2);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_FALSE(manager.createRentalContract(property1, tenant2, owner2,
        startDate, endDate, 1600.0, 800.0, "monthly"));
    
    EXPECT_EQ(manager.getActiveTenants().size(), 1);
}

TEST_F(RentalManagerTest, TerminateRentalContract) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_FALSE(manager.isPropertyAvailable(property1->getId()));
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    
    EXPECT_TRUE(manager.terminateRentalContract(contract->getContractId()));
    EXPECT_TRUE(manager.isPropertyAvailable(property1->getId()));
    EXPECT_EQ(manager.getActiveTenants().size(), 0);
}

TEST_F(RentalManagerTest, TerminateNonExistentRentalContract) {
    RentalManager manager(5.0, 30);
    
    EXPECT_FALSE(manager.terminateRentalContract("NON_EXISTENT"));
}

TEST_F(RentalManagerTest, GetAvailableProperties) {
    RentalManager manager(5.0, 30);
    
    EXPECT_TRUE(manager.getAvailableProperties().empty());
    
    manager.addProperty(property1);
    manager.addProperty(property2);
    manager.addProperty(property3);
    
    auto availableProps = manager.getAvailableProperties();
    EXPECT_EQ(availableProps.size(), 3);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    manager.addTenant(tenant1);
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    availableProps = manager.getAvailableProperties();
    EXPECT_EQ(availableProps.size(), 2);
    
    bool foundProperty2 = false;
    bool foundProperty3 = false;
    for (const auto& prop : availableProps) {
        if (prop->getId() == "PROP002") foundProperty2 = true;
        if (prop->getId() == "PROP003") foundProperty3 = true;
    }
    EXPECT_TRUE(foundProperty2);
    EXPECT_TRUE(foundProperty3);
}

TEST_F(RentalManagerTest, GetActiveTenants) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addProperty(property2);
    manager.addTenant(tenant1);
    manager.addTenant(tenant2);
    
    EXPECT_TRUE(manager.getActiveTenants().empty());
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    auto activeTenants = manager.getActiveTenants();
    EXPECT_EQ(activeTenants.size(), 1);
    EXPECT_EQ(activeTenants[0]->getTenantId(), "TEN001");
    
    EXPECT_TRUE(manager.createRentalContract(property2, tenant2, owner2,
        startDate, endDate, 2500.0, 1250.0, "monthly"));
    
    activeTenants = manager.getActiveTenants();
    EXPECT_EQ(activeTenants.size(), 2);
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    EXPECT_TRUE(manager.terminateRentalContract(contract->getContractId()));
    
    activeTenants = manager.getActiveTenants();
    EXPECT_EQ(activeTenants.size(), 1);
    EXPECT_EQ(activeTenants[0]->getTenantId(), "TEN002");
}

TEST_F(RentalManagerTest, GetActiveTenantsWithTerminatedContracts) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    contract->terminateContract();
    
    auto activeTenants = manager.getActiveTenants();
    EXPECT_TRUE(activeTenants.empty());
}

TEST_F(RentalManagerTest, CalculateTotalMonthlyRevenue) {
    RentalManager manager(5.0, 30);
    
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 0.0);
    
    manager.addProperty(property1);
    manager.addProperty(property2);
    manager.addProperty(property3);
    manager.addTenant(tenant1);
    manager.addTenant(tenant2);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 1500.0);
    
    EXPECT_TRUE(manager.createRentalContract(property2, tenant2, owner2,
        startDate, endDate, 2500.0, 1250.0, "monthly"));
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 4000.0);
    
    EXPECT_TRUE(manager.createRentalContract(property3, tenant1, owner1,
        startDate, endDate, 2000.0, 1000.0, "monthly"));
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 6000.0);
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    contract->terminateContract();
    
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 4500.0);
}

TEST_F(RentalManagerTest, CalculateManagementFees) {
    RentalManager manager(10.0, 30);
    
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(1000.0), 100.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(500.0), 50.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(0.0), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(-1000.0), -100.0);
    
    manager.setManagementFeePercentage(15.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(1000.0), 150.0);
    
    manager.setManagementFeePercentage(0.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(1000.0), 0.0);
    
    manager.setManagementFeePercentage(100.0);
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(1000.0), 1000.0);
}

TEST_F(RentalManagerTest, IsPropertyAvailable) {
    RentalManager manager(5.0, 30);
    
    EXPECT_TRUE(manager.isPropertyAvailable("NON_EXISTENT"));
    
    manager.addProperty(property1);
    EXPECT_TRUE(manager.isPropertyAvailable(property1->getId()));
    
    manager.addTenant(tenant1);
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_FALSE(manager.isPropertyAvailable(property1->getId()));
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    contract->terminateContract();
    
    EXPECT_TRUE(manager.isPropertyAvailable(property1->getId()));
}

TEST_F(RentalManagerTest, FindContractByProperty) {
    RentalManager manager(5.0, 30);
    
    EXPECT_EQ(manager.findContractByProperty("NON_EXISTENT"), nullptr);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    auto contract = manager.findContractByProperty(property1->getId());
    EXPECT_NE(contract, nullptr);
    EXPECT_EQ(contract->getProperty()->getId(), property1->getId());
    EXPECT_EQ(contract->getTenant()->getTenantId(), tenant1->getTenantId());
    EXPECT_DOUBLE_EQ(contract->getMonthlyRent(), 1500.0);
    
    contract->terminateContract();
    contract = manager.findContractByProperty(property1->getId());
    EXPECT_NE(contract, nullptr);
}

TEST_F(RentalManagerTest, GetExpiringContracts) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addProperty(property2);
    manager.addTenant(tenant1);
    manager.addTenant(tenant2);
    
    auto startDate = currentTime;
    auto shortEndDate = startDate + std::chrono::hours(24 * 5);
    auto longEndDate = startDate + std::chrono::hours(24 * 365);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, shortEndDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_TRUE(manager.createRentalContract(property2, tenant2, owner2,
        startDate, longEndDate, 2500.0, 1250.0, "monthly"));
    
    auto expiringSoon = manager.getExpiringContracts(7);
    EXPECT_EQ(expiringSoon.size(), 1);
    EXPECT_EQ(expiringSoon[0]->getProperty()->getId(), "PROP001");
    
    auto expiringLater = manager.getExpiringContracts(400);
    EXPECT_EQ(expiringLater.size(), 2);
    
    auto notExpiring = manager.getExpiringContracts(3);
    EXPECT_TRUE(notExpiring.empty());
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    contract->terminateContract();
    
    expiringSoon = manager.getExpiringContracts(7);
    EXPECT_TRUE(expiringSoon.empty());
}

TEST_F(RentalManagerTest, CanTenantAffordProperty) {
    RentalManager manager(5.0, 30);
    
    EXPECT_FALSE(manager.canTenantAffordProperty(nullptr, property1));
    EXPECT_FALSE(manager.canTenantAffordProperty(tenant1, nullptr));
    EXPECT_FALSE(manager.canTenantAffordProperty(nullptr, nullptr));
    
    auto poorTenant = std::make_shared<Tenant>("POOR", "Poor", "Tenant", "poor@example.com",
        "0000000000", nullptr, 300.0, 2000.0, false, 1);
    
    auto richTenant = std::make_shared<Tenant>("RICH", "Rich", "Tenant", "rich@example.com",
        "9999999999", nullptr, 2000.0, 15000.0, false, 1);
    
    auto cheapProperty = std::make_shared<Property>("CHEAP", 50.0, 500.0,
        nullptr, nullptr, nullptr);
    
    auto expensiveProperty = std::make_shared<Property>("EXPENSIVE", 300.0, 5000.0,
        nullptr, nullptr, nullptr);
    
    EXPECT_TRUE(manager.canTenantAffordProperty(poorTenant, cheapProperty));
    EXPECT_FALSE(manager.canTenantAffordProperty(poorTenant, expensiveProperty));
    
    EXPECT_TRUE(manager.canTenantAffordProperty(richTenant, cheapProperty));
    EXPECT_FALSE(manager.canTenantAffordProperty(richTenant, expensiveProperty));
    
    EXPECT_TRUE(manager.canTenantAffordProperty(tenant1, property1));
    EXPECT_FALSE(manager.canTenantAffordProperty(tenant2, property2));
}

TEST_F(RentalManagerTest, CalculateVacancyRate) {
    RentalManager manager(5.0, 30);
    
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 0.0);
    
    manager.addProperty(property1);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 100.0);
    
    manager.addProperty(property2);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 100.0);
    
    manager.addTenant(tenant1);
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly"));
    
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 50.0);
    
    manager.addTenant(tenant2);
    EXPECT_TRUE(manager.createRentalContract(property2, tenant2, owner2,
        startDate, endDate, 2500.0, 1250.0, "monthly"));
    
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 0.0);
    
    manager.addProperty(property3);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), (1.0 / 3.0) * 100.0);
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    contract->terminateContract();
    
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), (2.0 / 3.0) * 100.0);
}

TEST_F(RentalManagerTest, CalculateVacancyRateWithZeroRentalPriceProperties) {
    RentalManager manager(5.0, 30);
    
    auto zeroPriceProperty = std::make_shared<Property>("ZERO", 100.0, 0.0,
        nullptr, nullptr, nullptr);
    
    manager.addProperty(zeroPriceProperty);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 100.0);
    
    auto tenant = std::make_shared<Tenant>("T1", "Test", "Tenant", "test@example.com",
        "1111111111", nullptr, 500.0, 3000.0, false, 1);
    
    manager.addTenant(tenant);
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(zeroPriceProperty, tenant, owner1,
        startDate, endDate, 0.0, 0.0, "monthly"));
    
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 0.0);
}

TEST_F(RentalManagerTest, CopyConstructor) {
    RentalManager original(7.5, 45);
    
    original.addProperty(property1);
    original.addProperty(property2);
    original.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    original.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly");
    
    RentalManager copy(original);
    
    EXPECT_DOUBLE_EQ(copy.getManagementFeePercentage(), 7.5);
    EXPECT_EQ(copy.getLeaseRenewalNoticeDays(), 45);
    
    EXPECT_EQ(copy.getAvailableProperties().size(), 1);
    EXPECT_EQ(copy.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(copy.calculateTotalMonthlyRevenue(), 1500.0);
    
    auto contract = copy.findContractByProperty(property1->getId());
    EXPECT_NE(contract, nullptr);
    EXPECT_DOUBLE_EQ(contract->getMonthlyRent(), 1500.0);
}

TEST_F(RentalManagerTest, CopyAssignment) {
    RentalManager original(7.5, 45);
    
    original.addProperty(property1);
    original.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    original.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly");
    
    RentalManager assigned(10.0, 60);
    assigned.addProperty(property2);
    assigned.addTenant(tenant2);
    
    assigned = original;
    
    EXPECT_DOUBLE_EQ(assigned.getManagementFeePercentage(), 7.5);
    EXPECT_EQ(assigned.getLeaseRenewalNoticeDays(), 45);
    
    EXPECT_EQ(assigned.getAvailableProperties().size(), 0);
    EXPECT_EQ(assigned.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(assigned.calculateTotalMonthlyRevenue(), 1500.0);
}

TEST_F(RentalManagerTest, MoveConstructor) {
    RentalManager original(7.5, 45);
    
    original.addProperty(property1);
    original.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    original.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly");
    
    RentalManager moved(std::move(original));
    
    EXPECT_DOUBLE_EQ(moved.getManagementFeePercentage(), 7.5);
    EXPECT_EQ(moved.getLeaseRenewalNoticeDays(), 45);
    
    EXPECT_EQ(moved.getAvailableProperties().size(), 0);
    EXPECT_EQ(moved.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(moved.calculateTotalMonthlyRevenue(), 1500.0);
    
    auto contract = moved.findContractByProperty(property1->getId());
    EXPECT_NE(contract, nullptr);
}

TEST_F(RentalManagerTest, MoveAssignment) {
    RentalManager original(7.5, 45);
    
    original.addProperty(property1);
    original.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    original.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly");
    
    RentalManager moved(10.0, 60);
    moved.addProperty(property2);
    moved.addTenant(tenant2);
    
    moved = std::move(original);
    
    EXPECT_DOUBLE_EQ(moved.getManagementFeePercentage(), 7.5);
    EXPECT_EQ(moved.getLeaseRenewalNoticeDays(), 45);
    
    EXPECT_EQ(moved.getAvailableProperties().size(), 0);
    EXPECT_EQ(moved.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(moved.calculateTotalMonthlyRevenue(), 1500.0);
}

TEST_F(RentalManagerTest, SelfAssignment) {
    RentalManager manager(5.0, 30);
    
    manager.addProperty(property1);
    manager.addTenant(tenant1);
    
    auto startDate = currentTime;
    auto endDate = startDate + std::chrono::hours(24 * 30 * 12);
    
    manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate, 1500.0, 750.0, "monthly");
    
    manager = manager;
    
    EXPECT_DOUBLE_EQ(manager.getManagementFeePercentage(), 5.0);
    EXPECT_EQ(manager.getLeaseRenewalNoticeDays(), 30);
    
    EXPECT_EQ(manager.getAvailableProperties().size(), 0);
    EXPECT_EQ(manager.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 1500.0);
}

TEST_F(RentalManagerTest, MixedOperationsComplexScenario) {
    RentalManager manager(8.0, 30);
    
    manager.addProperty(property1);
    manager.addProperty(property2);
    manager.addProperty(property3);
    
    manager.addTenant(tenant1);
    manager.addTenant(tenant2);
    
    EXPECT_EQ(manager.getAvailableProperties().size(), 3);
    EXPECT_TRUE(manager.getActiveTenants().empty());
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), 100.0);
    
    auto startDate = currentTime;
    auto endDate1 = startDate + std::chrono::hours(24 * 30 * 6);
    auto endDate2 = startDate + std::chrono::hours(24 * 30 * 12);
    
    EXPECT_TRUE(manager.createRentalContract(property1, tenant1, owner1,
        startDate, endDate1, 1500.0, 750.0, "monthly"));
    
    EXPECT_EQ(manager.getAvailableProperties().size(), 2);
    EXPECT_EQ(manager.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 1500.0);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), (2.0 / 3.0) * 100.0);
    
    EXPECT_TRUE(manager.createRentalContract(property2, tenant2, owner2,
        startDate, endDate2, 2500.0, 1250.0, "monthly"));
    
    EXPECT_EQ(manager.getAvailableProperties().size(), 1);
    EXPECT_EQ(manager.getActiveTenants().size(), 2);
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 4000.0);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), (1.0 / 3.0) * 100.0);
    
    auto expiringContracts = manager.getExpiringContracts(210);
    EXPECT_EQ(expiringContracts.size(), 1);
    EXPECT_EQ(expiringContracts[0]->getProperty()->getId(), "PROP001");
    
    auto contract = manager.findContractByProperty(property1->getId());
    ASSERT_NE(contract, nullptr);
    EXPECT_TRUE(manager.terminateRentalContract(contract->getContractId()));
    
    EXPECT_EQ(manager.getAvailableProperties().size(), 2);
    EXPECT_EQ(manager.getActiveTenants().size(), 1);
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 2500.0);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), (2.0 / 3.0) * 100.0);
    
    EXPECT_FALSE(manager.canTenantAffordProperty(tenant1, property3));
    EXPECT_TRUE(manager.createRentalContract(property3, tenant1, owner1,
        startDate, endDate2, 2000.0, 1000.0, "monthly"));
    
    EXPECT_EQ(manager.getAvailableProperties().size(), 1);
    EXPECT_EQ(manager.getActiveTenants().size(), 2);
    EXPECT_DOUBLE_EQ(manager.calculateTotalMonthlyRevenue(), 4500.0);
    EXPECT_DOUBLE_EQ(manager.calculateVacancyRate(), (1.0 / 3.0) * 100.0);
    
    EXPECT_DOUBLE_EQ(manager.calculateManagementFees(4500.0), 360.0);
}
