#include <gtest/gtest.h>
#include <memory>
#include <chrono>

#include "../../include/finance/Receipt.h"
#include "../../include/finance/Payment.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"

using namespace std;

static shared_ptr<Address> makeAddr() {
    return make_shared<Address>("Street", "City", "State", "00000", "USA", 10.0, 20.0);
}

static unique_ptr<Address> makeAddrU() {
    return make_unique<Address>("Street", "City", "State", "00000", "USA", 10.0, 20.0);
}

static shared_ptr<Tenant> makeTenant() {
    return make_shared<Tenant>(
        "T1", "John", "Doe", "john@mail.com", "123",
        makeAddrU(), 700, 5000, false, 1
    );
}

static shared_ptr<Owner> makeOwner() {
    return make_shared<Owner>(
        "O1", "Bob", "Owner", "owner@mail.com", "999",
        makeAddrU(), "TAX123", 100.0, true, "bank"
    );
}

static shared_ptr<Property> makeProperty() {
    return make_shared<Property>(
        "P1", 100.0, 1500.0, makeAddrU(),
        nullptr, nullptr
    );
}

static shared_ptr<RentalContract> makeContract() {
    auto now = chrono::system_clock::now();
    return make_shared<RentalContract>(
        "C1", makeProperty(), makeTenant(), makeOwner(),
        now, now + chrono::hours(24), 1500.0, 500.0, "monthly"
    );
}

static shared_ptr<Payment> makePayment() {
    auto now = chrono::system_clock::now();
    return make_shared<Payment>(
        "Pay1", makeTenant(), makeContract(), 1000.0,
        now, "card", "completed", "TX1", 0.0, "rent"
    );
}

class ReceiptTest : public ::testing::Test {
protected:
    shared_ptr<Payment> payment;
    chrono::system_clock::time_point now;

    void SetUp() override {
        payment = makePayment();
        now = chrono::system_clock::now();
    }

    Receipt makeReceipt(const string& tax = "10%", const string& desc = "rent payment") {
        return Receipt(
            "R1", payment, now,
            "DIG-001", 1000.0, "electronic",
            "TenantName", "ManagerName",
            desc, "issued", tax
        );
    }
};

TEST_F(ReceiptTest, GettersWorkCorrectly) {
    Receipt r = makeReceipt();

    EXPECT_EQ(r.getReceiptId(), "R1");
    EXPECT_EQ(r.getPayment(), payment.get());
    EXPECT_EQ(r.getReceiptNumber(), "DIG-001");
    EXPECT_EQ(r.getAmount(), 1000.0);
    EXPECT_EQ(r.getPaymentMethod(), "electronic");
    EXPECT_EQ(r.getReceivedFrom(), "TenantName");
    EXPECT_EQ(r.getReceivedBy(), "ManagerName");
    EXPECT_EQ(r.getDescription(), "rent payment");
    EXPECT_EQ(r.getStatus(), "issued");
    EXPECT_EQ(r.getTaxDetails(), "10%");
}

TEST_F(ReceiptTest, SettersWorkCorrectly) {
    Receipt r = makeReceipt();

    r.setStatus("cancelled");
    EXPECT_EQ(r.getStatus(), "cancelled");

    r.setDescription("updated");
    EXPECT_EQ(r.getDescription(), "updated");
}

TEST_F(ReceiptTest, IsDigitalReceipt) {
    Receipt r = makeReceipt();
    EXPECT_TRUE(r.isDigitalReceipt());

    Receipt r2(
        "R2", payment, now, "001", 100, "cash",
        "A", "B", "desc", "issued", "10%"
    );
    EXPECT_FALSE(r2.isDigitalReceipt());
}

TEST_F(ReceiptTest, IsValidForTax) {
    Receipt r = makeReceipt("10%");
    EXPECT_TRUE(r.isValidForTax());

    Receipt r2 = makeReceipt("");
    EXPECT_FALSE(r2.isValidForTax());
}

TEST_F(ReceiptTest, CalculateTaxAmount) {
    Receipt r10 = makeReceipt("10%");
    EXPECT_DOUBLE_EQ(r10.calculateTaxAmount(), 100.0);

    Receipt r15 = makeReceipt("15%");
    EXPECT_DOUBLE_EQ(r15.calculateTaxAmount(), 150.0);

    Receipt r20 = makeReceipt("20%");
    EXPECT_DOUBLE_EQ(r20.calculateTaxAmount(), 200.0);

    Receipt r0 = makeReceipt("none");
    EXPECT_DOUBLE_EQ(r0.calculateTaxAmount(), 0.0);
}

TEST_F(ReceiptTest, ReceiptTypes) {
    Receipt rent = makeReceipt("10%", "rent for January");
    EXPECT_TRUE(rent.isRentReceipt());
    EXPECT_EQ(rent.getReceiptType(), "rent");

    Receipt dep = makeReceipt("10%", "security deposit");
    EXPECT_TRUE(dep.isSecurityDepositReceipt());
    EXPECT_EQ(dep.getReceiptType(), "security_deposit");

    Receipt maint = makeReceipt("10%", "maintenance fee");
    EXPECT_EQ(maint.getReceiptType(), "maintenance");

    Receipt other = makeReceipt("10%", "misc");
    EXPECT_EQ(other.getReceiptType(), "other");
}

TEST_F(ReceiptTest, RequiresArchiving) {
    Receipt r = makeReceipt();
    EXPECT_TRUE(r.requiresArchiving());

    Receipt small(
        "R", payment, now, "001", 10.0, "cash",
        "A", "B", "misc", "issued", ""
    );
    EXPECT_FALSE(small.requiresArchiving());
}

TEST_F(ReceiptTest, DaysSinceIssue) {
    Receipt r = makeReceipt();
    auto later = now + chrono::hours(48);

    EXPECT_EQ(r.getDaysSinceIssue(later), 2);
}

TEST_F(ReceiptTest, FinancialYearCalculation) {
    // April 5, 2024 -> FY 2024-2025
    tm t = {};
    t.tm_year = 2024 - 1900;
    t.tm_mon = 3; // April
    t.tm_mday = 5;

    auto tp = chrono::system_clock::from_time_t(std::mktime(&t));

    Receipt r(
        "R1", payment, tp, "001", 100.0, "cash",
        "A", "B", "desc", "issued", "10%"
    );

    EXPECT_EQ(r.getFinancialYear(), "2024-2025");
}

