#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class MaintenanceScheduler;
class DamageAssessor;

class Inventory {
private:
    std::unordered_map<std::string, std::shared_ptr<RentalItem>> items;
    std::unordered_map<std::string, int> categoryCounts;
    std::unordered_map<std::string, double> categoryPricing;
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::unordered_map<std::string, std::vector<std::string>> itemLocations;
    int totalItems;
    int rentedItemsCount;

public:
    Inventory();
    Inventory(std::shared_ptr<MaintenanceScheduler> scheduler, 
              std::shared_ptr<DamageAssessor> assessor);
    
    bool isItemAvailable(const std::string& itemId) const;
    void markItemAsRented(const std::string& itemId);
    void markItemAsReturned(const std::string& itemId);
    std::unordered_map<std::string, double> getUtilizationByCategory() const;
    void adjustCategoryCapacity(const std::string& category, double percentage);
    double getAveragePriceForCategory(const std::string& category) const;
    void adjustCategoryPricing(const std::string& category, double multiplier);
    std::vector<std::string> getAllCategories() const;
    double calculateTotalInventoryValue() const;
    std::vector<std::string> findItemsForMaintenance() const;
    bool relocateItem(const std::string& itemId, const std::string& newLocation);
    
    int getTotalItemCount() const { return totalItems; }
    int getRentedItemCount() const { return rentedItemsCount; }
    std::shared_ptr<MaintenanceScheduler> getMaintenanceScheduler() const;
    std::shared_ptr<DamageAssessor> getDamageAssessor() const;
    
    void setMaintenanceScheduler(std::shared_ptr<MaintenanceScheduler> scheduler);
    void setDamageAssessor(std::shared_ptr<DamageAssessor> assessor);
    
    void addItem(std::shared_ptr<RentalItem> item);
    void addItemWithLocation(std::shared_ptr<RentalItem> item, const std::string& location);
};

#endif