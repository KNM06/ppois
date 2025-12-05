#include <gtest/gtest.h>
#include "../../include/finance/Payment.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Address.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/Owner.h"
#include <memory>
#include <chrono>

class PaymentTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto tenantAddress = std::make_unique<Address>("456 Tenant St", "New York", "NY", "10002", "USA", 40.7130, -74.0050);
        tenant = std::make_shared<Tenant>("T001", "John", "Doe", "john.doe@email.com", "555-0101", 
                                         std::move(tenantAddress), 750.0, 8000.0, false, 2);
        
        auto propertyAddress = std::make_unique<Address>("123 Main St", "New York", "NY", "10001", "USA", 40.7128, -74.0060);
        auto propertyStatus = std::make_unique<PropertyStatus>("AVAILABLE", "Available", true, false, false,
                                                              std::chrono::system_clock::now(), "", 0.0, 0);
        auto propertyType = std::make_unique<PropertyType>("APT", "residential", "Apartment", 1, 4, 30.0, 200.0, false, "residential", 1.2);
        auto property = std::make_shared<Property>("PROP001", 85.5, 2500.0, std::move(propertyAddress), 
                                                  std::move(propertyStatus), std::move(propertyType));
        
        auto ownerAddress = std::make_unique<Address>("789 Owner Ave", "New York", "NY", "10003", "USA", 40.7140, -74.0040);
        owner = std::make_shared<Owner>("O001", "Robert", "Johnson", "robert.johnson@email.com", "555-0201", 
                                       std::move(ownerAddress), "123456789", 100.0, true, "direct_deposit");
        
        auto startDate = std::chrono::system_clock::now();
        auto endDate = startDate + std::chrono::hours(365 * 24);
        contract = std::make_shared<RentalContract>("CNT001", property, tenant, owner, startDate, endDate, 
                                                   2500.0, 5000.0, "Net 30 days");
        
        paymentDate = std::chrono::system_clock::now();
    }
    
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<Owner> owner;
    std::shared_ptr<RentalContract> contract;
    std::chrono::system_clock::time_point paymentDate;
};

TEST_F(PaymentTest, SimplifiedConstructor) {
    Payment payment("PAY001", "T001", 2500.0, "completed", paymentDate, true);
    
    EXPECT_EQ(payment.getPaymentId(), "PAY001");
    EXPECT_DOUBLE_EQ(payment.getAmount(), 2500.0);
    EXPECT_EQ(payment.getStatus(), "completed");
    EXPECT_EQ(payment.getPaymentDate(), paymentDate);
    EXPECT_EQ(payment.getPaymentMethod(), "unknown");
    EXPECT_EQ(payment.getTransactionId(), "");
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 0.0);
    EXPECT_EQ(payment.getPaymentPeriod(), "rent");
    EXPECT_EQ(payment.getTenant(), nullptr);
    EXPECT_EQ(payment.getContract(), nullptr);
}

TEST_F(PaymentTest, SimplifiedConstructorNonRent) {
    Payment payment("PAY001", "T001", 5000.0, "pending", paymentDate, false);
    
    EXPECT_EQ(payment.getPaymentId(), "PAY001");
    EXPECT_DOUBLE_EQ(payment.getAmount(), 5000.0);
    EXPECT_EQ(payment.getStatus(), "pending");
    EXPECT_EQ(payment.getPaymentDate(), paymentDate);
    EXPECT_EQ(payment.getPaymentMethod(), "unknown");
    EXPECT_EQ(payment.getTransactionId(), "");
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 0.0);
    EXPECT_EQ(payment.getPaymentPeriod(), "other");
    EXPECT_EQ(payment.getTenant(), nullptr);
    EXPECT_EQ(payment.getContract(), nullptr);
}

TEST_F(PaymentTest, ConstructorEdgeCases) {
    Payment payment("", nullptr, nullptr, -1000.0, 
                   std::chrono::system_clock::time_point(), 
                   "", "", "", -50.0, "");
    
    EXPECT_EQ(payment.getPaymentId(), "");
    EXPECT_EQ(payment.getTenant(), nullptr);
    EXPECT_EQ(payment.getContract(), nullptr);
    EXPECT_DOUBLE_EQ(payment.getAmount(), -1000.0);
    EXPECT_EQ(payment.getPaymentMethod(), "");
    EXPECT_EQ(payment.getStatus(), "");
    EXPECT_EQ(payment.getTransactionId(), "");
    EXPECT_DOUBLE_EQ(payment.getLateFee(), -50.0);
    EXPECT_EQ(payment.getPaymentPeriod(), "");
}

TEST_F(PaymentTest, CopyConstructor) {
    Payment payment1("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 100.0, "January 2024 Rent");
    
    Payment payment2(payment1);
    
    EXPECT_EQ(payment2.getPaymentId(), "PAY001");
    EXPECT_EQ(payment2.getTenant(), tenant.get());
    EXPECT_EQ(payment2.getContract(), contract.get());
    EXPECT_DOUBLE_EQ(payment2.getAmount(), 2500.0);
    EXPECT_EQ(payment2.getPaymentDate(), paymentDate);
    EXPECT_EQ(payment2.getPaymentMethod(), "credit_card");
    EXPECT_EQ(payment2.getStatus(), "completed");
    EXPECT_EQ(payment2.getTransactionId(), "TXN123456");
    EXPECT_DOUBLE_EQ(payment2.getLateFee(), 100.0);
    EXPECT_EQ(payment2.getPaymentPeriod(), "January 2024 Rent");
}

TEST_F(PaymentTest, CopyAssignment) {
    Payment payment1("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 100.0, "January 2024 Rent");
    
    Payment payment2("PAY002", nullptr, nullptr, 1000.0, 
                    std::chrono::system_clock::time_point(), 
                    "cash", "pending", "", 0.0, "Other");
    
    payment2 = payment1;
    
    EXPECT_EQ(payment2.getPaymentId(), "PAY001");
    EXPECT_EQ(payment2.getTenant(), tenant.get());
    EXPECT_EQ(payment2.getContract(), contract.get());
    EXPECT_DOUBLE_EQ(payment2.getAmount(), 2500.0);
    EXPECT_EQ(payment2.getPaymentDate(), paymentDate);
    EXPECT_EQ(payment2.getPaymentMethod(), "credit_card");
    EXPECT_EQ(payment2.getStatus(), "completed");
    EXPECT_EQ(payment2.getTransactionId(), "TXN123456");
    EXPECT_DOUBLE_EQ(payment2.getLateFee(), 100.0);
    EXPECT_EQ(payment2.getPaymentPeriod(), "January 2024 Rent");
}

TEST_F(PaymentTest, MoveConstructor) {
    Payment payment1("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 100.0, "January 2024 Rent");
    
    Payment payment2(std::move(payment1));
    
    EXPECT_EQ(payment2.getPaymentId(), "PAY001");
    EXPECT_EQ(payment2.getTenant(), tenant.get());
    EXPECT_EQ(payment2.getContract(), contract.get());
    EXPECT_DOUBLE_EQ(payment2.getAmount(), 2500.0);
    EXPECT_EQ(payment2.getPaymentDate(), paymentDate);
    EXPECT_EQ(payment2.getPaymentMethod(), "credit_card");
    EXPECT_EQ(payment2.getStatus(), "completed");
    EXPECT_EQ(payment2.getTransactionId(), "TXN123456");
    EXPECT_DOUBLE_EQ(payment2.getLateFee(), 100.0);
    EXPECT_EQ(payment2.getPaymentPeriod(), "January 2024 Rent");
}

TEST_F(PaymentTest, MoveAssignment) {
    Payment payment1("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 100.0, "January 2024 Rent");
    
    Payment payment2("PAY002", nullptr, nullptr, 1000.0, 
                    std::chrono::system_clock::time_point(), 
                    "cash", "pending", "", 0.0, "Other");
    
    payment2 = std::move(payment1);
    
    EXPECT_EQ(payment2.getPaymentId(), "PAY001");
    EXPECT_EQ(payment2.getTenant(), tenant.get());
    EXPECT_EQ(payment2.getContract(), contract.get());
    EXPECT_DOUBLE_EQ(payment2.getAmount(), 2500.0);
    EXPECT_EQ(payment2.getPaymentDate(), paymentDate);
    EXPECT_EQ(payment2.getPaymentMethod(), "credit_card");
    EXPECT_EQ(payment2.getStatus(), "completed");
    EXPECT_EQ(payment2.getTransactionId(), "TXN123456");
    EXPECT_DOUBLE_EQ(payment2.getLateFee(), 100.0);
    EXPECT_EQ(payment2.getPaymentPeriod(), "January 2024 Rent");
}

TEST_F(PaymentTest, UpdateLateFeeEdgeCases) {
    Payment payment("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                   "completed", "TXN123456", 100.0, "January 2024 Rent");
    
    payment.updateLateFee(-50.0);
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 0.0);
    
    payment.updateLateFee(0.0);
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 0.0);
    
    payment.updateLateFee(75.0);
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 75.0);
    
    payment.updateLateFee(200.0);
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 200.0);
}

TEST_F(PaymentTest, IsPaymentOnTimeEdgeCases) {
    auto dueDate = std::chrono::system_clock::now();
    
    Payment payment1("PAY001", tenant, contract, 2500.0, dueDate, "credit_card", 
                    "completed", "TXN123456", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment1.isPaymentOnTime(dueDate));
    
    Payment payment2("PAY002", tenant, contract, 2500.0, dueDate - std::chrono::hours(1), 
                    "credit_card", "completed", "TXN123457", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment2.isPaymentOnTime(dueDate));
    
    Payment payment3("PAY003", tenant, contract, 2500.0, dueDate + std::chrono::hours(1), 
                    "credit_card", "completed", "TXN123458", 50.0, "January 2024 Rent");
    EXPECT_FALSE(payment3.isPaymentOnTime(dueDate));
    
    Payment payment4("PAY004", tenant, contract, 2500.0, dueDate - std::chrono::hours(24*365), 
                    "credit_card", "completed", "TXN123459", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment4.isPaymentOnTime(dueDate));
}

TEST_F(PaymentTest, CalculateTotalAmountWithFeesEdgeCases) {
    Payment payment1("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 100.0, "January 2024 Rent");
    EXPECT_DOUBLE_EQ(payment1.calculateTotalAmountWithFees(), 2600.0);
    
    Payment payment2("PAY002", tenant, contract, 0.0, paymentDate, "credit_card", 
                    "completed", "TXN123457", 0.0, "January 2024 Rent");
    EXPECT_DOUBLE_EQ(payment2.calculateTotalAmountWithFees(), 0.0);
    
    Payment payment3("PAY003", tenant, contract, -1000.0, paymentDate, "credit_card", 
                    "completed", "TXN123458", 200.0, "January 2024 Rent");
    EXPECT_DOUBLE_EQ(payment3.calculateTotalAmountWithFees(), -800.0);
    
    Payment payment4("PAY004", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123459", -50.0, "January 2024 Rent");
    EXPECT_DOUBLE_EQ(payment4.calculateTotalAmountWithFees(), 2450.0);
}

TEST_F(PaymentTest, IsRentPaymentEdgeCases) {
    Payment payment1("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment1.isRentPayment());
    
    Payment payment2("PAY002", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123457", 0.0, "February rent");
    EXPECT_TRUE(payment2.isRentPayment());
    
    Payment payment3("PAY003", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123458", 0.0, "RENT March");
    EXPECT_FALSE(payment3.isRentPayment());
    
    Payment payment4("PAY004", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123459", 0.0, "Security Deposit");
    EXPECT_FALSE(payment4.isRentPayment());
    
    Payment payment5("PAY005", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123460", 0.0, "Maintenance Fee");
    EXPECT_FALSE(payment5.isRentPayment());
    
    Payment payment6("PAY006", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123461", 0.0, "");
    EXPECT_FALSE(payment6.isRentPayment());
    
    Payment payment7("PAY007", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123462", 0.0, "RentAdvance");
    EXPECT_TRUE(payment7.isRentPayment());
}

TEST_F(PaymentTest, IsSecurityDepositEdgeCases) {
    Payment payment1("PAY001", tenant, contract, 5000.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 0.0, "Security Deposit");
    EXPECT_TRUE(payment1.isSecurityDeposit());
    
    Payment payment2("PAY002", tenant, contract, 5000.0, paymentDate, "credit_card", 
                    "completed", "TXN123457", 0.0, "security deposit");
    EXPECT_TRUE(payment2.isSecurityDeposit());
    
    Payment payment3("PAY003", tenant, contract, 5000.0, paymentDate, "credit_card", 
                    "completed", "TXN123458", 0.0, "Deposit Refund");
    EXPECT_TRUE(payment3.isSecurityDeposit());
    
    Payment payment4("PAY004", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123459", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment4.isSecurityDeposit());
    
    Payment payment5("PAY005", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123460", 0.0, "");
    EXPECT_FALSE(payment5.isSecurityDeposit());
    
    Payment payment6("PAY006", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123461", 0.0, "Depositary");
    EXPECT_TRUE(payment6.isSecurityDeposit());
}

TEST_F(PaymentTest, RequiresBankTransferEdgeCases) {
    Payment payment1("PAY001", tenant, contract, 6000.0, paymentDate, "credit_card", 
                    "completed", "TXN123456", 0.0, "Security Deposit");
    EXPECT_TRUE(payment1.requiresBankTransfer());
    
    Payment payment2("PAY002", tenant, contract, 5000.0, paymentDate, "credit_card", 
                    "completed", "TXN123457", 0.0, "Security Deposit");
    EXPECT_FALSE(payment2.requiresBankTransfer());
    
    Payment payment3("PAY003", tenant, contract, 5000.1, paymentDate, "credit_card", 
                    "completed", "TXN123458", 0.0, "Security Deposit");
    EXPECT_TRUE(payment3.requiresBankTransfer());
    
    Payment payment4("PAY004", tenant, contract, 1000.0, paymentDate, "bank_transfer", 
                    "completed", "TXN123459", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment4.requiresBankTransfer());
    
    Payment payment5("PAY005", tenant, contract, 0.0, paymentDate, "bank_transfer", 
                    "completed", "TXN123460", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment5.requiresBankTransfer());
    
    Payment payment6("PAY006", tenant, contract, -1000.0, paymentDate, "credit_card", 
                    "completed", "TXN123461", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment6.requiresBankTransfer());
    
    Payment payment7("PAY007", tenant, contract, 1000.0, paymentDate, "cash", 
                    "completed", "TXN123462", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment7.requiresBankTransfer());
    
    Payment payment8("PAY008", tenant, contract, 5000.0, paymentDate, "", 
                    "completed", "TXN123463", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment8.requiresBankTransfer());
}

TEST_F(PaymentTest, CanBeRefundedEdgeCases) {
    Payment payment1("PAY001", tenant, contract, 5000.0, paymentDate, "bank_transfer", 
                    "completed", "TXN123456", 0.0, "Security Deposit");
    EXPECT_TRUE(payment1.canBeRefunded());
    
    Payment payment2("PAY002", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123457", 0.0, "advance payment");
    EXPECT_TRUE(payment2.canBeRefunded());
    
    Payment payment3("PAY003", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123458", 0.0, "Advance Rent");
    EXPECT_FALSE(payment3.canBeRefunded());
    
    Payment payment4("PAY004", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "pending", "TXN123459", 0.0, "Security Deposit");
    EXPECT_FALSE(payment4.canBeRefunded());
    
    Payment payment5("PAY005", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123460", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment5.canBeRefunded());
    
    Payment payment6("PAY006", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "refunded", "TXN123461", 0.0, "Security Deposit");
    EXPECT_FALSE(payment6.canBeRefunded());
    
    Payment payment7("PAY007", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "failed", "TXN123462", 0.0, "Security Deposit");
    EXPECT_FALSE(payment7.canBeRefunded());
    
    Payment payment8("PAY008", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "", "TXN123463", 0.0, "Security Deposit");
    EXPECT_FALSE(payment8.canBeRefunded());
}

TEST_F(PaymentTest, CalculateRefundAmountEdgeCases) {
    Payment payment1("PAY001", tenant, contract, 5000.0, paymentDate, "bank_transfer", 
                    "completed", "TXN123456", 0.0, "Security Deposit");
    EXPECT_DOUBLE_EQ(payment1.calculateRefundAmount(), 4000.0);
    
    Payment payment2("PAY002", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123457", 0.0, "advance payment");
    EXPECT_DOUBLE_EQ(payment2.calculateRefundAmount(), 2500.0);
    
    Payment payment3("PAY003", tenant, contract, 0.0, paymentDate, "credit_card", 
                    "completed", "TXN123458", 0.0, "Security Deposit");
    EXPECT_DOUBLE_EQ(payment3.calculateRefundAmount(), 0.0);
    
    Payment payment4("PAY004", tenant, contract, -1000.0, paymentDate, "credit_card", 
                    "completed", "TXN123459", 0.0, "Security Deposit");
    EXPECT_DOUBLE_EQ(payment4.calculateRefundAmount(), -800.0);
    
    Payment payment5("PAY005", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "pending", "TXN123460", 0.0, "Security Deposit");
    EXPECT_DOUBLE_EQ(payment5.calculateRefundAmount(), 0.0);
    
    Payment payment6("PAY006", tenant, contract, 2500.0, paymentDate, "credit_card", 
                    "completed", "TXN123461", 0.0, "January 2024 Rent");
    EXPECT_DOUBLE_EQ(payment6.calculateRefundAmount(), 0.0);
    
    Payment payment7("PAY007", tenant, contract, 1000.0, paymentDate, "credit_card", 
                    "completed", "TXN123462", 0.0, "Advance");
    EXPECT_DOUBLE_EQ(payment7.calculateRefundAmount(), 0.0);
    
    Payment payment8("PAY008", tenant, contract, 1000.0, paymentDate, "credit_card", 
                    "completed", "TXN123463", 0.0, "ADVANCE Payment");
    EXPECT_DOUBLE_EQ(payment8.calculateRefundAmount(), 0.0);
}

TEST_F(PaymentTest, IsOverdueEdgeCases) {
    auto currentDate = std::chrono::system_clock::now();
    auto oldDate = currentDate - std::chrono::hours(24 * 6);
    auto veryOldDate = currentDate - std::chrono::hours(24 * 10);
    auto recentDate = currentDate - std::chrono::hours(24 * 2);
    
    Payment payment1("PAY001", tenant, contract, 2500.0, veryOldDate, "credit_card", 
                    "pending", "TXN123456", 100.0, "January 2024 Rent");
    EXPECT_TRUE(payment1.isOverdue(currentDate));
    
    Payment payment2("PAY002", tenant, contract, 2500.0, oldDate, "credit_card", 
                    "pending", "TXN123457", 50.0, "January 2024 Rent");
    EXPECT_TRUE(payment2.isOverdue(currentDate));
    
    Payment payment3("PAY003", tenant, contract, 2500.0, recentDate, "credit_card", 
                    "pending", "TXN123458", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment3.isOverdue(currentDate));
    
    Payment payment4("PAY004", tenant, contract, 2500.0, veryOldDate, "credit_card", 
                    "completed", "TXN123459", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment4.isOverdue(currentDate));
    
    Payment payment5("PAY005", tenant, contract, 2500.0, veryOldDate, "credit_card", 
                    "refunded", "TXN123460", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment5.isOverdue(currentDate));
    
    Payment payment6("PAY006", tenant, contract, 2500.0, veryOldDate, "credit_card", 
                    "failed", "TXN123461", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment6.isOverdue(currentDate));
    
    Payment payment7("PAY007", tenant, contract, 2500.0, veryOldDate, "credit_card", 
                    "", "TXN123462", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment7.isOverdue(currentDate));
    
    Payment payment8("PAY008", tenant, contract, 2500.0, veryOldDate, "credit_card", 
                    "cancelled", "TXN123463", 0.0, "January 2024 Rent");
    EXPECT_TRUE(payment8.isOverdue(currentDate));
}

TEST_F(PaymentTest, IsOverdueExactBoundary) {
    auto currentDate = std::chrono::system_clock::now();
    auto exactly5DaysOld = currentDate - std::chrono::hours(24 * 5);
    auto justOver5DaysOld = currentDate - std::chrono::hours(24 * 5 + 1);
    
    Payment payment1("PAY001", tenant, contract, 2500.0, exactly5DaysOld, "credit_card", 
                    "pending", "TXN123456", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment1.isOverdue(currentDate));
    
    Payment payment2("PAY002", tenant, contract, 2500.0, justOver5DaysOld, "credit_card", 
                    "pending", "TXN123457", 0.0, "January 2024 Rent");
    EXPECT_FALSE(payment2.isOverdue(currentDate));
}

TEST_F(PaymentTest, PaymentBeforeDueDate) {
    auto dueDate = std::chrono::system_clock::now();
    auto earlyDate = dueDate - std::chrono::hours(24);
    
    Payment payment("PAY001", tenant, contract, 2500.0, earlyDate, "credit_card", 
                   "completed", "TXN123456", 0.0, "January 2024 Rent");
    
    EXPECT_TRUE(payment.isPaymentOnTime(dueDate));
    EXPECT_DOUBLE_EQ(payment.getLateFee(), 0.0);
}

TEST_F(PaymentTest, SetStatusAndTransactionId) {
    Payment payment("PAY001", tenant, contract, 2500.0, paymentDate, "credit_card", 
                   "pending", "", 0.0, "January 2024 Rent");
    
    payment.setStatus("completed");
    EXPECT_EQ(payment.getStatus(), "completed");
    
    payment.setTransactionId("TXN987654");
    EXPECT_EQ(payment.getTransactionId(), "TXN987654");
    
    payment.setStatus("");
    EXPECT_EQ(payment.getStatus(), "");
    
    payment.setTransactionId("");
    EXPECT_EQ(payment.getTransactionId(), "");
}