#include "../../include/core/Inventory.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/DamageAssessor.h"
#include <algorithm>
#include <stdexcept>

Inventory::Inventory() 
    : totalItems(0), rentedItemsCount(0), maintenanceScheduler(nullptr), damageAssessor(nullptr) {}

Inventory::Inventory(std::shared_ptr<MaintenanceScheduler> scheduler, 
                   std::shared_ptr<DamageAssessor> assessor)
    : totalItems(0), rentedItemsCount(0), maintenanceScheduler(scheduler), damageAssessor(assessor) {}

bool Inventory::isItemAvailable(const std::string& itemId) const {
    auto it = items.find(itemId);
    if (it != items.end()) {
        return it->second->getIsAvailable();
    }
    return false;
}

void Inventory::markItemAsRented(const std::string& itemId) {
    auto it = items.find(itemId);
    if (it != items.end() && it->second->getIsAvailable()) {
        it->second->markAsRented();
        rentedItemsCount++;
    }
}

void Inventory::markItemAsReturned(const std::string& itemId) {
    auto it = items.find(itemId);
    if (it != items.end() && !it->second->getIsAvailable()) {
        it->second->markAsReturned();
        rentedItemsCount--;
    }
}

std::unordered_map<std::string, double> Inventory::getUtilizationByCategory() const {
    std::unordered_map<std::string, double> utilization;
    std::unordered_map<std::string, int> rentedByCategory;
    std::unordered_map<std::string, int> totalByCategory;
    
    for (const auto& [id, item] : items) {
        std::string category = item->getCategory();
        totalByCategory[category]++;
        if (!item->getIsAvailable()) {
            rentedByCategory[category]++;
        }
    }
    
    for (const auto& [category, total] : totalByCategory) {
        int rented = rentedByCategory[category];
        utilization[category] = static_cast<double>(rented) / total;
    }
    
    return utilization;
}

void Inventory::adjustCategoryCapacity(const std::string& category, double percentage) {
    if (categoryPricing.find(category) != categoryPricing.end()) {
        double currentPrice = categoryPricing[category];
        double adjustment = 1.0 + (percentage * 0.5);
        categoryPricing[category] = currentPrice * adjustment;
    }
}

double Inventory::getAveragePriceForCategory(const std::string& category) const {
    auto it = categoryPricing.find(category);
    if (it != categoryPricing.end()) {
        return it->second;
    }
    
    double total = 0.0;
    int count = 0;
    for (const auto& [id, item] : items) {
        if (item->getCategory() == category) {
            total += 50.0;
            count++;
        }
    }
    
    return count > 0 ? total / count : 0.0;
}

void Inventory::adjustCategoryPricing(const std::string& category, double multiplier) {
    categoryPricing[category] = getAveragePriceForCategory(category) * multiplier;
}

std::vector<std::string> Inventory::getAllCategories() const {
    std::vector<std::string> categories;
    for (const auto& [id, item] : items) {
        std::string category = item->getCategory();
        if (std::find(categories.begin(), categories.end(), category) == categories.end()) {
            categories.push_back(category);
        }
    }
    return categories;
}

double Inventory::calculateTotalInventoryValue() const {
    double totalValue = 0.0;
    for (const auto& [id, item] : items) {
        totalValue += item->calculateDepreciation();
    }
    return totalValue;
}

std::vector<std::string> Inventory::findItemsForMaintenance() const {
    std::vector<std::string> maintenanceItems;
    for (const auto& [id, item] : items) {
        if (item->requiresMaintenance()) {
            maintenanceItems.push_back(id);
        }
    }
    return maintenanceItems;
}

bool Inventory::relocateItem(const std::string& itemId, const std::string& newLocation) {
    auto itemIt = items.find(itemId);
    if (itemIt != items.end()) {
        if (itemIt->second->getIsAvailable()) {
            itemLocations[itemId] = {newLocation};
            return true;
        }
    }
    return false;
}

std::shared_ptr<MaintenanceScheduler> Inventory::getMaintenanceScheduler() const {
    return maintenanceScheduler;
}

std::shared_ptr<DamageAssessor> Inventory::getDamageAssessor() const {
    return damageAssessor;
}

void Inventory::setMaintenanceScheduler(std::shared_ptr<MaintenanceScheduler> scheduler) {
    maintenanceScheduler = scheduler;
}

void Inventory::setDamageAssessor(std::shared_ptr<DamageAssessor> assessor) {
    damageAssessor = assessor;
}

void Inventory::addItem(std::shared_ptr<RentalItem> item) {
    std::string category = item->getCategory();
    items[item->getItemId()] = item;
    totalItems++;
    categoryCounts[category]++;
    
    if (categoryPricing.find(category) == categoryPricing.end()) {
        categoryPricing[category] = 50.0;
    }
}

void Inventory::addItemWithLocation(std::shared_ptr<RentalItem> item, const std::string& location) {
    addItem(item);
    itemLocations[item->getItemId()] = {location};
}