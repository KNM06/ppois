#include "../../include/business/LateFeeCalculator.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/utils/DateUtils.h"
#include <algorithm>
#include <cmath>

LateFeeCalculator::LateFeeCalculator() 
    : baseLateFeePerDay(25.0), customerManager(nullptr) {
    
    categoryLateRates["VEHICLE"] = 1.5;
    categoryLateRates["ELECTRONICS"] = 1.2;
    categoryLateRates["CONSTRUCTION"] = 2.0;
    categoryLateRates["TOOLS"] = 1.0;
    categoryLateRates["SPORTS"] = 1.1;
    categoryLateRates["LUXURY"] = 2.5;
    
    customerPenaltyMultipliers["NEW"] = 1.2;
    customerPenaltyMultipliers["REGULAR"] = 1.0;
    customerPenaltyMultipliers["VIP"] = 0.7;
    customerPenaltyMultipliers["BUSINESS"] = 1.1;
    customerPenaltyMultipliers["STUDENT"] = 0.9;
    
    gracePeriodDays["VIP"] = 2;
    gracePeriodDays["BUSINESS"] = 1;
    gracePeriodDays["REGULAR"] = 0;
    gracePeriodDays["NEW"] = 0;
}

LateFeeCalculator::LateFeeCalculator(std::shared_ptr<CustomerManager> custManager)
    : baseLateFeePerDay(25.0), customerManager(custManager) {
    
    categoryLateRates["VEHICLE"] = 1.5;
    categoryLateRates["ELECTRONICS"] = 1.2;
    categoryLateRates["CONSTRUCTION"] = 2.0;
    categoryLateRates["TOOLS"] = 1.0;
    categoryLateRates["SPORTS"] = 1.1;
    categoryLateRates["LUXURY"] = 2.5;
    
    customerPenaltyMultipliers["NEW"] = 1.2;
    customerPenaltyMultipliers["REGULAR"] = 1.0;
    customerPenaltyMultipliers["VIP"] = 0.7;
    customerPenaltyMultipliers["BUSINESS"] = 1.1;
    customerPenaltyMultipliers["STUDENT"] = 0.9;
    
    gracePeriodDays["VIP"] = 2;
    gracePeriodDays["BUSINESS"] = 1;
    gracePeriodDays["REGULAR"] = 0;
    gracePeriodDays["NEW"] = 0;
}

double LateFeeCalculator::calculateLateFee(const std::string& category, double dailyRate, 
                                         int lateDays, const std::string& customerType) const {
    if (lateDays <= 0) return 0.0;
    
    if (isGracePeriodApplicable(customerType, lateDays)) {
        return 0.0;
    }
    
    double baseFee = baseLateFeePerDay * lateDays;
    
    auto categoryIt = categoryLateRates.find(category);
    double categoryMultiplier = (categoryIt != categoryLateRates.end()) ? categoryIt->second : 1.0;
    baseFee *= categoryMultiplier;
    
    auto customerIt = customerPenaltyMultipliers.find(customerType);
    double customerMultiplier = (customerIt != customerPenaltyMultipliers.end()) ? customerIt->second : 1.0;
    baseFee *= customerMultiplier;
    
    double progressivePenalty = calculateProgressivePenalty(lateDays, baseFee);
    baseFee += progressivePenalty;
    
    double lostRevenue = calculateLostRevenuePenalty(category, lateDays);
    baseFee += lostRevenue;
    
    double adminFee = calculateAdministrativeFee(lateDays);
    baseFee += adminFee;
    
    double rentalValue = dailyRate * lateDays;
    double maxFee = rentalValue * 2.0;
    
    return std::min(maxFee, baseFee);
}

double LateFeeCalculator::calculateProgressivePenalty(int lateDays, double baseFee) const {
    double progressiveMultiplier = 1.0;
    
    if (lateDays > 14) {
        progressiveMultiplier = 2.0;
    } else if (lateDays > 7) {
        progressiveMultiplier = 1.5;
    } else if (lateDays > 3) {
        progressiveMultiplier = 1.2;
    }
    
    return baseFee * (progressiveMultiplier - 1.0);
}

bool LateFeeCalculator::shouldSuspendRentalPrivileges(const std::string& customerId, int totalLateDays) const {
    if (totalLateDays >= 30) {
        return true;
    }
    
    int lateIncidents = 0;
    if (lateIncidents >= 3) {
        return true;
    }
    
    if (totalLateDays >= 10 && lateIncidents >= 2) {
        return true;
    }
    
    bool hasUnpaidFines = false;
    if (hasUnpaidFines && totalLateDays >= 5) {
        return true;
    }
    
    return false;
}

double LateFeeCalculator::calculateLostRevenuePenalty(const std::string& category, int lateDays) const {
    double baseDailyRevenue = 0.0;
    
    if (category == "VEHICLE") baseDailyRevenue = 100.0;
    else if (category == "ELECTRONICS") baseDailyRevenue = 60.0;
    else if (category == "CONSTRUCTION") baseDailyRevenue = 150.0;
    else if (category == "TOOLS") baseDailyRevenue = 40.0;
    else if (category == "SPORTS") baseDailyRevenue = 50.0;
    else baseDailyRevenue = 70.0;
    
    double potentialRentalsLost = lateDays * 0.3;
    double lostRevenue = potentialRentalsLost * baseDailyRevenue;
    
    double downtimeCost = baseDailyRevenue * lateDays * 0.1;
    
    return lostRevenue + downtimeCost;
}

double LateFeeCalculator::calculateAdministrativeFee(int lateDays) const {
    double baseAdminFee = 15.0;
    
    if (lateDays > 7) {
        baseAdminFee += 25.0;
    }
    
    if (lateDays > 14) {
        baseAdminFee += 50.0;
    }
    
    double notificationFee = 5.0 * (lateDays / 3);
    
    return baseAdminFee + notificationFee;
}

bool LateFeeCalculator::isGracePeriodApplicable(const std::string& customerType, int lateDays) const {
    if (lateDays <= 0) return true;
    
    if (customerType == "VIP" && lateDays <= 2) {
        return true;
    }
    
    if (customerType == "BUSINESS" && lateDays <= 1) {
        return true;
    }
    
    bool isFirstLate = true;
    if (isFirstLate && lateDays <= 1) {
        return true;
    }
    
    bool isHoliday = false;
    if (isHoliday && lateDays <= 1) {
        return true;
    }
    
    return false;
}

double LateFeeCalculator::calculateTotalPenalty(const std::string& customerId, const std::string& category, 
                                              int lateDays, double dailyRate) const {
    if (customerManager) {
        auto customer = customerManager->getCustomer(customerId);
        if (customer) {
            std::string customerType = "REGULAR";
            switch (customer->getStatus()) {
                case CustomerStatus::VIP: customerType = "VIP"; break;
                case CustomerStatus::BUSINESS: customerType = "BUSINESS"; break;
                case CustomerStatus::NEW: customerType = "NEW"; break;
                default: customerType = "REGULAR";
            }
            
            return calculateLateFee(category, dailyRate, lateDays, customerType);
        }
    }
    
    return calculateLateFee(category, dailyRate, lateDays, "REGULAR");
}

int LateFeeCalculator::getGracePeriodForCustomer(const std::string& customerType) const {
    auto it = gracePeriodDays.find(customerType);
    if (it != gracePeriodDays.end()) {
        return it->second;
    }
    return 0;
}

std::shared_ptr<CustomerManager> LateFeeCalculator::getCustomerManager() const {
    return customerManager;
}

void LateFeeCalculator::updateCategoryLateRate(const std::string& category, double rate) {
    categoryLateRates[category] = std::max(0.5, std::min(5.0, rate));
}

void LateFeeCalculator::setCustomerManager(std::shared_ptr<CustomerManager> manager) {
    customerManager = manager;
}

void LateFeeCalculator::setGracePeriod(const std::string& customerType, int days) {
    gracePeriodDays[customerType] = std::max(0, std::min(7, days));
}