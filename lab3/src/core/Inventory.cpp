#include "../../include/core/Inventory.h"
#include <chrono>
#include <algorithm>

Inventory::Inventory(const std::string& id, std::shared_ptr<Property> prop,
                     const std::string& itemName, const std::string& category,
                     int quantity, int minimumStock, double unitCost,
                     const std::chrono::system_clock::time_point& lastRestocked,
                     const std::string& storageLocation, const std::string& condition,
                     bool consumable, const std::string& supplier)
    : inventoryId(id), property(prop), itemName(itemName), category(category),
      quantity(quantity), minimumStock(minimumStock), unitCost(unitCost),
      lastRestocked(lastRestocked), storageLocation(storageLocation),
      condition(condition), isConsumable(consumable), supplier(supplier) {}

Inventory::Inventory(const Inventory& other)
    : inventoryId(other.inventoryId), property(other.property),
      itemName(other.itemName), category(other.category),
      quantity(other.quantity), minimumStock(other.minimumStock),
      unitCost(other.unitCost), lastRestocked(other.lastRestocked),
      storageLocation(other.storageLocation), condition(other.condition),
      isConsumable(other.isConsumable), supplier(other.supplier) {}

Inventory& Inventory::operator=(const Inventory& other) {
    if (this != &other) {
        inventoryId = other.inventoryId;
        property = other.property;
        itemName = other.itemName;
        category = other.category;
        quantity = other.quantity;
        minimumStock = other.minimumStock;
        unitCost = other.unitCost;
        lastRestocked = other.lastRestocked;
        storageLocation = other.storageLocation;
        condition = other.condition;
        isConsumable = other.isConsumable;
        supplier = other.supplier;
    }
    return *this;
}

Inventory::Inventory(Inventory&& other) noexcept
    : inventoryId(std::move(other.inventoryId)), property(std::move(other.property)),
      itemName(std::move(other.itemName)), category(std::move(other.category)),
      quantity(other.quantity), minimumStock(other.minimumStock),
      unitCost(other.unitCost), lastRestocked(other.lastRestocked),
      storageLocation(std::move(other.storageLocation)), condition(std::move(other.condition)),
      isConsumable(other.isConsumable), supplier(std::move(other.supplier)) {}

Inventory& Inventory::operator=(Inventory&& other) noexcept {
    if (this != &other) {
        inventoryId = std::move(other.inventoryId);
        property = std::move(other.property);
        itemName = std::move(other.itemName);
        category = std::move(other.category);
        quantity = other.quantity;
        minimumStock = other.minimumStock;
        unitCost = other.unitCost;
        lastRestocked = other.lastRestocked;
        storageLocation = std::move(other.storageLocation);
        condition = std::move(other.condition);
        isConsumable = other.isConsumable;
        supplier = std::move(other.supplier);
    }
    return *this;
}

Inventory::~Inventory() {}

std::string Inventory::getInventoryId() const { return inventoryId; }
Property* Inventory::getProperty() const { return property.get(); }
std::string Inventory::getItemName() const { return itemName; }
std::string Inventory::getCategory() const { return category; }
int Inventory::getQuantity() const { return quantity; }
int Inventory::getMinimumStock() const { return minimumStock; }
double Inventory::getUnitCost() const { return unitCost; }
std::chrono::system_clock::time_point Inventory::getLastRestocked() const { return lastRestocked; }
std::string Inventory::getStorageLocation() const { return storageLocation; }
std::string Inventory::getCondition() const { return condition; }
bool Inventory::getIsConsumable() const { return isConsumable; }
std::string Inventory::getSupplier() const { return supplier; }

void Inventory::setQuantity(int newQuantity) {
    quantity = std::max(0, newQuantity);
}

void Inventory::setUnitCost(double newCost) {
    unitCost = std::max(0.0, newCost);
}

void Inventory::updateLastRestocked() {
    lastRestocked = std::chrono::system_clock::now();
}

bool Inventory::needsRestocking() const {
    return quantity <= minimumStock;
}

double Inventory::calculateTotalValue() const {
    return quantity * unitCost;
}

bool Inventory::isLowStock() const {
    return quantity <= (minimumStock * 1.5);
}

int Inventory::calculateMonthsSinceLastRestock(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - lastRestocked);
    return static_cast<int>(duration.count() / (24 * 30));
}

bool Inventory::requiresMaintenance() const {
    return condition == "poor" || condition == "needs_repair" || 
           calculateMonthsSinceLastRestock(std::chrono::system_clock::now()) > 24;
}

double Inventory::calculateDepreciation() const {
    if (!isConsumable) {
        int months = calculateMonthsSinceLastRestock(std::chrono::system_clock::now());
        double depreciationRate = 0.02;
        return calculateTotalValue() * depreciationRate * (months / 12.0);
    }
    return 0.0;
}

bool Inventory::isCriticalItem() const {
    return category == "safety" || category == "security" || 
           category == "electrical" || category == "plumbing";
}

double Inventory::calculateReplacementCost() const {
    return (minimumStock - quantity) * unitCost;
}

std::string Inventory::getInventoryStatus() const {
    if (quantity == 0) return "out_of_stock";
    if (needsRestocking()) return "low_stock";
    if (isLowStock()) return "adequate";
    return "well_stocked";
}