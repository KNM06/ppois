#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Property.h"

class Inventory {
private:
    std::string inventoryId;
    std::shared_ptr<Property> property;
    std::string itemName;
    std::string category;
    int quantity;
    int minimumStock;
    double unitCost;
    std::chrono::system_clock::time_point lastRestocked;
    std::string storageLocation;
    std::string condition;
    bool isConsumable;
    std::string supplier;

public:
    Inventory(const std::string& id, std::shared_ptr<Property> prop,
              const std::string& itemName, const std::string& category,
              int quantity, int minimumStock, double unitCost,
              const std::chrono::system_clock::time_point& lastRestocked,
              const std::string& storageLocation, const std::string& condition,
              bool consumable, const std::string& supplier);
    
    Inventory(const Inventory& other);
    Inventory& operator=(const Inventory& other);
    Inventory(Inventory&& other) noexcept;
    Inventory& operator=(Inventory&& other) noexcept;
    ~Inventory();

    std::string getInventoryId() const;
    Property* getProperty() const;
    std::string getItemName() const;
    std::string getCategory() const;
    int getQuantity() const;
    int getMinimumStock() const;
    double getUnitCost() const;
    std::chrono::system_clock::time_point getLastRestocked() const;
    std::string getStorageLocation() const;
    std::string getCondition() const;
    bool getIsConsumable() const;
    std::string getSupplier() const;

    void setQuantity(int newQuantity);
    void setUnitCost(double newCost);
    void updateLastRestocked();

    bool needsRestocking() const;
    double calculateTotalValue() const;
    bool isLowStock() const;
    int calculateMonthsSinceLastRestock(const std::chrono::system_clock::time_point& currentDate) const;
    bool requiresMaintenance() const;
    double calculateDepreciation() const;
    bool isCriticalItem() const;
    double calculateReplacementCost() const;
    std::string getInventoryStatus() const;
};

#endif