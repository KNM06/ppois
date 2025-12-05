#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include "../../include/business/NotificationSystem.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/Address.h"
#include "../../include/finance/RentalContract.h"

class NotificationSystemTest : public ::testing::Test {
protected:
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<Owner> owner;
    std::shared_ptr<RentalContract> contract;
    std::chrono::system_clock::time_point now;

    void SetUp() override {
        tenant = std::make_shared<Tenant>("ten1", "John", "Doe", "john@example.com", "1234567890", nullptr, 700, 2000, false, 1);

        auto ownerAddress = std::make_unique<Address>("123 Oak St", "City", "State", "12345");
        owner = std::make_shared<Owner>("own1", "Alice", "Smith", "alice@example.com", "0987654321",
                                        std::move(ownerAddress), "123456789", 100.0, true, "bank_transfer");

        auto propertyAddress = std::make_unique<Address>("456 Elm St", "City", "State", "67890");
        auto propertyType = std::make_unique<PropertyType>("PT1", "residential", "House type", 1, 5, 50.0, 200.0, false, "residential", 1.5);
        auto propertyStatus = std::make_unique<PropertyStatus>("AVAILABLE", "Property is available",
                                                               true, false, false, std::chrono::system_clock::now(), "", 0.0, 0);

        auto property = std::make_shared<Property>("prop1", 100.0, 2000.0,
                                                   std::move(propertyAddress),
                                                   std::move(propertyStatus),
                                                   std::move(propertyType));

        auto start = std::chrono::system_clock::now();
        auto end = start + std::chrono::hours(24*365);
        contract = std::make_shared<RentalContract>("con1", property, tenant, owner, start, end, 1000.0, 500.0, "monthly");

        now = std::chrono::system_clock::now();
    }
};


TEST_F(NotificationSystemTest, ConstructorAndGetters) {
    NotificationSystem ns(true, false, true, 5);
    EXPECT_TRUE(ns.getEmailEnabled());
    EXPECT_FALSE(ns.getSmsEnabled());
    EXPECT_TRUE(ns.getPushEnabled());
    EXPECT_EQ(ns.getMaxNotificationsPerDay(), 5);
}

TEST_F(NotificationSystemTest, Setters) {
    NotificationSystem ns(true, false, true, 5);
    ns.setEmailEnabled(false);
    ns.setSmsEnabled(true);
    ns.setPushEnabled(false);
    ns.setMaxNotificationsPerDay(10);
    EXPECT_FALSE(ns.getEmailEnabled());
    EXPECT_TRUE(ns.getSmsEnabled());
    EXPECT_FALSE(ns.getPushEnabled());
    EXPECT_EQ(ns.getMaxNotificationsPerDay(), 10);
}

TEST_F(NotificationSystemTest, SendRentReminder) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendRentReminder(tenant, contract, now));
    auto notifs = ns.getNotificationsForUser("ten1");
    EXPECT_FALSE(notifs.empty());
    EXPECT_TRUE(ns.hasUserReceivedNotification("ten1", "Rent due reminder"));
}

TEST_F(NotificationSystemTest, SendMaintenanceUpdate) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendMaintenanceUpdate(tenant, "In Progress", "Leak fixed"));
    auto notifs = ns.getNotificationsForUser("ten1");
    EXPECT_FALSE(notifs.empty());
    EXPECT_TRUE(ns.hasUserReceivedNotification("ten1", "Maintenance update"));
}

TEST_F(NotificationSystemTest, SendContractExpirationAlert) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendContractExpirationAlert(owner, contract, 5));
    auto notifs = ns.getNotificationsForUser("own1");
    EXPECT_FALSE(notifs.empty());
    EXPECT_TRUE(ns.hasUserReceivedNotification("own1", "expires in"));
}

TEST_F(NotificationSystemTest, SendPaymentConfirmation) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendPaymentConfirmation(tenant, 500.0, now));
    EXPECT_TRUE(ns.hasUserReceivedNotification("ten1", "Payment confirmed"));
}

TEST_F(NotificationSystemTest, SendSecurityDepositRefund) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendSecurityDepositRefund(tenant, 200.0));
    EXPECT_TRUE(ns.hasUserReceivedNotification("ten1", "Security deposit refund"));
}

TEST_F(NotificationSystemTest, SendEmergencyAlert) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendEmergencyAlert(tenant, "Fire", "Evacuate immediately"));
    EXPECT_TRUE(ns.hasUserReceivedNotification("ten1", "EMERGENCY"));
}

TEST_F(NotificationSystemTest, SendInspectionNotice) {
    NotificationSystem ns(true, true, true, 10);
    EXPECT_TRUE(ns.sendInspectionNotice(tenant, now));
    EXPECT_TRUE(ns.hasUserReceivedNotification("ten1", "Property inspection"));
}

TEST_F(NotificationSystemTest, UserNotificationCountAndDailyLimit) {
    NotificationSystem ns(true, true, true, 2);
    EXPECT_TRUE(ns.sendRentReminder(tenant, contract, now));
    EXPECT_FALSE(ns.sendMaintenanceUpdate(tenant, "Done", "All fixed"));
    EXPECT_EQ(ns.getUserNotificationCount("ten1"), 1);
    EXPECT_FALSE(ns.canSendNotification("ten1"));
}


TEST_F(NotificationSystemTest, ClearOldNotifications) {
    NotificationSystem ns(true, true, true, 10);
    ns.sendRentReminder(tenant, contract, now);
    ns.clearOldNotifications(0);
    EXPECT_FALSE(ns.getNotificationsForUser("ten1").empty());
}
