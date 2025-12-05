#include <gtest/gtest.h>
#include "../../include/core/RentalHistory.h"

class RentalHistoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        rentalHistory = std::make_shared<RentalHistory>();
    }
    
    std::shared_ptr<RentalHistory> rentalHistory;
};

TEST_F(RentalHistoryTest, AddRental) {
    rentalHistory->addRental(150.0);
    EXPECT_GT(rentalHistory->getTotalAmountSpent(), 0.0);
}

TEST_F(RentalHistoryTest, GetOnTimePaymentRatio) {
    double ratio = rentalHistory->getOnTimePaymentRatio();
    EXPECT_GE(ratio, 0.0);
    EXPECT_LE(ratio, 1.0);
}

TEST_F(RentalHistoryTest, GetLateReturnRatio) {
    double ratio = rentalHistory->getLateReturnRatio();
    EXPECT_GE(ratio, 0.0);
    EXPECT_LE(ratio, 1.0);
}

TEST_F(RentalHistoryTest, GetRentalCountForCategory) {
    int count = rentalHistory->getRentalCountForCategory("TOOLS");
    EXPECT_GE(count, 0);
}

TEST_F(RentalHistoryTest, GetAverageRentalAmount) {
    rentalHistory->addRental(150.0);
    rentalHistory->addRental(200.0);
    double average = rentalHistory->getAverageRentalAmount();
    EXPECT_GT(average, 0.0);
}