#include "../../include/business/AvailabilityCalendar.h"
#include <chrono>
#include <algorithm>

AvailabilityCalendar::AvailabilityCalendar(const std::string& id, std::shared_ptr<Property> prop,
                                         const std::vector<std::chrono::system_clock::time_point>& booked,
                                         const std::vector<std::chrono::system_clock::time_point>& maintenance,
                                         const std::chrono::system_clock::time_point& availableFrom,
                                         const std::chrono::system_clock::time_point& availableTo,
                                         int minStay, int maxStay, bool instantBooking,
                                         const std::string& noticePeriod)
    : calendarId(id), property(prop), bookedDates(booked), maintenanceDates(maintenance),
      availableFrom(availableFrom), availableTo(availableTo), minimumStayDays(minStay),
      maximumStayDays(maxStay), allowsInstantBooking(instantBooking),
      bookingNoticePeriod(noticePeriod) {}

AvailabilityCalendar::AvailabilityCalendar(const AvailabilityCalendar& other)
    : calendarId(other.calendarId), property(other.property), bookedDates(other.bookedDates),
      maintenanceDates(other.maintenanceDates), availableFrom(other.availableFrom),
      availableTo(other.availableTo), minimumStayDays(other.minimumStayDays),
      maximumStayDays(other.maximumStayDays), allowsInstantBooking(other.allowsInstantBooking),
      bookingNoticePeriod(other.bookingNoticePeriod) {}

AvailabilityCalendar& AvailabilityCalendar::operator=(const AvailabilityCalendar& other) {
    if (this != &other) {
        calendarId = other.calendarId;
        property = other.property;
        bookedDates = other.bookedDates;
        maintenanceDates = other.maintenanceDates;
        availableFrom = other.availableFrom;
        availableTo = other.availableTo;
        minimumStayDays = other.minimumStayDays;
        maximumStayDays = other.maximumStayDays;
        allowsInstantBooking = other.allowsInstantBooking;
        bookingNoticePeriod = other.bookingNoticePeriod;
    }
    return *this;
}

AvailabilityCalendar::AvailabilityCalendar(AvailabilityCalendar&& other) noexcept
    : calendarId(std::move(other.calendarId)), property(std::move(other.property)),
      bookedDates(std::move(other.bookedDates)), maintenanceDates(std::move(other.maintenanceDates)),
      availableFrom(other.availableFrom), availableTo(other.availableTo),
      minimumStayDays(other.minimumStayDays), maximumStayDays(other.maximumStayDays),
      allowsInstantBooking(other.allowsInstantBooking), bookingNoticePeriod(std::move(other.bookingNoticePeriod)) {}

AvailabilityCalendar& AvailabilityCalendar::operator=(AvailabilityCalendar&& other) noexcept {
    if (this != &other) {
        calendarId = std::move(other.calendarId);
        property = std::move(other.property);
        bookedDates = std::move(other.bookedDates);
        maintenanceDates = std::move(other.maintenanceDates);
        availableFrom = other.availableFrom;
        availableTo = other.availableTo;
        minimumStayDays = other.minimumStayDays;
        maximumStayDays = other.maximumStayDays;
        allowsInstantBooking = other.allowsInstantBooking;
        bookingNoticePeriod = std::move(other.bookingNoticePeriod);
    }
    return *this;
}

AvailabilityCalendar::~AvailabilityCalendar() {}

std::string AvailabilityCalendar::getCalendarId() const { return calendarId; }
std::shared_ptr<Property> AvailabilityCalendar::getProperty() const { return property; }
std::vector<std::chrono::system_clock::time_point> AvailabilityCalendar::getBookedDates() const { return bookedDates; }
std::vector<std::chrono::system_clock::time_point> AvailabilityCalendar::getMaintenanceDates() const { return maintenanceDates; }
std::chrono::system_clock::time_point AvailabilityCalendar::getAvailableFrom() const { return availableFrom; }
std::chrono::system_clock::time_point AvailabilityCalendar::getAvailableTo() const { return availableTo; }
int AvailabilityCalendar::getMinimumStayDays() const { return minimumStayDays; }
int AvailabilityCalendar::getMaximumStayDays() const { return maximumStayDays; }
bool AvailabilityCalendar::getAllowsInstantBooking() const { return allowsInstantBooking; }
std::string AvailabilityCalendar::getBookingNoticePeriod() const { return bookingNoticePeriod; }

void AvailabilityCalendar::setAvailableFrom(const std::chrono::system_clock::time_point& newFrom) {
    availableFrom = newFrom;
}

void AvailabilityCalendar::setAvailableTo(const std::chrono::system_clock::time_point& newTo) {
    availableTo = newTo;
}

void AvailabilityCalendar::setMinimumStayDays(int minDays) {
    minimumStayDays = std::max(1, minDays);
}

void AvailabilityCalendar::setMaximumStayDays(int maxDays) {
    maximumStayDays = std::max(minimumStayDays, maxDays);
}

bool AvailabilityCalendar::isAvailable(const std::chrono::system_clock::time_point& checkDate) const {
    if (checkDate < availableFrom || checkDate > availableTo) return false;
    
    for (const auto& bookedDate : bookedDates) {
        if (std::chrono::duration_cast<std::chrono::hours>(checkDate - bookedDate).count() < 24) {
            return false;
        }
    }
    
    for (const auto& maintenanceDate : maintenanceDates) {
        if (std::chrono::duration_cast<std::chrono::hours>(checkDate - maintenanceDate).count() < 24) {
            return false;
        }
    }
    
    return true;
}

bool AvailabilityCalendar::isAvailableForPeriod(const std::chrono::system_clock::time_point& startDate,
                                               const std::chrono::system_clock::time_point& endDate) const {
    auto currentDate = startDate;
    while (currentDate <= endDate) {
        if (!isAvailable(currentDate)) return false;
        currentDate += std::chrono::hours(24);
    }
    return true;
}

int AvailabilityCalendar::getAvailableDaysCount(const std::chrono::system_clock::time_point& startDate,
                                               const std::chrono::system_clock::time_point& endDate) const {
    int count = 0;
    auto currentDate = startDate;
    
    while (currentDate <= endDate) {
        if (isAvailable(currentDate)) count++;
        currentDate += std::chrono::hours(24);
    }
    
    return count;
}

bool AvailabilityCalendar::canAccommodateStay(int stayDays) const {
    return stayDays >= minimumStayDays && stayDays <= maximumStayDays;
}

bool AvailabilityCalendar::requiresAdvanceNotice(const std::chrono::system_clock::time_point& requestedDate) const {
    if (allowsInstantBooking) return false;
    
    auto now = std::chrono::system_clock::now();
    auto hoursUntilRequest = std::chrono::duration_cast<std::chrono::hours>(requestedDate - now).count();
    
    if (bookingNoticePeriod == "24_hours") return hoursUntilRequest < 24;
    if (bookingNoticePeriod == "48_hours") return hoursUntilRequest < 48;
    if (bookingNoticePeriod == "1_week") return hoursUntilRequest < 168;
    
    return hoursUntilRequest < 72;
}

double AvailabilityCalendar::calculateOccupancyRate(const std::chrono::system_clock::time_point& startDate,
                                                   const std::chrono::system_clock::time_point& endDate) const {
    int totalDays = getAvailableDaysCount(startDate, endDate);
    int bookedDays = bookedDates.size();
    
    if (totalDays == 0) return 0.0;
    return (static_cast<double>(bookedDays) / totalDays) * 100.0;
}

std::vector<std::chrono::system_clock::time_point> AvailabilityCalendar::getAvailableDatesInRange(
    const std::chrono::system_clock::time_point& startDate,
    const std::chrono::system_clock::time_point& endDate) const {
    
    std::vector<std::chrono::system_clock::time_point> availableDates;
    auto currentDate = startDate;
    
    while (currentDate <= endDate) {
        if (isAvailable(currentDate)) {
            availableDates.push_back(currentDate);
        }
        currentDate += std::chrono::hours(24);
    }
    
    return availableDates;
}

bool AvailabilityCalendar::hasMaintenanceScheduled(const std::chrono::system_clock::time_point& date) const {
    for (const auto& maintenanceDate : maintenanceDates) {
        if (std::chrono::duration_cast<std::chrono::hours>(date - maintenanceDate).count() < 24) {
            return true;
        }
    }
    return false;
}

int AvailabilityCalendar::getDaysUntilNextAvailable(const std::chrono::system_clock::time_point& currentDate) const {
    auto checkDate = currentDate;
    int days = 0;
    
    while (checkDate <= availableTo) {
        if (isAvailable(checkDate)) return days;
        checkDate += std::chrono::hours(24);
        days++;
    }
    
    return -1;
}