#ifndef AVAILABILITYCALENDAR_H
#define AVAILABILITYCALENDAR_H

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "../../include/core/Property.h"

class AvailabilityCalendar {
private:
    std::string calendarId;
    std::shared_ptr<Property> property;
    std::vector<std::chrono::system_clock::time_point> bookedDates;
    std::vector<std::chrono::system_clock::time_point> maintenanceDates;
    std::chrono::system_clock::time_point availableFrom;
    std::chrono::system_clock::time_point availableTo;
    int minimumStayDays;
    int maximumStayDays;
    bool allowsInstantBooking;
    std::string bookingNoticePeriod;

public:
    AvailabilityCalendar(const std::string& id, std::shared_ptr<Property> prop,
                        const std::vector<std::chrono::system_clock::time_point>& booked,
                        const std::vector<std::chrono::system_clock::time_point>& maintenance,
                        const std::chrono::system_clock::time_point& availableFrom,
                        const std::chrono::system_clock::time_point& availableTo,
                        int minStay, int maxStay, bool instantBooking,
                        const std::string& noticePeriod);
    
    AvailabilityCalendar(const AvailabilityCalendar& other);
    AvailabilityCalendar& operator=(const AvailabilityCalendar& other);
    AvailabilityCalendar(AvailabilityCalendar&& other) noexcept;
    AvailabilityCalendar& operator=(AvailabilityCalendar&& other) noexcept;
    ~AvailabilityCalendar();

    std::string getCalendarId() const;
    std::shared_ptr<Property> getProperty() const;
    std::vector<std::chrono::system_clock::time_point> getBookedDates() const;
    std::vector<std::chrono::system_clock::time_point> getMaintenanceDates() const;
    std::chrono::system_clock::time_point getAvailableFrom() const;
    std::chrono::system_clock::time_point getAvailableTo() const;
    int getMinimumStayDays() const;
    int getMaximumStayDays() const;
    bool getAllowsInstantBooking() const;
    std::string getBookingNoticePeriod() const;

    void setAvailableFrom(const std::chrono::system_clock::time_point& newFrom);
    void setAvailableTo(const std::chrono::system_clock::time_point& newTo);
    void setMinimumStayDays(int minDays);
    void setMaximumStayDays(int maxDays);

    bool isAvailable(const std::chrono::system_clock::time_point& checkDate) const;
    bool isAvailableForPeriod(const std::chrono::system_clock::time_point& startDate,
                             const std::chrono::system_clock::time_point& endDate) const;
    int getAvailableDaysCount(const std::chrono::system_clock::time_point& startDate,
                             const std::chrono::system_clock::time_point& endDate) const;
    bool canAccommodateStay(int stayDays) const;
    bool requiresAdvanceNotice(const std::chrono::system_clock::time_point& requestedDate) const;
    double calculateOccupancyRate(const std::chrono::system_clock::time_point& startDate,
                                 const std::chrono::system_clock::time_point& endDate) const;
    std::vector<std::chrono::system_clock::time_point> getAvailableDatesInRange(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate) const;
    bool hasMaintenanceScheduled(const std::chrono::system_clock::time_point& date) const;
    int getDaysUntilNextAvailable(const std::chrono::system_clock::time_point& currentDate) const;
};

#endif