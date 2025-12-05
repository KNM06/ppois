#ifndef ENVIRONMENTALCOMPLIANCE_H
#define ENVIRONMENTALCOMPLIANCE_H

#include "../../include/core/Inventory.h"
#include "../../include/business/MaintenanceScheduler.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class EnvironmentalCompliance {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::unordered_map<std::string, double> categoryEnvironmentalImpact;
    std::unordered_map<std::string, double> disposalCosts;
    std::vector<std::string> ecoFriendlyCategories;
    double carbonFootprintFactor;

public:
    EnvironmentalCompliance(std::shared_ptr<Inventory> inv, 
                           std::shared_ptr<MaintenanceScheduler> scheduler);
    
    double calculateCarbonFootprint(const std::string& itemId, int usageDays) const;
    double calculateRecyclingCost(const std::string& category, double itemValue) const;
    bool isEcoFriendlyCategory(const std::string& category) const;
    double calculateEnvironmentalPenalty(const std::string& category, const std::string& damageType) const;
    std::vector<std::string> getEcoFriendlyRecommendations() const;
    double calculateGreenDiscount(const std::string& category) const;
    bool requiresEnvironmentalDisclosure(const std::string& category) const;
    double calculateWasteManagementCost(const std::string& itemId) const;
    
    double getCarbonFootprintFactor() const;
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<MaintenanceScheduler> getMaintenanceScheduler() const;
    
    void setCarbonFootprintFactor(double factor);
    void setInventory(std::shared_ptr<Inventory> inv);
    void setMaintenanceScheduler(std::shared_ptr<MaintenanceScheduler> scheduler);
    void addEcoFriendlyCategory(const std::string& category);
};

#endif