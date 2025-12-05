#ifndef WARRANTYMANAGER_H
#define WARRANTYMANAGER_H

#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

struct Warranty {
    std::string warrantyId;
    std::string itemId;
    std::string startDate;
    std::string endDate;
    std::string coverageType;
    double coverageAmount;
    bool isActive;
};

class WarrantyManager {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::unordered_map<std::string, Warranty> warranties;
    std::unordered_map<std::string, double> categoryWarrantyRates;
    std::unordered_map<std::string, int> warrantyDurations;

public:
    WarrantyManager(std::shared_ptr<Inventory> inv, std::shared_ptr<DamageAssessor> assessor);
    
    bool issueWarranty(const std::string& itemId, const std::string& coverageType, int durationMonths);
    double calculateWarrantyCost(const std::string& itemId, const std::string& coverageType) const;
    bool isWarrantyValid(const std::string& itemId, const std::string& currentDate) const;
    double processWarrantyClaim(const std::string& itemId, double damageCost);
    std::vector<std::string> getExpiringWarranties(int daysThreshold) const;
    double calculateWarrantyROI(const std::string& itemId) const;
    bool shouldExtendWarranty(const std::string& itemId) const;
    void updateWarrantyRates(const std::string& category, double rate);
    
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<DamageAssessor> getDamageAssessor() const;
    double getCategoryWarrantyRate(const std::string& category) const;
    
    void setInventory(std::shared_ptr<Inventory> inv);
    void setDamageAssessor(std::shared_ptr<DamageAssessor> assessor);
    void setWarrantyDuration(const std::string& category, int months);
};

#endif