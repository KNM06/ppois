#include "../../include/business/BookingSystem.h"
#include <chrono>
#include <algorithm>

BookingSystem::BookingSystem(double bookingFee, double securityDeposit, int maxConcurrent,
                             const std::string& cancellationPolicy, double lateCancellationFee)
    : bookingFeePercentage(bookingFee), securityDepositPercentage(securityDeposit),
      maxConcurrentBookings(maxConcurrent), cancellationPolicy(cancellationPolicy),
      lateCancellationFee(lateCancellationFee) {}

BookingSystem::BookingSystem(const BookingSystem& other)
    : calendars(other.calendars), propertyCalendars(other.propertyCalendars),
      bookingFeePercentage(other.bookingFeePercentage),
      securityDepositPercentage(other.securityDepositPercentage),
      maxConcurrentBookings(other.maxConcurrentBookings),
      cancellationPolicy(other.cancellationPolicy),
      lateCancellationFee(other.lateCancellationFee) {}

BookingSystem& BookingSystem::operator=(const BookingSystem& other) {
    if (this != &other) {
        calendars = other.calendars;
        propertyCalendars = other.propertyCalendars;
        bookingFeePercentage = other.bookingFeePercentage;
        securityDepositPercentage = other.securityDepositPercentage;
        maxConcurrentBookings = other.maxConcurrentBookings;
        cancellationPolicy = other.cancellationPolicy;
        lateCancellationFee = other.lateCancellationFee;
    }
    return *this;
}

BookingSystem::BookingSystem(BookingSystem&& other) noexcept
    : calendars(std::move(other.calendars)), propertyCalendars(std::move(other.propertyCalendars)),
      bookingFeePercentage(other.bookingFeePercentage),
      securityDepositPercentage(other.securityDepositPercentage),
      maxConcurrentBookings(other.maxConcurrentBookings),
      cancellationPolicy(std::move(other.cancellationPolicy)),
      lateCancellationFee(other.lateCancellationFee) {}

BookingSystem& BookingSystem::operator=(BookingSystem&& other) noexcept {
    if (this != &other) {
        calendars = std::move(other.calendars);
        propertyCalendars = std::move(other.propertyCalendars);
        bookingFeePercentage = other.bookingFeePercentage;
        securityDepositPercentage = other.securityDepositPercentage;
        maxConcurrentBookings = other.maxConcurrentBookings;
        cancellationPolicy = std::move(other.cancellationPolicy);
        lateCancellationFee = other.lateCancellationFee;
    }
    return *this;
}

BookingSystem::~BookingSystem() {}

double BookingSystem::getBookingFeePercentage() const { return bookingFeePercentage; }
double BookingSystem::getSecurityDepositPercentage() const { return securityDepositPercentage; }
int BookingSystem::getMaxConcurrentBookings() const { return maxConcurrentBookings; }
std::string BookingSystem::getCancellationPolicy() const { return cancellationPolicy; }
double BookingSystem::getLateCancellationFee() const { return lateCancellationFee; }

void BookingSystem::setBookingFeePercentage(double newFee) {
    bookingFeePercentage = std::max(0.0, newFee);
}

void BookingSystem::setSecurityDepositPercentage(double newDeposit) {
    securityDepositPercentage = std::max(0.0, newDeposit);
}

void BookingSystem::setCancellationPolicy(const std::string& newPolicy) {
    cancellationPolicy = newPolicy;
}

bool BookingSystem::createBooking(std::shared_ptr<Property> property, std::shared_ptr<Tenant> tenant,
                                 const std::chrono::system_clock::time_point& startDate,
                                 const std::chrono::system_clock::time_point& endDate,
                                 double rentalPrice) {
    if (!property || !tenant) return false;
    
    auto calendarIt = propertyCalendars.find(property->getId());
    if (calendarIt == propertyCalendars.end()) return false;
    
    auto calendar = calendarIt->second;
    if (!calendar->isAvailableForPeriod(startDate, endDate)) return false;
    
    int stayDays = std::chrono::duration_cast<std::chrono::hours>(endDate - startDate).count() / 24;
    if (!calendar->canAccommodateStay(stayDays)) return false;
    
    return true;
}

bool BookingSystem::cancelBooking(const std::string& propertyId, const std::string& tenantId,
                                 const std::chrono::system_clock::time_point& cancellationDate) {
    auto calendarIt = propertyCalendars.find(propertyId);
    if (calendarIt == propertyCalendars.end()) return false;
    
    return true;
}

double BookingSystem::calculateBookingFee(double rentalPrice) const {
    return rentalPrice * (bookingFeePercentage / 100.0);
}

double BookingSystem::calculateSecurityDeposit(double rentalPrice) const {
    return rentalPrice * (securityDepositPercentage / 100.0);
}

double BookingSystem::calculateCancellationFee(const std::chrono::system_clock::time_point& bookingDate,
                                              const std::chrono::system_clock::time_point& cancellationDate,
                                              double totalAmount) const {
    auto hoursUntilBooking = std::chrono::duration_cast<std::chrono::hours>(bookingDate - cancellationDate).count();
    
    if (cancellationPolicy == "flexible") {
        if (hoursUntilBooking < 24) return totalAmount * 0.1;
        return 0.0;
    } else if (cancellationPolicy == "moderate") {
        if (hoursUntilBooking < 48) return totalAmount * 0.5;
        if (hoursUntilBooking < 168) return totalAmount * 0.2;
        return 0.0;
    } else if (cancellationPolicy == "strict") {
        if (hoursUntilBooking < 168) return totalAmount * 0.8;
        if (hoursUntilBooking < 336) return totalAmount * 0.5;
        return totalAmount * 0.2;
    }
    
    return lateCancellationFee;
}

std::vector<std::shared_ptr<Property>> BookingSystem::searchAvailableProperties(
    const std::chrono::system_clock::time_point& startDate,
    const std::chrono::system_clock::time_point& endDate,
    const std::string& location) const {
    
    std::vector<std::shared_ptr<Property>> availableProperties;
    
    for (const auto& calendar : calendars) {
        if (calendar->isAvailableForPeriod(startDate, endDate)) {
            auto property = calendar->getProperty();
            if (property && (location.empty() || 
                property->getAddress()->getCity().find(location) != std::string::npos)) {
                availableProperties.push_back(property);
            }
        }
    }
    
    return availableProperties;
}

bool BookingSystem::modifyBooking(const std::string& propertyId, const std::string& tenantId,
                                 const std::chrono::system_clock::time_point& newStartDate,
                                 const std::chrono::system_clock::time_point& newEndDate) {
    auto calendarIt = propertyCalendars.find(propertyId);
    if (calendarIt == propertyCalendars.end()) return false;
    
    auto calendar = calendarIt->second;
    return calendar->isAvailableForPeriod(newStartDate, newEndDate);
}

double BookingSystem::calculateTotalBookingCost(double rentalPrice, int stayDays) const {
    double baseCost = rentalPrice * stayDays;
    double bookingFee = calculateBookingFee(baseCost);
    double securityDeposit = calculateSecurityDeposit(baseCost);
    
    return baseCost + bookingFee + securityDeposit;
}

bool BookingSystem::isPropertyAvailableForBooking(const std::string& propertyId,
                                                 const std::chrono::system_clock::time_point& startDate,
                                                 const std::chrono::system_clock::time_point& endDate) const {
    auto calendarIt = propertyCalendars.find(propertyId);
    if (calendarIt == propertyCalendars.end()) return false;
    
    return calendarIt->second->isAvailableForPeriod(startDate, endDate);
}

int BookingSystem::getPropertyBookingsCount(const std::string& propertyId) const {
    auto calendarIt = propertyCalendars.find(propertyId);
    if (calendarIt == propertyCalendars.end()) return 0;
    
    return calendarIt->second->getBookedDates().size();
}