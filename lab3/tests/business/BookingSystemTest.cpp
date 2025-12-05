#include <gtest/gtest.h>
#include "../../include/business/BookingSystem.h"
#include "../../include/core/Property.h"
#include "../../include/core/PropertyType.h"
#include "../../include/core/Address.h"
#include "../../include/core/Tenant.h"
#include "../../include/business/AvailabilityCalendar.h"
#include <chrono>

class BookingSystemTest : public ::testing::Test {
protected:
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::shared_ptr<AvailabilityCalendar> calendar;
    BookingSystem bookingSystem;
    std::chrono::system_clock::time_point today;
    std::chrono::system_clock::time_point tomorrow;

    BookingSystemTest() : bookingSystem(10.0, 20.0, 5, "flexible", 50.0) {}

    void SetUp() override {
        today = std::chrono::system_clock::now();
        tomorrow = today + std::chrono::hours(24);

        property = std::make_shared<Property>(
            "P001", 100.0, 3000.0,
            std::make_unique<Address>("Street 1", "City", "State", "12345"),
            nullptr,
            std::make_unique<PropertyType>("R001", "residential", "Apartment", 1, 3, 50, 200, false, "residential", 1.0)
        );

        tenant = std::make_shared<Tenant>("T001", "John Doe");

        calendar = std::make_shared<AvailabilityCalendar>(
            "C001", property, std::vector<std::chrono::system_clock::time_point>{}, 
            std::vector<std::chrono::system_clock::time_point>{},
            today, today + std::chrono::hours(24*10),
            1, 10, true, "24_hours"
        );

        bookingSystem.calendars.push_back(calendar);
        bookingSystem.propertyCalendars[property->getId()] = calendar;

        bookingSystem.createBooking(property, tenant, today, tomorrow, property->getRentalPrice());
    }
};

TEST_F(BookingSystemTest, GettersAndSetters) {
    EXPECT_EQ(bookingSystem.getBookingFeePercentage(), 10.0);
    EXPECT_EQ(bookingSystem.getSecurityDepositPercentage(), 20.0);
    EXPECT_EQ(bookingSystem.getCancellationPolicy(), "flexible");

    bookingSystem.setBookingFeePercentage(15.0);
    bookingSystem.setSecurityDepositPercentage(25.0);
    bookingSystem.setCancellationPolicy("strict");

    EXPECT_EQ(bookingSystem.getBookingFeePercentage(), 15.0);
    EXPECT_EQ(bookingSystem.getSecurityDepositPercentage(), 25.0);
    EXPECT_EQ(bookingSystem.getCancellationPolicy(), "strict");
}

TEST_F(BookingSystemTest, BookingFees) {
    double rentalPrice = 1000.0;
    EXPECT_EQ(bookingSystem.calculateBookingFee(rentalPrice), 100.0);
    EXPECT_EQ(bookingSystem.calculateSecurityDeposit(rentalPrice), 200.0);
}

TEST_F(BookingSystemTest, CancellationFeeFlexible) {
    auto bookingDate = today + std::chrono::hours(48);
    auto cancelDate = today + std::chrono::hours(30);
    double totalAmount = 1000.0;

    EXPECT_EQ(bookingSystem.calculateCancellationFee(bookingDate, cancelDate, totalAmount), 100.0);
    
    auto cancelDateEarly = today + std::chrono::hours(20); 
    EXPECT_EQ(bookingSystem.calculateCancellationFee(bookingDate, cancelDateEarly, totalAmount), 0.0);
}

TEST_F(BookingSystemTest, SearchAvailableProperties) {
    std::vector<std::shared_ptr<Property>> available = 
        bookingSystem.searchAvailableProperties(today, tomorrow, "City");
    EXPECT_FALSE(available.empty());
    EXPECT_EQ(available[0]->getId(), "P001");
}

TEST_F(BookingSystemTest, TotalBookingCost) {
    double totalCost = bookingSystem.calculateTotalBookingCost(1000.0, 2);
    double expected = 1000.0*2 + 1000.0*2*0.1 + 1000.0*2*0.2;
    EXPECT_DOUBLE_EQ(totalCost, expected);
}

TEST_F(BookingSystemTest, PropertyAvailability) {
    EXPECT_TRUE(bookingSystem.isPropertyAvailableForBooking("P001", today, tomorrow));
}

TEST_F(BookingSystemTest, BookingsCount) {
    EXPECT_EQ(bookingSystem.getPropertyBookingsCount("P001"), 0);
}
