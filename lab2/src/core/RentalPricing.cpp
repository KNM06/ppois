#include "../../include/core/RentalPricing.h"
#include <algorithm>
#include <cmath>

RentalPricing::RentalPricing() {
    categoryBaseRates["VEHICLE"] = 75.0;
    categoryBaseRates["ELECTRONICS"] = 45.0;
    categoryBaseRates["CONSTRUCTION"] = 120.0;
    categoryBaseRates["TOOLS"] = 25.0;
    categoryBaseRates["SPORTS"] = 30.0;
    categoryBaseRates["PARTY"] = 35.0;
    
    seasonalMultipliers["SPRING"] = 1.0;
    seasonalMultipliers["SUMMER"] = 1.3;
    seasonalMultipliers["AUTUMN"] = 0.9;
    seasonalMultipliers["WINTER"] = 0.8;
    
    customerTypeDiscounts["NEW"] = 0.0;
    customerTypeDiscounts["REGULAR"] = 0.05;
    customerTypeDiscounts["VIP"] = 0.15;
    customerTypeDiscounts["BUSINESS"] = 0.10;
}

double RentalPricing::calculateDynamicPrice(const std::string& category, int durationDays, 
                                          const std::string& season, const std::string& customerType) const {
    double baseRate = getBaseRate(category);
    double totalPrice = baseRate * durationDays;
    
    double seasonalMult = getSeasonalMultiplier(season);
    totalPrice *= seasonalMult;
    
    if (durationDays >= 30) {
        totalPrice *= 0.75; 
    } else if (durationDays >= 7) {
        totalPrice *= 0.85; 
    }
    
    double customerDiscount = getCustomerDiscount(customerType);
    totalPrice *= (1.0 - customerDiscount);
    
    return std::max(0.0, totalPrice);
}

double RentalPricing::applyLoyaltyDiscount(double basePrice, int loyaltyPoints) const {
    double pointsDiscount = std::min(0.2, loyaltyPoints * 0.0001);
    return basePrice * (1.0 - pointsDiscount);
}

double RentalPricing::calculateBulkDiscount(int itemCount, double totalPrice) const {
    double discount = 0.0;
    
    if (itemCount >= 10) {
        discount = 0.20; 
    } else if (itemCount >= 5) {
        discount = 0.15; 
    } else if (itemCount >= 3) {
        discount = 0.10;
    }
    
    return totalPrice * (1.0 - discount);
}

double RentalPricing::getSeasonalMultiplier(const std::string& season) const {
    auto it = seasonalMultipliers.find(season);
    if (it != seasonalMultipliers.end()) {
        return it->second;
    }
    return 1.0;
}

double RentalPricing::getCustomerDiscount(const std::string& customerType) const {
    auto it = customerTypeDiscounts.find(customerType);
    if (it != customerTypeDiscounts.end()) {
        return it->second;
    }
    return 0.0;
}

double RentalPricing::calculatePenaltyRate(int lateDays) const {
    double penaltyRate = 0.0;
    
    if (lateDays <= 0) return 0.0;
    
    if (lateDays <= 3) {
        penaltyRate = 0.1;
    } else if (lateDays <= 7) {
        penaltyRate = 0.25; 
    } else if (lateDays <= 14) {
        penaltyRate = 0.5; 
    } else {
        penaltyRate = 1.0; 
    }
    
    return penaltyRate;
}

double RentalPricing::getBaseRate(const std::string& category) const {
    auto it = categoryBaseRates.find(category);
    if (it != categoryBaseRates.end()) {
        return it->second;
    }
    return 30.0;
}

void RentalPricing::updateBaseRate(const std::string& category, double newRate) {
    categoryBaseRates[category] = std::max(0.0, newRate);
}