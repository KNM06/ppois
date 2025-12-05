#ifndef SUPPLIERMANAGER_H
#define SUPPLIERMANAGER_H

#include "../core/Inventory.h"
#include "../core/RentalItem.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

struct Supplier {
    std::string supplierId;
    std::string name;
    std::string contactInfo;
    std::vector<std::string> providedCategories;
    double reliabilityScore;
    double deliveryTimeDays;
};

class SupplierManager {
private:
    std::shared_ptr<Inventory> inventory;
    std::unordered_map<std::string, Supplier> suppliers;
    std::unordered_map<std::string, std::string> itemSuppliers;

public:
    SupplierManager(std::shared_ptr<Inventory> inv);
    
    void addSupplier(const Supplier& supplier);
    bool assignSupplierToItem(const std::string& itemId, const std::string& supplierId);
    double calculateProcurementCost(const std::string& itemId, int quantity) const;
    std::vector<std::string> findReliableSuppliers(const std::string& category, double minReliability) const;
    double predictDeliveryTime(const std::string& itemId) const;
    void updateSupplierPerformance(const std::string& supplierId, double performanceScore);
};

#endif