#include <gtest/gtest.h>
#include "../../include/business/PaymentManager.h"
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"
#include "../../include/core/Owner.h"
#include "../../include/finance/Payment.h"
#include <chrono>

class PaymentManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<Property> property;
    std::shared_ptr<Owner> owner;
    std::shared_ptr<RentalContract> contract;
    PaymentManager manager;

    PaymentManagerTest()
        : manager(0.01, 5, "credit_card") {}

    void SetUp() override {
        tenant = std::make_shared<Tenant>("T1", "John", "Doe", "j@example.com", "123456",
                                          std::make_unique<Address>("Street", "City", "Zip", "Country"),
                                          700, 3000, false, 2);
        owner = std::make_shared<Owner>("O1", "Alice", "Smith", "a@example.com", "987654",
                                        std::make_unique<Address>("Street", "City", "Zip", "Country"),
                                        "123456789", 100, true, "bank_transfer");
        property = std::make_shared<Property>("P1", 50, 1500,
                                              std::make_unique<Address>("Street", "City", "Zip", "Country"),
                                              std::make_unique<PropertyStatus>("AVAILABLE"),
                                              std::make_unique<PropertyType>("APARTMENT"));
        contract = std::make_shared<RentalContract>("C1", property, tenant, owner,
                                                    std::chrono::system_clock::now() - std::chrono::hours(24*30),
                                                    std::chrono::system_clock::now() + std::chrono::hours(24*365),
                                                    1500, 1500, "monthly");
    }
};

TEST_F(PaymentManagerTest, TestSettersAndGetters) {
    manager.setLateFeeRate(0.02);
    manager.setGracePeriodDays(7);
    manager.setDefaultPaymentMethod("cash");
    EXPECT_EQ(manager.getLateFeeRate(), 0.02);
    EXPECT_EQ(manager.getGracePeriodDays(), 7);
    EXPECT_EQ(manager.getDefaultPaymentMethod(), "cash");
}

TEST_F(PaymentManagerTest, TestProcessPaymentAndTenantBalance) {
    auto payment = std::make_shared<Payment>("PAY1", tenant, contract, 1000,
                                             std::chrono::system_clock::now(), "credit_card",
                                             "completed", "", 0.0, "rent");
    EXPECT_TRUE(manager.processPayment(payment));
    EXPECT_DOUBLE_EQ(manager.getTenantBalance(tenant->getTenantId()), -1000);
    auto payments = manager.getPaymentsByTenant(tenant->getTenantId());
    EXPECT_EQ(payments.size(), 1);
    EXPECT_EQ(payments[0]->getAmount(), 1000);
}

TEST_F(PaymentManagerTest, TestRecordRentPayment) {
    EXPECT_TRUE(manager.recordRentPayment(tenant, contract, 1200, std::chrono::system_clock::now(), "", "rent"));
    auto payments = manager.getPaymentsByTenant(tenant->getTenantId());
    EXPECT_EQ(payments.size(), 1);
    EXPECT_EQ(payments[0]->getAmount(), 1200);
}

TEST_F(PaymentManagerTest, TestCalculateLateFee) {
    EXPECT_DOUBLE_EQ(manager.calculateLateFee(1000, 3), 0.0);
    EXPECT_DOUBLE_EQ(manager.calculateLateFee(1000, 8), 0.01 * 1000 * (8 - 5));
}

TEST_F(PaymentManagerTest, TestGetOverduePayments) {
    auto overduePayment = std::make_shared<Payment>("PAY2", tenant, contract, 500,
                                                    std::chrono::system_clock::now() - std::chrono::hours(24*10),
                                                    "credit_card", "pending", "", 0.0, "rent");
    manager.processPayment(overduePayment);
    auto overdue = manager.getOverduePayments(std::chrono::system_clock::now());
    EXPECT_EQ(overdue.size(), 1);
    EXPECT_EQ(overdue[0]->getPaymentId(), "PAY2");
}

TEST_F(PaymentManagerTest, TestCalculateTotalRevenue) {
    manager.recordRentPayment(tenant, contract, 1000, std::chrono::system_clock::now(), "", "rent");
    auto start = std::chrono::system_clock::now() - std::chrono::hours(24*1);
    auto end = std::chrono::system_clock::now() + std::chrono::hours(24*1);
    EXPECT_DOUBLE_EQ(manager.calculateTotalRevenue(start, end), 1000);
}

TEST_F(PaymentManagerTest, TestHasTenantMissedPayments) {
    auto overduePayment = std::make_shared<Payment>("PAY3", tenant, contract, 500,
                                                    std::chrono::system_clock::now() - std::chrono::hours(24*10),
                                                    "credit_card", "pending", "", 0.0, "rent");
    manager.processPayment(overduePayment);
    EXPECT_TRUE(manager.hasTenantMissedPayments(tenant->getTenantId(), 0));
    EXPECT_FALSE(manager.hasTenantMissedPayments(tenant->getTenantId(), 1));
}

TEST_F(PaymentManagerTest, TestCalculateCollectionRate) {
    manager.recordRentPayment(tenant, contract, 1000, std::chrono::system_clock::now(), "", "rent");
    auto payment2 = std::make_shared<Payment>("PAY4", tenant, contract, 500,
                                              std::chrono::system_clock::now(), "credit_card",
                                              "pending", "", 0.0, "rent");
    manager.processPayment(payment2);
    auto start = std::chrono::system_clock::now() - std::chrono::hours(24*1);
    auto end = std::chrono::system_clock::now() + std::chrono::hours(24*1);
    EXPECT_DOUBLE_EQ(manager.calculateCollectionRate(start, end), 50.0);
}
