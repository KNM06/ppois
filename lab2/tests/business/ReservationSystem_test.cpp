#include <gtest/gtest.h>
#include "../../include/business/ReservationSystem.h"
#include <memory>

class ReservationSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        reservationSystem = std::make_unique<ReservationSystem>();
    }
    
    std::unique_ptr<ReservationSystem> reservationSystem;
};

TEST_F(ReservationSystemTest, MakeReservationSuccess) {
    bool result = reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    EXPECT_TRUE(result);
}

TEST_F(ReservationSystemTest, MakeReservationWithConflictingReservation) {
    reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    reservationSystem->makeReservation("CUST002", "ITEM001", "2024-01-08", "2024-01-14");
    reservationSystem->makeReservation("CUST003", "ITEM001", "2024-01-15", "2024-01-21");
    
    bool result = reservationSystem->makeReservation("CUST004", "ITEM001", "2024-01-22", "2024-01-28");
    EXPECT_FALSE(result);
}

TEST_F(ReservationSystemTest, CalculateReservationFeeForVehicle) {
    double fee = reservationSystem->calculateReservationFee("VEH001", 7);
    EXPECT_GT(fee, 0.0);
}

TEST_F(ReservationSystemTest, CalculateReservationFeeForElectronics) {
    double fee = reservationSystem->calculateReservationFee("ELEC001", 3);
    EXPECT_GT(fee, 0.0);
}

TEST_F(ReservationSystemTest, CalculateReservationFeeForConstruction) {
    double fee = reservationSystem->calculateReservationFee("CONST001", 14);
    EXPECT_GT(fee, 0.0);
}

TEST_F(ReservationSystemTest, HasConflictingReservationsInitiallyFalse) {
    bool hasConflicts = reservationSystem->hasConflictingReservations("ITEM001");
    EXPECT_FALSE(hasConflicts);
}

TEST_F(ReservationSystemTest, GetSeasonalDemandFactor) {
    double factor = reservationSystem->getSeasonalDemandFactor();
    EXPECT_GT(factor, 0.0);
}

TEST_F(ReservationSystemTest, GetDemandForecast) {
    auto forecast = reservationSystem->getDemandForecast(30);
    EXPECT_FALSE(forecast.empty());
    EXPECT_GT(forecast["VEHICLE"], 0.0);
    EXPECT_GT(forecast["ELECTRONICS"], 0.0);
}

TEST_F(ReservationSystemTest, ProcessWaitingListNoCrash) {
    EXPECT_NO_THROW(reservationSystem->processWaitingList("ITEM001"));
}

TEST_F(ReservationSystemTest, CancelReservationSuccess) {
    bool made = reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    ASSERT_TRUE(made);
    
    bool cancelled = reservationSystem->cancelReservation("RES1000");
    EXPECT_TRUE(cancelled);
}

TEST_F(ReservationSystemTest, CancelReservationInvalidId) {
    bool cancelled = reservationSystem->cancelReservation("INVALID");
    EXPECT_FALSE(cancelled);
}

TEST_F(ReservationSystemTest, CalculateCancellationPenalty) {
    bool made = reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    ASSERT_TRUE(made);
    
    double penalty = reservationSystem->calculateCancellationPenalty("RES1000");
    EXPECT_GE(penalty, 0.0);
}

TEST_F(ReservationSystemTest, CalculateCancellationPenaltyInvalidId) {
    double penalty = reservationSystem->calculateCancellationPenalty("INVALID");
    EXPECT_EQ(penalty, 0.0);
}

TEST_F(ReservationSystemTest, MultipleReservationsDifferentItems) {
    bool result1 = reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    bool result2 = reservationSystem->makeReservation("CUST002", "ITEM002", "2024-01-01", "2024-01-07");
    
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
}

TEST_F(ReservationSystemTest, ReservationFeeIncreasesWithDuration) {
    double fee1 = reservationSystem->calculateReservationFee("ITEM001", 1);
    double fee7 = reservationSystem->calculateReservationFee("ITEM001", 7);
    double fee30 = reservationSystem->calculateReservationFee("ITEM001", 30);
    
    EXPECT_GT(fee7, fee1);
    EXPECT_GT(fee30, fee7);
}

TEST_F(ReservationSystemTest, DemandForecastChangesWithDaysForward) {
    auto forecast7 = reservationSystem->getDemandForecast(7);
    auto forecast30 = reservationSystem->getDemandForecast(30);
    auto forecast90 = reservationSystem->getDemandForecast(90);
    
    EXPECT_GT(forecast30["VEHICLE"], forecast7["VEHICLE"]);
    EXPECT_GT(forecast90["VEHICLE"], forecast30["VEHICLE"]);
}

TEST_F(ReservationSystemTest, SeasonalFactorConsistent) {
    double factor1 = reservationSystem->getSeasonalDemandFactor();
    double factor2 = reservationSystem->getSeasonalDemandFactor();
    
    EXPECT_DOUBLE_EQ(factor1, factor2);
}

TEST_F(ReservationSystemTest, MakeReservationSameCustomerDifferentItems) {
    bool result1 = reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    bool result2 = reservationSystem->makeReservation("CUST001", "ITEM002", "2024-01-08", "2024-01-14");
    
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
}

TEST_F(ReservationSystemTest, ReservationIdFormat) {
    bool made = reservationSystem->makeReservation("CUST001", "ITEM001", "2024-01-01", "2024-01-07");
    ASSERT_TRUE(made);
    
    EXPECT_NO_THROW(reservationSystem->cancelReservation("RES1000"));
}