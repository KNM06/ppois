#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../include/core/Property.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/maintenance/MaintenanceRequest.h"

class PropertyManager {
private:
    std::vector<std::shared_ptr<Property>> properties;
    std::unordered_map<std::string, std::shared_ptr<RentalContract>> propertyContracts;
    std::unordered_map<std::string, std::vector<std::shared_ptr<MaintenanceRequest>>> propertyMaintenance;
    double vacancyRateThreshold;
    double maintenanceBudgetPercentage;

public:
    PropertyManager(double vacancyThreshold, double maintenanceBudget);
    
    PropertyManager(const PropertyManager& other);
    PropertyManager& operator=(const PropertyManager& other);
    PropertyManager(PropertyManager&& other) noexcept;
    PropertyManager& operator=(PropertyManager&& other) noexcept;
    ~PropertyManager();

    double getVacancyRateThreshold() const;
    double getMaintenanceBudgetPercentage() const;

    void setVacancyRateThreshold(double newThreshold);
    void setMaintenanceBudgetPercentage(double newPercentage);

    bool addProperty(std::shared_ptr<Property> property);
    bool removeProperty(const std::string& propertyId);
    double calculatePropertyVacancyRate(const std::string& propertyId) const;
    double calculateMaintenanceBudget(std::shared_ptr<Property> property) const;
    std::vector<std::shared_ptr<Property>> getVacantProperties() const;
    std::vector<std::shared_ptr<Property>> getPropertiesNeedingMaintenance() const;
    double calculateTotalPortfolioValue() const;
    double calculateAverageRent() const;
    std::vector<std::shared_ptr<Property>> getUnderperformingProperties() const;
    bool shouldConsiderSale(std::shared_ptr<Property> property) const;
    double calculateROI(std::shared_ptr<Property> property) const;
    friend class PropertyManagerTest;
};

#endif