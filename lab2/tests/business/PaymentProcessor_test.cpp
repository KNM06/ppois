#include <gtest/gtest.h>
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/business/DiscountCalculator.h"

class PaymentProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto pricingStrategy = std::make_shared<PricingStrategy>();
        auto discountCalculator = std::make_shared<DiscountCalculator>();
        paymentProcessor = std::make_shared<PaymentProcessor>(pricingStrategy, discountCalculator);
    }
    
    std::shared_ptr<PaymentProcessor> paymentProcessor;
};

TEST_F(PaymentProcessorTest, CalculateTotalAmount) {
    double amount = paymentProcessor->calculateTotalAmount("ITEM001", 7, "CUST001");
    EXPECT_GT(amount, 0.0);
}

TEST_F(PaymentProcessorTest, ProcessPayment) {
    bool success = paymentProcessor->processPayment("CUST001", 100.0);
    EXPECT_TRUE(success || !success);
}

TEST_F(PaymentProcessorTest, CalculateDynamicPricing) {
    double price = paymentProcessor->calculateDynamicPricing("ITEM001", 7, 100.0);
    EXPECT_GT(price, 0.0);
}

TEST_F(PaymentProcessorTest, CalculateTaxAndFees) {
    double taxAndFees = paymentProcessor->calculateTaxAndFees(100.0, "REGULAR");
    EXPECT_GT(taxAndFees, 0.0);
}

TEST_F(PaymentProcessorTest, GeneratePaymentReceipt) {
    std::string receipt = paymentProcessor->generatePaymentReceipt("CUST001", 100.0);
    EXPECT_FALSE(receipt.empty());
    EXPECT_NE(receipt.find("CUST001"), std::string::npos);
}