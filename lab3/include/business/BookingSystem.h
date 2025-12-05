#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/business/AvailabilityCalendar.h"

class BookingSystem {
private:
    std::vector<std::shared_ptr<AvailabilityCalendar>> calendars;
    std::unordered_map<std::string, std::shared_ptr<AvailabilityCalendar>> propertyCalendars;
    double bookingFeePercentage;
    double securityDepositPercentage;
    int maxConcurrentBookings;
    std::string cancellationPolicy;
    double lateCancellationFee;

public:
    BookingSystem(double bookingFee, double securityDeposit, int maxConcurrent,
                  const std::string& cancellationPolicy, double lateCancellationFee);
    
    BookingSystem(const BookingSystem& other);
    BookingSystem& operator=(const BookingSystem& other);
    BookingSystem(BookingSystem&& other) noexcept;
    BookingSystem& operator=(BookingSystem&& other) noexcept;
    ~BookingSystem();

    double getBookingFeePercentage() const;
    double getSecurityDepositPercentage() const;
    int getMaxConcurrentBookings() const;
    std::string getCancellationPolicy() const;
    double getLateCancellationFee() const;

    void setBookingFeePercentage(double newFee);
    void setSecurityDepositPercentage(double newDeposit);
    void setCancellationPolicy(const std::string& newPolicy);

    bool createBooking(std::shared_ptr<Property> property, std::shared_ptr<Tenant> tenant,
                      const std::chrono::system_clock::time_point& startDate,
                      const std::chrono::system_clock::time_point& endDate,
                      double rentalPrice);
    bool cancelBooking(const std::string& propertyId, const std::string& tenantId,
                      const std::chrono::system_clock::time_point& cancellationDate);
    double calculateBookingFee(double rentalPrice) const;
    double calculateSecurityDeposit(double rentalPrice) const;
    double calculateCancellationFee(const std::chrono::system_clock::time_point& bookingDate,
                                   const std::chrono::system_clock::time_point& cancellationDate,
                                   double totalAmount) const;
    std::vector<std::shared_ptr<Property>> searchAvailableProperties(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate,
        const std::string& location) const;
    bool modifyBooking(const std::string& propertyId, const std::string& tenantId,
                      const std::chrono::system_clock::time_point& newStartDate,
                      const std::chrono::system_clock::time_point& newEndDate);
    double calculateTotalBookingCost(double rentalPrice, int stayDays) const;
    bool isPropertyAvailableForBooking(const std::string& propertyId,
                                      const std::chrono::system_clock::time_point& startDate,
                                      const std::chrono::system_clock::time_point& endDate) const;
    int getPropertyBookingsCount(const std::string& propertyId) const;
    friend class BookingSystemTest;
};

#endif