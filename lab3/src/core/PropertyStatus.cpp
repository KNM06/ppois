#include "../../include/core/PropertyStatus.h"
#include <chrono>
#include <algorithm>

PropertyStatus::PropertyStatus(const std::string& code, const std::string& desc,
                               bool available, bool maintenance, bool rented,
                               const std::chrono::system_clock::time_point& date,
                               const std::string& reason, double discount, int daysOnMarket)
    : statusCode(code), description(desc), isAvailable(available),
      isUnderMaintenance(maintenance), isRented(rented), statusDate(date),
      statusReason(reason), discountPercentage(discount), daysOnMarket(daysOnMarket) {}
PropertyStatus::PropertyStatus(const std::string& code)
    : statusCode(code),
      description(""),
      isAvailable(code == "AVAILABLE"),
      isUnderMaintenance(code == "MAINTENANCE"),
      isRented(code == "RENTED"),
      statusDate(std::chrono::system_clock::now()),
      statusReason(""),
      discountPercentage(0.0),
      daysOnMarket(0) {}

PropertyStatus::PropertyStatus(const PropertyStatus& other)
    : statusCode(other.statusCode), description(other.description),
      isAvailable(other.isAvailable), isUnderMaintenance(other.isUnderMaintenance),
      isRented(other.isRented), statusDate(other.statusDate),
      statusReason(other.statusReason), discountPercentage(other.discountPercentage),
      daysOnMarket(other.daysOnMarket) {}
PropertyStatus::PropertyStatus() :
    statusCode(""), description(""), isAvailable(true),
    isUnderMaintenance(false), isRented(false),
    statusDate(std::chrono::system_clock::now()),
    statusReason(""), discountPercentage(0.0), daysOnMarket(0) {}

PropertyStatus& PropertyStatus::operator=(const PropertyStatus& other) {
    if (this != &other) {
        statusCode = other.statusCode;
        description = other.description;
        isAvailable = other.isAvailable;
        isUnderMaintenance = other.isUnderMaintenance;
        isRented = other.isRented;
        statusDate = other.statusDate;
        statusReason = other.statusReason;
        discountPercentage = other.discountPercentage;
        daysOnMarket = other.daysOnMarket;
    }
    return *this;
}

PropertyStatus::PropertyStatus(PropertyStatus&& other) noexcept
    : statusCode(std::move(other.statusCode)), description(std::move(other.description)),
      isAvailable(other.isAvailable), isUnderMaintenance(other.isUnderMaintenance),
      isRented(other.isRented), statusDate(other.statusDate),
      statusReason(std::move(other.statusReason)), discountPercentage(other.discountPercentage),
      daysOnMarket(other.daysOnMarket) {}

PropertyStatus& PropertyStatus::operator=(PropertyStatus&& other) noexcept {
    if (this != &other) {
        statusCode = std::move(other.statusCode);
        description = std::move(other.description);
        isAvailable = other.isAvailable;
        isUnderMaintenance = other.isUnderMaintenance;
        isRented = other.isRented;
        statusDate = other.statusDate;
        statusReason = std::move(other.statusReason);
        discountPercentage = other.discountPercentage;
        daysOnMarket = other.daysOnMarket;
    }
    return *this;
}

PropertyStatus::~PropertyStatus() {}

std::string PropertyStatus::getStatusCode() const { return statusCode; }
std::string PropertyStatus::getDescription() const { return description; }
bool PropertyStatus::getIsAvailable() const { return isAvailable; }
bool PropertyStatus::getIsUnderMaintenance() const { return isUnderMaintenance; }
bool PropertyStatus::getIsRented() const { return isRented; }
std::chrono::system_clock::time_point PropertyStatus::getStatusDate() const { return statusDate; }
std::string PropertyStatus::getStatusReason() const { return statusReason; }
double PropertyStatus::getDiscountPercentage() const { return discountPercentage; }
int PropertyStatus::getDaysOnMarket() const { return daysOnMarket; }

void PropertyStatus::setStatusCode(const std::string& newCode) {
    statusCode = newCode;
}

void PropertyStatus::setDescription(const std::string& newDesc) {
    description = newDesc;
}

void PropertyStatus::updateStatus(bool available, bool maintenance, bool rented) {
    isAvailable = available;
    isUnderMaintenance = maintenance;
    isRented = rented;
}

void PropertyStatus::setDiscountPercentage(double discount) {
    discountPercentage = std::max(0.0, std::min(50.0, discount));
}

bool PropertyStatus::canShowProperty() const {
    return isAvailable && !isUnderMaintenance;
}

bool PropertyStatus::requiresPriceAdjustment() const {
    return daysOnMarket > 60 && discountPercentage == 0.0;
}

double PropertyStatus::calculateDiscountedPrice(double originalPrice) const {
    return originalPrice * (1.0 - (discountPercentage / 100.0));
}

bool PropertyStatus::isLongTermVacancy(int thresholdDays) const {
    return isAvailable && daysOnMarket > thresholdDays;
}

bool PropertyStatus::shouldIncreaseMarketing() const {
    return isAvailable && daysOnMarket > 30 && discountPercentage < 10.0;
}

bool PropertyStatus::isEligibleForPromotion() const {
    return isAvailable && !isUnderMaintenance && daysOnMarket > 45;
}

bool PropertyStatus::needsMaintenanceCheck() const {
    return isUnderMaintenance || (!isAvailable && !isRented);
}

std::string PropertyStatus::getMarketAnalysis() const {
    if (daysOnMarket < 15) return "new_listing";
    if (daysOnMarket < 45) return "normal_market";
    if (daysOnMarket < 90) return "slow_market";
    return "stagnant_listing";
}