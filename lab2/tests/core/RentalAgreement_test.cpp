#include <gtest/gtest.h>
#include "../../include/core/RentalAgreement.h"
#include "../../include/business/InsuranceCalculator.h"
#include "../../include/business/LateFeeCalculator.h"

class RentalAgreementTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto insuranceCalculator = std::make_shared<InsuranceCalculator>();
        auto lateFeeCalculator = std::make_shared<LateFeeCalculator>();
        agreement = std::make_shared<RentalAgreement>("CUST001", "ITEM001", 7, insuranceCalculator, lateFeeCalculator);
        
        agreement->finalizeAgreement(350.0, true);
    }
    
    std::shared_ptr<RentalAgreement> agreement;
};

TEST_F(RentalAgreementTest, ConstructorInitialization) {
    EXPECT_EQ(agreement->getCustomerId(), "CUST001");
    EXPECT_EQ(agreement->getItemId(), "ITEM001");
    EXPECT_FALSE(agreement->getAgreementId().empty());
}

TEST_F(RentalAgreementTest, CalculateEarlyTerminationFee) {
    double fee = agreement->calculateEarlyTerminationFee(3);
    EXPECT_GE(fee, 0.0);
}

TEST_F(RentalAgreementTest, CalculatePotentialLateFee) {
    double fee = agreement->calculatePotentialLateFee(2);
    EXPECT_GE(fee, 0.0);
}

TEST_F(RentalAgreementTest, IsOverdue) {
    bool overdue = agreement->isOverdue("2024-01-10");
    EXPECT_TRUE(overdue);
}

TEST_F(RentalAgreementTest, CalculateSecurityDepositReturn) {
    double deposit = agreement->calculateSecurityDepositReturn("EXCELLENT");
    EXPECT_GE(deposit, 0.0);
}

TEST_F(RentalAgreementTest, ValidateAgreementTerms) {
    bool valid = agreement->validateAgreementTerms();
    EXPECT_TRUE(valid);
}