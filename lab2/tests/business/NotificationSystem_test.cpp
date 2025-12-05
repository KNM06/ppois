#include <gtest/gtest.h>
#include "../../include/business/NotificationSystem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/ReservationSystem.h"

class NotificationSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        auto reservationSystem = std::make_shared<ReservationSystem>();
        notificationSystem = std::make_shared<NotificationSystem>(maintenanceScheduler, reservationSystem);
    }
    
    std::shared_ptr<NotificationSystem> notificationSystem;
};

TEST_F(NotificationSystemTest, SendMaintenanceReminder) {
    EXPECT_NO_THROW(notificationSystem->sendMaintenanceReminder("ITEM001"));
}

TEST_F(NotificationSystemTest, SendReservationConfirmation) {
    EXPECT_NO_THROW(notificationSystem->sendReservationConfirmation("CUST001", "RES001"));
}

TEST_F(NotificationSystemTest, SendRentalReminder) {
    EXPECT_NO_THROW(notificationSystem->sendRentalReminder("CUST001", "ITEM001"));
}

TEST_F(NotificationSystemTest, SendPromotionalOffer) {
    EXPECT_NO_THROW(notificationSystem->sendPromotionalOffer("CUST001", "LOYALTY"));
}

TEST_F(NotificationSystemTest, GetPendingNotifications) {
    auto notifications = notificationSystem->getPendingNotifications();
    EXPECT_FALSE(notifications.empty());
}