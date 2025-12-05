#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/core/Tenant.h"
#include "../../include/finance/SecurityDeposit.h"
#include "../../include/finance/RentalContract.h"

class SecurityDepositTest : public ::testing::Test {
protected:
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<RentalContract> contract;
    std::chrono::system_clock::time_point now;

    void SetUp() override {
        tenant = std::make_shared<Tenant>("T1", "John", "Doe", "john@example.com", "1234567890",
                                          nullptr, 700, 5000, true, 2);
        contract = std::make_shared<RentalContract>(
            "C1", nullptr, tenant, nullptr,
            std::chrono::system_clock::now() - std::chrono::hours(24*30),
            std::chrono::system_clock::now() + std::chrono::hours(24*365),
            1500, 1500, "Monthly"
        );
        now = std::chrono::system_clock::now();
    }
};

TEST_F(SecurityDepositTest, ConstructorWithInvalidDeductionNegative) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", -100, "negative deduction", 0, "escrow_account");
    EXPECT_EQ(sd.getDeductions(), -100.0);
}

TEST_F(SecurityDepositTest, ConstructorWithInvalidDeductionExceedsAmount) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 1500, "exceeds amount", 0, "escrow_account");
    EXPECT_EQ(sd.getDeductions(), 1500.0);
}

TEST_F(SecurityDepositTest, SetDeductionsNegative) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "escrow_account");
    sd.setDeductions(-50, "negative");
    EXPECT_EQ(sd.getDeductions(), 0.0);
    EXPECT_EQ(sd.getDeductionReason(), "negative");
}

TEST_F(SecurityDepositTest, SetDeductionsExceedsAmount) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "escrow_account");
    sd.setDeductions(1500, "too much");
    EXPECT_EQ(sd.getDeductions(), 1000.0);
    EXPECT_EQ(sd.getDeductionReason(), "too much");
}

TEST_F(SecurityDepositTest, SetInterestEarnedNegative) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "escrow_account");
    sd.setInterestEarned(-50);
    EXPECT_EQ(sd.getInterestEarned(), 0.0);
}

TEST_F(SecurityDepositTest, CopyConstructor) {
    SecurityDeposit sd1("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "damage", 50, "escrow_account");
    SecurityDeposit sd2(sd1);
    
    EXPECT_EQ(sd2.getDepositId(), "D1");
    EXPECT_EQ(sd2.getTenant(), tenant.get());
    EXPECT_EQ(sd2.getContract(), contract.get());
    EXPECT_EQ(sd2.getAmount(), 1000);
    EXPECT_EQ(sd2.getDeductions(), 100);
    EXPECT_EQ(sd2.getDeductionReason(), "damage");
    EXPECT_EQ(sd2.getInterestEarned(), 50);
    EXPECT_EQ(sd2.getStatus(), "held");
}

TEST_F(SecurityDepositTest, CopyAssignment) {
    SecurityDeposit sd1("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "damage", 50, "escrow_account");
    SecurityDeposit sd2("D2", nullptr, nullptr, 500, now, now + std::chrono::hours(24*100),
                       "released", 0, "", 0, "");
    
    sd2 = sd1;
    
    EXPECT_EQ(sd2.getDepositId(), "D1");
    EXPECT_EQ(sd2.getTenant(), tenant.get());
    EXPECT_EQ(sd2.getContract(), contract.get());
    EXPECT_EQ(sd2.getAmount(), 1000);
    EXPECT_EQ(sd2.getDeductions(), 100);
    EXPECT_EQ(sd2.getDeductionReason(), "damage");
    EXPECT_EQ(sd2.getInterestEarned(), 50);
    EXPECT_EQ(sd2.getStatus(), "held");
}

TEST_F(SecurityDepositTest, MoveConstructor) {
    SecurityDeposit sd1("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "damage", 50, "escrow_account");
    SecurityDeposit sd2(std::move(sd1));
    
    EXPECT_EQ(sd2.getDepositId(), "D1");
    EXPECT_EQ(sd2.getAmount(), 1000);
    EXPECT_EQ(sd2.getDeductions(), 100);
    EXPECT_EQ(sd2.getDeductionReason(), "damage");
    EXPECT_EQ(sd2.getInterestEarned(), 50);
}

TEST_F(SecurityDepositTest, MoveAssignment) {
    SecurityDeposit sd1("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "damage", 50, "escrow_account");
    SecurityDeposit sd2("D2", nullptr, nullptr, 500, now, now + std::chrono::hours(24*100),
                       "released", 0, "", 0, "");
    
    sd2 = std::move(sd1);
    
    EXPECT_EQ(sd2.getDepositId(), "D1");
    EXPECT_EQ(sd2.getAmount(), 1000);
    EXPECT_EQ(sd2.getDeductions(), 100);
    EXPECT_EQ(sd2.getDeductionReason(), "damage");
    EXPECT_EQ(sd2.getInterestEarned(), 50);
}

TEST_F(SecurityDepositTest, IsNotRefundableStatusForfeited) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "forfeited", 0, "", 0, "escrow_account");
    EXPECT_FALSE(sd.isRefundable());
}

TEST_F(SecurityDepositTest, IsNotRefundableStatusAppliedToRent) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "applied_to_rent", 0, "", 0, "escrow_account");
    EXPECT_FALSE(sd.isRefundable());
}

TEST_F(SecurityDepositTest, CalculateReturnAmountZeroAmount) {
    SecurityDeposit sd("D1", tenant, contract, 0, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "escrow_account");
    EXPECT_EQ(sd.calculateReturnAmount(), 0.0);
}

TEST_F(SecurityDepositTest, CalculateReturnAmountWithDeductionsExceedingAmount) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 1200, "major damage", 100, "escrow_account");
    EXPECT_EQ(sd.calculateReturnAmount(), -100.0);
}

TEST_F(SecurityDepositTest, CalculateInterestZeroAmount) {
    SecurityDeposit sd("D1", tenant, contract, 0, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "escrow_account");
    double interest = sd.calculateInterest(0.05, now + std::chrono::hours(24*365));
    EXPECT_EQ(interest, 0.0);
}

TEST_F(SecurityDepositTest, CalculateInterestBeforeDepositDate) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now + std::chrono::hours(24), 
                       now + std::chrono::hours(24*365), "held", 0, "", 0, "escrow_account");
    double interest = sd.calculateInterest(0.05, now);
    EXPECT_TRUE(interest < 0.0);
}

TEST_F(SecurityDepositTest, CanBeReleasedStatusNotHeld) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "released", 0, "", 0, "escrow_account");
    EXPECT_FALSE(sd.canBeReleased(now));
}

TEST_F(SecurityDepositTest, CanBeReleasedWithoutContract) {
    SecurityDeposit sd("D1", nullptr, nullptr, 1000, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "escrow_account");
    EXPECT_TRUE(sd.canBeReleased(now + std::chrono::hours(24*400)));
}

TEST_F(SecurityDepositTest, HasDamageDeductionsVariousReasons) {
    SecurityDeposit sd1("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "wall damage", 0, "escrow_account");
    EXPECT_TRUE(sd1.hasDamageDeductions());
    
    SecurityDeposit sd2("D2", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "repair needed", 0, "escrow_account");
    EXPECT_TRUE(sd2.hasDamageDeductions());
    
    SecurityDeposit sd3("D3", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "cleaning fee", 0, "escrow_account");
    EXPECT_FALSE(sd3.hasDamageDeductions());
}

TEST_F(SecurityDepositTest, CalculateDeductionPercentageZeroAmount) {
    SecurityDeposit sd("D1", tenant, contract, 0, now, now + std::chrono::hours(24*365),
                       "held", 100, "damage", 0, "escrow_account");
    EXPECT_EQ(sd.calculateDeductionPercentage(), 0.0);
}

TEST_F(SecurityDepositTest, CalculateDeductionPercentageValid) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 250, "damage", 0, "escrow_account");
    EXPECT_NEAR(sd.calculateDeductionPercentage(), 25.0, 0.01);
}

TEST_F(SecurityDepositTest, RequiresLegalReviewDeductionsOver50Percent) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 600, "various issues", 0, "escrow_account");
    EXPECT_TRUE(sd.requiresLegalReview());
    
    SecurityDeposit sd2("D2", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 400, "various issues", 0, "escrow_account");
    EXPECT_FALSE(sd2.requiresLegalReview());
}

TEST_F(SecurityDepositTest, RequiresLegalReviewLegalReason) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 100, "legal dispute", 0, "escrow_account");
    EXPECT_TRUE(sd.requiresLegalReview());
}

TEST_F(SecurityDepositTest, IsHeldInEscrowEmptyAccount) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "");
    EXPECT_FALSE(sd.isHeldInEscrow());
}

TEST_F(SecurityDepositTest, IsHeldInEscrowGeneralAccount) {
    SecurityDeposit sd("D1", tenant, contract, 1000, now, now + std::chrono::hours(24*365),
                       "held", 0, "", 0, "general_account");
    EXPECT_FALSE(sd.isHeldInEscrow());
}