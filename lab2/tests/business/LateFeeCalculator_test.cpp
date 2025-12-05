#include <gtest/gtest.h>
#include "../../include/business/LateFeeCalculator.h"
#include "../../include/business/CustomerManager.h"

class LateFeeCalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto customerManager = std::make_shared<CustomerManager>();
        lateFeeCalculator = std::make_shared<LateFeeCalculator>(customerManager);
    }
    
    std::shared_ptr<LateFeeCalculator> lateFeeCalculator;
};

TEST_F(LateFeeCalculatorTest, CalculateLateFee) {
    double fee = lateFeeCalculator->calculateLateFee("VEHICLE", 50.0, 3, "REGULAR");
    EXPECT_GE(fee, 0.0);
}

TEST_F(LateFeeCalculatorTest, CalculateProgressivePenalty) {
    double penalty = lateFeeCalculator->calculateProgressivePenalty(5, 100.0);
    EXPECT_GE(penalty, 0.0);
}

TEST_F(LateFeeCalculatorTest, ShouldSuspendRentalPrivileges) {
    bool shouldSuspend = lateFeeCalculator->shouldSuspendRentalPrivileges("CUST001", 10);
    EXPECT_FALSE(shouldSuspend);
}

TEST_F(LateFeeCalculatorTest, CalculateLostRevenuePenalty) {
    double penalty = lateFeeCalculator->calculateLostRevenuePenalty("VEHICLE", 5);
    EXPECT_GE(penalty, 0.0);
}

TEST_F(LateFeeCalculatorTest, IsGracePeriodApplicable) {
    bool applicable = lateFeeCalculator->isGracePeriodApplicable("VIP", 1);
    EXPECT_TRUE(applicable);
}