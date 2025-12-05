#include <gtest/gtest.h>
#include "../../include/business/AvailabilityCalendar.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/Address.h"
#include <chrono>

class AvailabilityCalendarTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    std::chrono::system_clock::time_point today;
    std::chrono::system_clock::time_point tomorrow;
    std::chrono::system_clock::time_point nextWeek;

    void SetUp() override {
        property = std::make_shared<Property>(
            "P001", 100.0, 3000.0,
            std::make_unique<Address>("Street 1", "City", "State", "12345"),
            nullptr,
            std::make_unique<PropertyType>("R001", "residential", "Apartment", 1, 3, 50, 200, false, "residential", 1.0)
        );

        today = std::chrono::system_clock::now();
        tomorrow = today + std::chrono::hours(24);
        nextWeek = today + std::chrono::hours(24*7);
    }
};

TEST_F(AvailabilityCalendarTest, ConstructorAndGetters) {
    AvailabilityCalendar cal("C001", property, {}, {}, today, nextWeek, 2, 10, true, "24_hours");

    EXPECT_EQ(cal.getCalendarId(), "C001");
    EXPECT_EQ(cal.getProperty(), property);
    EXPECT_EQ(cal.getBookedDates().size(), 0);
    EXPECT_EQ(cal.getMaintenanceDates().size(), 0);
    EXPECT_EQ(cal.getAvailableFrom(), today);
    EXPECT_EQ(cal.getAvailableTo(), nextWeek);
    EXPECT_EQ(cal.getMinimumStayDays(), 2);
    EXPECT_EQ(cal.getMaximumStayDays(), 10);
    EXPECT_TRUE(cal.getAllowsInstantBooking());
    EXPECT_EQ(cal.getBookingNoticePeriod(), "24_hours");
}

TEST_F(AvailabilityCalendarTest, Setters) {
    AvailabilityCalendar cal("C001", property, {}, {}, today, nextWeek, 2, 10, true, "24_hours");

    auto newFrom = today + std::chrono::hours(48);
    auto newTo = nextWeek + std::chrono::hours(24);
    cal.setAvailableFrom(newFrom);
    cal.setAvailableTo(newTo);
    cal.setMinimumStayDays(3);
    cal.setMaximumStayDays(5);

    EXPECT_EQ(cal.getAvailableFrom(), newFrom);
    EXPECT_EQ(cal.getAvailableTo(), newTo);
    EXPECT_EQ(cal.getMinimumStayDays(), 3);
    EXPECT_EQ(cal.getMaximumStayDays(), 5);
}

TEST_F(AvailabilityCalendarTest, AvailabilityChecks) {
    AvailabilityCalendar cal("C001", property, {tomorrow}, {nextWeek}, today, nextWeek, 1, 10, false, "24_hours");

    EXPECT_FALSE(cal.isAvailable(today));
    EXPECT_FALSE(cal.isAvailable(tomorrow)); 
    EXPECT_FALSE(cal.isAvailable(nextWeek)); 

    EXPECT_FALSE(cal.isAvailableForPeriod(today, nextWeek)); 
}

TEST_F(AvailabilityCalendarTest, AvailableDaysCount) {
    AvailabilityCalendar cal("C001", property, {tomorrow}, {nextWeek}, today, nextWeek, 1, 10, false, "24_hours");
    int count = cal.getAvailableDaysCount(today, nextWeek);
    EXPECT_EQ(count, 0); 
}

TEST_F(AvailabilityCalendarTest, CanAccommodateStay) {
    AvailabilityCalendar cal("C001", property, {}, {}, today, nextWeek, 2, 5, true, "24_hours");
    EXPECT_TRUE(cal.canAccommodateStay(3));
    EXPECT_FALSE(cal.canAccommodateStay(1));
    EXPECT_FALSE(cal.canAccommodateStay(6));
}

TEST_F(AvailabilityCalendarTest, RequiresAdvanceNotice) {
    AvailabilityCalendar cal("C001", property, {}, {}, today, nextWeek, 1, 10, false, "24_hours");
    auto requested = today + std::chrono::hours(12);
    EXPECT_TRUE(cal.requiresAdvanceNotice(requested));

    requested = today + std::chrono::hours(30);
    EXPECT_FALSE(cal.requiresAdvanceNotice(requested));

    AvailabilityCalendar instantCal("C002", property, {}, {}, today, nextWeek, 1, 10, true, "24_hours");
    EXPECT_FALSE(instantCal.requiresAdvanceNotice(today));
}

TEST_F(AvailabilityCalendarTest, OccupancyAndAvailableDates) {
    AvailabilityCalendar cal("C001", property, {tomorrow}, {}, today, nextWeek, 1, 10, true, "24_hours");

    double rate = cal.calculateOccupancyRate(today, nextWeek);
   
    EXPECT_NEAR(rate, 16.666666666666664, 1e-6);

    auto dates = cal.getAvailableDatesInRange(today, nextWeek);
    EXPECT_EQ(dates.size(), 6); 
}

TEST_F(AvailabilityCalendarTest, MaintenanceAndNextAvailable) {
    AvailabilityCalendar cal("C001", property, {}, {tomorrow}, today, nextWeek, 1, 10, true, "24_hours");
    EXPECT_TRUE(cal.hasMaintenanceScheduled(tomorrow));
    EXPECT_TRUE(cal.hasMaintenanceScheduled(today));

    int daysUntil = cal.getDaysUntilNextAvailable(today);
    EXPECT_EQ(daysUntil, 2);
}