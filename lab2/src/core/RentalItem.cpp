#include "../../include/core/RentalItem.h"
#include "../../include/core/RentalPricing.h"
#include <cmath>
#include <algorithm>

RentalItem::RentalItem(const std::string& id, const std::string& itemName, 
                       const std::string& itemCategory, double basePrice)
    : itemId(id), name(itemName), category(itemCategory), basePricePerDay(basePrice),
      condition(ItemCondition::NEW), totalRentalDays(0), maintenanceIntervalDays(90),
      daysSinceLastMaintenance(0), isAvailable(true) {}

double RentalItem::calculateRentalPrice(int days, const std::string& customerType) const {
    RentalPricing pricing;
    
    std::string currentSeason = "SUMMER"; 
    double basePrice = pricing.calculateDynamicPrice(category, days, currentSeason, customerType);
    
    double conditionMultiplier = 1.0;
    switch (condition) {
        case ItemCondition::NEW: conditionMultiplier = 1.2; break;
        case ItemCondition::EXCELLENT: conditionMultiplier = 1.1; break;
        case ItemCondition::GOOD: conditionMultiplier = 1.0; break;
        case ItemCondition::FAIR: conditionMultiplier = 0.9; break;
        case ItemCondition::POOR: conditionMultiplier = 0.7; break;
        default: conditionMultiplier = 0.5; break;
    }
    
    return basePrice * conditionMultiplier;
}

bool RentalItem::requiresMaintenance() const {
    return daysSinceLastMaintenance >= maintenanceIntervalDays || 
           condition == ItemCondition::MAINTENANCE_REQUIRED;
}

double RentalItem::calculateDepreciation() const {
    double depreciationRate = 0.0;
    
    switch (condition) {
        case ItemCondition::NEW: depreciationRate = 0.05; break;
        case ItemCondition::EXCELLENT: depreciationRate = 0.15; break;
        case ItemCondition::GOOD: depreciationRate = 0.30; break;
        case ItemCondition::FAIR: depreciationRate = 0.50; break;
        case ItemCondition::POOR: depreciationRate = 0.75; break;
        default: depreciationRate = 0.90; break;
    }
    
    double usageDepreciation = (static_cast<double>(totalRentalDays) / 365.0) * 0.1;
    return basePricePerDay * (depreciationRate + usageDepreciation);
}

double RentalItem::calculateInsurancePremium() const {
    double basePremium = basePricePerDay * 0.02; 
    double riskMultiplier = 1.0;
    
    if (category == "VEHICLE") riskMultiplier = 1.5;
    else if (category == "ELECTRONICS") riskMultiplier = 1.3;
    else if (category == "CONSTRUCTION") riskMultiplier = 2.0;
    
    if (condition == ItemCondition::POOR || condition == ItemCondition::MAINTENANCE_REQUIRED) {
        riskMultiplier *= 1.5;
    }
    
    return basePremium * riskMultiplier;
}

int RentalItem::getRemainingUsefulLife() const {
    int baseLifeExpectancy = 1095; 
    int daysUsed = totalRentalDays;
    int maintenanceImpact = (daysSinceLastMaintenance > maintenanceIntervalDays) ? 100 : 0;
    
    return std::max(0, baseLifeExpectancy - daysUsed - maintenanceImpact);
}

bool RentalItem::validateRentalDuration(int days) const {
    if (days <= 0) return false;
    
    int maxRentalDays = 365;
    if (category == "VEHICLE") maxRentalDays = 30;
    else if (category == "ELECTRONICS") maxRentalDays = 14;
    else if (category == "CONSTRUCTION") maxRentalDays = 90;
    
    return days <= maxRentalDays;
}

void RentalItem::updateMaintenanceStatus(int daysRented) {
    totalRentalDays += daysRented;
    daysSinceLastMaintenance += daysRented;
    
    if (totalRentalDays > 300 && condition == ItemCondition::NEW) {
        condition = ItemCondition::EXCELLENT;
    } else if (totalRentalDays > 600 && condition == ItemCondition::EXCELLENT) {
        condition = ItemCondition::GOOD;
    } else if (totalRentalDays > 900 && condition == ItemCondition::GOOD) {
        condition = ItemCondition::FAIR;
    } else if (totalRentalDays > 1200 && condition == ItemCondition::FAIR) {
        condition = ItemCondition::POOR;
    }
}