#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class RentalContractTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto address = std::make_unique<Address>("Main St", "City", "State", "11111");
        auto status = std::make_unique<PropertyStatus>(
            "AVAILABLE", "Available", true, false, false,
            std::chrono::system_clock::now(), "", 0.0, 0
        );
        auto type = std::make_unique<PropertyType>(
            "TYPE1", "residential", "House", 1, 5, 50.0, 200.0, false, "residential", 1.5
        );
        auto ownerAddress = std::make_unique<Address>("Owner St", "City", "State", "22222");
        
        property = std::make_shared<Property>("P1", 120.0, 1500.0, std::move(address), std::move(status), std::move(type));
        tenant = std::make_shared<Tenant>("T1", "Alice", "Smith", "alice@email.com", "1234567890", nullptr, 500, 2000, false, 1);
        owner = std::make_shared<Owner>("O1", "Bob", "Owner", "bob@email.com", "5555555555", std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");
        
        startDate = std::chrono::system_clock::now();
        endDate = startDate + std::chrono::hours(12 * 30 * 24);
    }
    
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<Owner> owner;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
};

TEST_F(RentalContractTest, ConstructorAndGetters) {
    RentalContract contract("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    
    EXPECT_EQ(contract.getContractId(), "CNTR001");
    EXPECT_EQ(contract.getProperty(), property.get());
    EXPECT_EQ(contract.getTenant(), tenant.get());
    EXPECT_EQ(contract.getOwner(), owner.get());
    EXPECT_EQ(contract.getStartDate(), startDate);
    EXPECT_EQ(contract.getEndDate(), endDate);
    EXPECT_DOUBLE_EQ(contract.getMonthlyRent(), 2000.0);
    EXPECT_DOUBLE_EQ(contract.getSecurityDeposit(), 3000.0);
    EXPECT_EQ(contract.getPaymentTerms(), "monthly");
    EXPECT_TRUE(contract.getIsActive());
}

TEST_F(RentalContractTest, AlternativeConstructor) {
    RentalContract contract("CNTR001", property, tenant, 2000.0, startDate, endDate);
    
    EXPECT_EQ(contract.getContractId(), "CNTR001");
    EXPECT_EQ(contract.getProperty(), property.get());
    EXPECT_EQ(contract.getTenant(), tenant.get());
    EXPECT_EQ(contract.getOwner(), nullptr);
    EXPECT_DOUBLE_EQ(contract.getMonthlyRent(), 2000.0);
    EXPECT_DOUBLE_EQ(contract.getSecurityDeposit(), 0.0);
    EXPECT_EQ(contract.getPaymentTerms(), "standard");
    EXPECT_TRUE(contract.getIsActive());
}

TEST_F(RentalContractTest, Setters) {
    RentalContract contract("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    
    contract.setMonthlyRent(2200.0);
    EXPECT_DOUBLE_EQ(contract.getMonthlyRent(), 2200.0);
    
    contract.setPaymentTerms("quarterly");
    EXPECT_EQ(contract.getPaymentTerms(), "quarterly");
}

TEST_F(RentalContractTest, TerminateContract) {
    RentalContract contract("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    
    EXPECT_TRUE(contract.getIsActive());
    contract.terminateContract();
    EXPECT_FALSE(contract.getIsActive());
}

TEST_F(RentalContractTest, IsContractValid) {
    auto pastDate = std::chrono::system_clock::now() - std::chrono::hours(24 * 30 * 13);
    auto pastEndDate = pastDate + std::chrono::hours(12 * 30 * 24);
    
    RentalContract expiredContract("CNTR001", property, tenant, owner, pastDate, pastEndDate, 2000.0, 3000.0, "monthly");
    EXPECT_FALSE(expiredContract.isContractValid());
    
    RentalContract activeContract("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    EXPECT_TRUE(activeContract.isContractValid());
    
    activeContract.terminateContract();
    EXPECT_FALSE(activeContract.isContractValid());
}

TEST_F(RentalContractTest, GetRemainingDays) {
    auto now = std::chrono::system_clock::now();
    auto futureEndDate = now + std::chrono::hours(10 * 24);
    
    RentalContract contract("CNTR001", property, tenant, owner, now, futureEndDate, 2000.0, 3000.0, "monthly");
    
    int remainingDays = contract.getRemainingDays();
    EXPECT_GE(remainingDays, 9);
    EXPECT_LE(remainingDays, 10);
    
    auto pastEndDate = now - std::chrono::hours(24);
    RentalContract expiredContract("CNTR002", property, tenant, owner, now - std::chrono::hours(48), pastEndDate, 2000.0, 3000.0, "monthly");
    EXPECT_EQ(expiredContract.getRemainingDays(), 0);
}

TEST_F(RentalContractTest, CalculateTotalContractValue) {
    auto start = std::chrono::system_clock::now();
    auto end = start + std::chrono::hours(12 * 30 * 24);
    
    RentalContract contract("CNTR001", property, tenant, owner, start, end, 2000.0, 3000.0, "monthly");
    
    double totalValue = contract.calculateTotalContractValue();
    EXPECT_DOUBLE_EQ(totalValue, 2000.0 * 12);
}

TEST_F(RentalContractTest, IsRentPaymentDue) {
    auto start = std::chrono::system_clock::now();
    auto end = start + std::chrono::hours(12 * 30 * 24);
    
    RentalContract contract("CNTR001", property, tenant, owner, start, end, 2000.0, 3000.0, "monthly");
    
    EXPECT_TRUE(contract.isRentPaymentDue(start));
    
    auto monthLater = start + std::chrono::hours(30 * 24);
    EXPECT_TRUE(contract.isRentPaymentDue(monthLater));
    
    auto twoMonthsLater = start + std::chrono::hours(60 * 24);
    EXPECT_TRUE(contract.isRentPaymentDue(twoMonthsLater));
    
    auto halfMonth = start + std::chrono::hours(15 * 24);
    EXPECT_FALSE(contract.isRentPaymentDue(halfMonth));
    
    auto afterEnd = end + std::chrono::hours(24);
    EXPECT_FALSE(contract.isRentPaymentDue(afterEnd));
}

TEST_F(RentalContractTest, CalculateEarlyTerminationFee) {
    RentalContract contract("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    
    EXPECT_DOUBLE_EQ(contract.calculateEarlyTerminationFee(0), 0.0);
    EXPECT_DOUBLE_EQ(contract.calculateEarlyTerminationFee(6), 2000.0 * 6 * 0.5);
    EXPECT_DOUBLE_EQ(contract.calculateEarlyTerminationFee(12), 2000.0 * 12 * 0.5);
    EXPECT_DOUBLE_EQ(contract.calculateEarlyTerminationFee(-1), 0.0);
}

TEST_F(RentalContractTest, CopyConstructor) {
    RentalContract original("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    original.terminateContract();
    
    RentalContract copy(original);
    
    EXPECT_EQ(copy.getContractId(), "CNTR001");
    EXPECT_DOUBLE_EQ(copy.getMonthlyRent(), 2000.0);
    EXPECT_DOUBLE_EQ(copy.getSecurityDeposit(), 3000.0);
    EXPECT_EQ(copy.getPaymentTerms(), "monthly");
    EXPECT_FALSE(copy.getIsActive());
}

TEST_F(RentalContractTest, CopyAssignment) {
    RentalContract original("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    RentalContract copy("CNTR002", property, tenant, nullptr, startDate, endDate, 1500.0, 2000.0, "quarterly");
    
    copy = original;
    
    EXPECT_EQ(copy.getContractId(), "CNTR001");
    EXPECT_DOUBLE_EQ(copy.getMonthlyRent(), 2000.0);
    EXPECT_DOUBLE_EQ(copy.getSecurityDeposit(), 3000.0);
    EXPECT_EQ(copy.getPaymentTerms(), "monthly");
    EXPECT_EQ(copy.getOwner(), owner.get());
}

TEST_F(RentalContractTest, MoveConstructor) {
    RentalContract original("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    
    RentalContract moved(std::move(original));
    
    EXPECT_EQ(moved.getContractId(), "CNTR001");
    EXPECT_DOUBLE_EQ(moved.getMonthlyRent(), 2000.0);
    EXPECT_DOUBLE_EQ(moved.getSecurityDeposit(), 3000.0);
    EXPECT_EQ(moved.getPaymentTerms(), "monthly");
    EXPECT_TRUE(moved.getIsActive());
}

TEST_F(RentalContractTest, MoveAssignment) {
    RentalContract original("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    RentalContract moved("CNTR002", property, tenant, nullptr, startDate, endDate, 1500.0, 2000.0, "quarterly");
    
    moved = std::move(original);
    
    EXPECT_EQ(moved.getContractId(), "CNTR001");
    EXPECT_DOUBLE_EQ(moved.getMonthlyRent(), 2000.0);
    EXPECT_DOUBLE_EQ(moved.getSecurityDeposit(), 3000.0);
    EXPECT_EQ(moved.getPaymentTerms(), "monthly");
    EXPECT_EQ(moved.getOwner(), owner.get());
}

TEST_F(RentalContractTest, SharedPtrGetters) {
    RentalContract contract("CNTR001", property, tenant, owner, startDate, endDate, 2000.0, 3000.0, "monthly");
    
    EXPECT_EQ(contract.getPropertySharedPtr(), property);
    EXPECT_EQ(contract.getTenantSharedPtr(), tenant);
    EXPECT_EQ(contract.getOwnerSharedPtr(), owner);
}