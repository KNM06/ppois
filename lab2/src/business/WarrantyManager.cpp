#include "../../include/business/WarrantyManager.h"
#include <algorithm>
#include <cmath>

WarrantyManager::WarrantyManager(std::shared_ptr<Inventory> inv, std::shared_ptr<DamageAssessor> assessor)
    : inventory(inv), damageAssessor(assessor) {
    
    categoryWarrantyRates["ELECTRONICS"] = 0.08;
    categoryWarrantyRates["VEHICLE"] = 0.12;
    categoryWarrantyRates["CONSTRUCTION"] = 0.15;
    categoryWarrantyRates["TOOLS"] = 0.05;
    categoryWarrantyRates["LUXURY"] = 0.10;
    
    warrantyDurations["ELECTRONICS"] = 24;
    warrantyDurations["VEHICLE"] = 12;
    warrantyDurations["CONSTRUCTION"] = 6;
    warrantyDurations["TOOLS"] = 36;
    warrantyDurations["LUXURY"] = 18;
}

bool WarrantyManager::issueWarranty(const std::string& itemId, const std::string& coverageType, int durationMonths) {
    Warranty warranty;
    warranty.warrantyId = "WARR" + std::to_string(warranties.size() + 1000);
    warranty.itemId = itemId;
    warranty.startDate = "2024-01-01";
    warranty.endDate = "2024-12-31";
    warranty.coverageType = coverageType;
    warranty.coverageAmount = calculateWarrantyCost(itemId, coverageType) * 10.0;
    warranty.isActive = true;
    
    warranties[warranty.warrantyId] = warranty;
    return true;
}

double WarrantyManager::calculateWarrantyCost(const std::string& itemId, const std::string& coverageType) const {
    std::string category = "GENERAL";
    if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    else if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    
    auto rateIt = categoryWarrantyRates.find(category);
    double baseRate = rateIt != categoryWarrantyRates.end() ? rateIt->second : 0.1;
    
    double coverageMultiplier = 1.0;
    if (coverageType == "PREMIUM") coverageMultiplier = 1.5;
    else if (coverageType == "BASIC") coverageMultiplier = 0.7;
    
    double itemValue = 1000.0;
    double maintenanceFactor = 1.0;
    
    if (damageAssessor) {
        double damageProbability = damageAssessor->assessDamageCost(category, itemValue, "MINOR_SCRATCH", "MINOR") / itemValue;
        maintenanceFactor += damageProbability * 0.5;
    }
    
    return itemValue * baseRate * coverageMultiplier * maintenanceFactor;
}

bool WarrantyManager::isWarrantyValid(const std::string& itemId, const std::string& currentDate) const {
    for (const auto& [warrantyId, warranty] : warranties) {
        if (warranty.itemId == itemId && warranty.isActive) {
            return currentDate <= warranty.endDate;
        }
    }
    return false;
}

double WarrantyManager::processWarrantyClaim(const std::string& itemId, double damageCost) {
    for (auto& [warrantyId, warranty] : warranties) {
        if (warranty.itemId == itemId && warranty.isActive) {
            double coveredAmount = std::min(damageCost, warranty.coverageAmount);
            warranty.coverageAmount -= coveredAmount;
            
            if (warranty.coverageAmount <= 0) {
                warranty.isActive = false;
            }
            
            return coveredAmount;
        }
    }
    return 0.0;
}

std::vector<std::string> WarrantyManager::getExpiringWarranties(int daysThreshold) const {
    std::vector<std::string> expiringWarranties;
    
    std::string currentDate = "2024-01-01";
    std::string thresholdDate = "2024-12-31";
    
    for (const auto& [warrantyId, warranty] : warranties) {
        if (warranty.isActive && warranty.endDate <= thresholdDate) {
            expiringWarranties.push_back(warranty.itemId + " - Warranty expires: " + warranty.endDate);
        }
    }
    
    return expiringWarranties;
}

double WarrantyManager::calculateWarrantyROI(const std::string& itemId) const {
    double totalWarrantyCost = 0.0;
    double totalClaimsPaid = 0.0;
    
    for (const auto& [warrantyId, warranty] : warranties) {
        if (warranty.itemId == itemId) {
            totalWarrantyCost += calculateWarrantyCost(itemId, warranty.coverageType);
            
            double initialCoverage = warranty.coverageAmount * 10.0;
            double remainingCoverage = warranty.isActive ? warranty.coverageAmount : 0.0;
            totalClaimsPaid += initialCoverage - remainingCoverage;
        }
    }
    
    if (totalWarrantyCost == 0.0) return 0.0;
    return (totalClaimsPaid - totalWarrantyCost) / totalWarrantyCost;
}

bool WarrantyManager::shouldExtendWarranty(const std::string& itemId) const {
    double roi = calculateWarrantyROI(itemId);
    int claimCount = 0;
    
    for (const auto& [warrantyId, warranty] : warranties) {
        if (warranty.itemId == itemId && !warranty.isActive) {
            claimCount++;
        }
    }
    
    return roi > 0.5 && claimCount < 3;
}

void WarrantyManager::updateWarrantyRates(const std::string& category, double rate) {
    categoryWarrantyRates[category] = std::max(0.01, std::min(0.5, rate));
}

std::shared_ptr<Inventory> WarrantyManager::getInventory() const {
    return inventory;
}

std::shared_ptr<DamageAssessor> WarrantyManager::getDamageAssessor() const {
    return damageAssessor;
}

double WarrantyManager::getCategoryWarrantyRate(const std::string& category) const {
    auto it = categoryWarrantyRates.find(category);
    return it != categoryWarrantyRates.end() ? it->second : 0.1;
}

void WarrantyManager::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void WarrantyManager::setDamageAssessor(std::shared_ptr<DamageAssessor> assessor) {
    damageAssessor = assessor;
}

void WarrantyManager::setWarrantyDuration(const std::string& category, int months) {
    warrantyDurations[category] = std::max(1, months);
}