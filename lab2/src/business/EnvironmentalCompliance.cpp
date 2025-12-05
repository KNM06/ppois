#include "../../include/business/EnvironmentalCompliance.h"
#include <algorithm>
#include <cmath>

EnvironmentalCompliance::EnvironmentalCompliance(std::shared_ptr<Inventory> inv, 
                                               std::shared_ptr<MaintenanceScheduler> scheduler)
    : inventory(inv), maintenanceScheduler(scheduler), carbonFootprintFactor(0.1) {
    
    categoryEnvironmentalImpact["VEHICLE"] = 2.5;
    categoryEnvironmentalImpact["CONSTRUCTION"] = 3.0;
    categoryEnvironmentalImpact["ELECTRONICS"] = 1.8;
    categoryEnvironmentalImpact["TOOLS"] = 1.2;
    categoryEnvironmentalImpact["SPORTS"] = 0.8;
    
    disposalCosts["ELECTRONICS"] = 50.0;
    disposalCosts["BATTERIES"] = 25.0;
    disposalCosts["CHEMICALS"] = 100.0;
    disposalCosts["METALS"] = 30.0;
    
    ecoFriendlyCategories = {"BICYCLE", "MANUAL_TOOLS", "SOLAR_EQUIPMENT"};
}

double EnvironmentalCompliance::calculateCarbonFootprint(const std::string& itemId, int usageDays) const {
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    
    auto impactIt = categoryEnvironmentalImpact.find(category);
    double categoryImpact = impactIt != categoryEnvironmentalImpact.end() ? impactIt->second : 1.0;
    
    double maintenanceImpact = 0.0;
    if (maintenanceScheduler) {
        int maintenanceCount = maintenanceScheduler->getMaintenanceCount(itemId);
        maintenanceImpact = maintenanceCount * 0.1;
    }
    
    return categoryImpact * usageDays * carbonFootprintFactor + maintenanceImpact;
}

double EnvironmentalCompliance::calculateRecyclingCost(const std::string& category, double itemValue) const {
    double baseCost = itemValue * 0.05;
    
    auto disposalIt = disposalCosts.find(category);
    if (disposalIt != disposalCosts.end()) {
        baseCost += disposalIt->second;
    }
    
    if (category == "ELECTRONICS") {
        baseCost *= 1.5;
    } else if (category == "BATTERIES") {
        baseCost *= 2.0;
    }
    
    return baseCost;
}

bool EnvironmentalCompliance::isEcoFriendlyCategory(const std::string& category) const {
    return std::find(ecoFriendlyCategories.begin(), ecoFriendlyCategories.end(), category) != ecoFriendlyCategories.end();
}

double EnvironmentalCompliance::calculateEnvironmentalPenalty(const std::string& category, const std::string& damageType) const {
    double penalty = 0.0;
    
    if (damageType == "OIL_SPILL" || damageType == "CHEMICAL_LEAK") {
        penalty = 500.0;
    } else if (damageType == "HAZARDOUS_WASTE") {
        penalty = 300.0;
    } else if (damageType == "ELECTRONIC_WASTE") {
        penalty = 200.0;
    }
    
    auto impactIt = categoryEnvironmentalImpact.find(category);
    double categoryMultiplier = impactIt != categoryEnvironmentalImpact.end() ? impactIt->second : 1.0;
    
    return penalty * categoryMultiplier;
}

std::vector<std::string> EnvironmentalCompliance::getEcoFriendlyRecommendations() const {
    std::vector<std::string> recommendations;
    
    for (const auto& category : ecoFriendlyCategories) {
        recommendations.push_back("Consider " + category + " for lower environmental impact");
    }
    
    if (carbonFootprintFactor > 0.15) {
        recommendations.push_back("High carbon footprint detected - consider efficiency improvements");
    }
    
    return recommendations;
}

double EnvironmentalCompliance::calculateGreenDiscount(const std::string& category) const {
    if (isEcoFriendlyCategory(category)) {
        return 0.1;
    }
    
    auto impactIt = categoryEnvironmentalImpact.find(category);
    if (impactIt != categoryEnvironmentalImpact.end() && impactIt->second < 1.5) {
        return 0.05;
    }
    
    return 0.0;
}

bool EnvironmentalCompliance::requiresEnvironmentalDisclosure(const std::string& category) const {
    auto impactIt = categoryEnvironmentalImpact.find(category);
    if (impactIt != categoryEnvironmentalImpact.end() && impactIt->second > 2.0) {
        return true;
    }
    
    return category == "CHEMICALS" || category == "BATTERIES" || category == "ELECTRONICS";
}

double EnvironmentalCompliance::calculateWasteManagementCost(const std::string& itemId) const {
    std::string category = "GENERAL";
    if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    else if (itemId.find("BATT") != std::string::npos) category = "BATTERIES";
    
    double baseCost = calculateRecyclingCost(category, 100.0);
    
    if (maintenanceScheduler) {
        int maintenanceCount = maintenanceScheduler->getMaintenanceCount(itemId);
        baseCost += maintenanceCount * 5.0;
    }
    
    return baseCost;
}

double EnvironmentalCompliance::getCarbonFootprintFactor() const {
    return carbonFootprintFactor;
}

std::shared_ptr<Inventory> EnvironmentalCompliance::getInventory() const {
    return inventory;
}

std::shared_ptr<MaintenanceScheduler> EnvironmentalCompliance::getMaintenanceScheduler() const {
    return maintenanceScheduler;
}

void EnvironmentalCompliance::setCarbonFootprintFactor(double factor) {
    carbonFootprintFactor = std::max(0.0, factor);
}

void EnvironmentalCompliance::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void EnvironmentalCompliance::setMaintenanceScheduler(std::shared_ptr<MaintenanceScheduler> scheduler) {
    maintenanceScheduler = scheduler;
}

void EnvironmentalCompliance::addEcoFriendlyCategory(const std::string& category) {
    if (std::find(ecoFriendlyCategories.begin(), ecoFriendlyCategories.end(), category) == ecoFriendlyCategories.end()) {
        ecoFriendlyCategories.push_back(category);
    }
}