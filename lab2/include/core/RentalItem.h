#ifndef RENTALITEM_H
#define RENTALITEM_H

#include <string>
#include <memory>
#include "../core/RentalPricing.h"

enum class ItemCondition {
    NEW,
    EXCELLENT,
    GOOD,
    FAIR,
    POOR,
    MAINTENANCE_REQUIRED
};

class RentalItem {
protected:
    std::string itemId;
    std::string name;
    std::string category;
    double basePricePerDay;
    ItemCondition condition;
    int totalRentalDays;
    int maintenanceIntervalDays;
    int daysSinceLastMaintenance;
    bool isAvailable;

public:
    RentalItem(const std::string& id, const std::string& itemName, 
               const std::string& itemCategory, double basePrice);
    virtual ~RentalItem() = default;
    
    virtual double calculateRentalPrice(int days, const std::string& customerType) const;
    virtual bool requiresMaintenance() const;
    virtual double calculateDepreciation() const;
    virtual double calculateInsurancePremium() const;
    virtual int getRemainingUsefulLife() const;
    virtual bool validateRentalDuration(int days) const;
    
    std::string getItemId() const { return itemId; }
    std::string getName() const { return name; }
    std::string getCategory() const { return category; }
    bool getIsAvailable() const { return isAvailable; }
    ItemCondition getCondition() const { return condition; } 
    
    void setCondition(ItemCondition newCondition) { condition = newCondition; }
    void markAsRented() { isAvailable = false; }
    void markAsReturned() { isAvailable = true; }
    void updateMaintenanceStatus(int daysRented);
};

#endif