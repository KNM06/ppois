#include "../../include/business/SupplierManager.h"

SupplierManager::SupplierManager(std::shared_ptr<Inventory> inv) : inventory(inv) {}

void SupplierManager::addSupplier(const Supplier& supplier) {
    suppliers[supplier.supplierId] = supplier;
}

bool SupplierManager::assignSupplierToItem(const std::string& itemId, const std::string& supplierId) {
    if (suppliers.find(supplierId) == suppliers.end()) {
        return false;
    }
    
    itemSuppliers[itemId] = supplierId;
    return true;
}

double SupplierManager::calculateProcurementCost(const std::string& itemId, int quantity) const {
    if (itemSuppliers.find(itemId) == itemSuppliers.end()) {
        return 0.0;
    }
    
    std::string supplierId = itemSuppliers.at(itemId);
    const Supplier& supplier = suppliers.at(supplierId);
    
    double baseItemCost = 100.0;
    double reliabilityDiscount = supplier.reliabilityScore * 0.1;
    double bulkDiscount = quantity > 10 ? 0.15 : (quantity > 5 ? 0.10 : 0.05);
    
    double unitCost = baseItemCost * (1.0 - reliabilityDiscount) * (1.0 - bulkDiscount);
    return unitCost * quantity;
}

std::vector<std::string> SupplierManager::findReliableSuppliers(const std::string& category, double minReliability) const {
    std::vector<std::string> reliableSuppliers;
    
    for (const auto& [supplierId, supplier] : suppliers) {
        if (supplier.reliabilityScore >= minReliability) {
            for (const auto& cat : supplier.providedCategories) {
                if (cat == category) {
                    reliableSuppliers.push_back(supplierId);
                    break;
                }
            }
        }
    }
    
    return reliableSuppliers;
}

double SupplierManager::predictDeliveryTime(const std::string& itemId) const {
    if (itemSuppliers.find(itemId) == itemSuppliers.end()) {
        return 7.0;
    }
    
    std::string supplierId = itemSuppliers.at(itemId);
    const Supplier& supplier = suppliers.at(supplierId);
    
    double baseDeliveryTime = supplier.deliveryTimeDays;
    double reliabilityFactor = 1.0 - (supplier.reliabilityScore * 0.1);
    
    return baseDeliveryTime * reliabilityFactor;
}

void SupplierManager::updateSupplierPerformance(const std::string& supplierId, double performanceScore) {
    if (suppliers.find(supplierId) != suppliers.end()) {
        suppliers[supplierId].reliabilityScore = performanceScore;
    }
}