#include "../../include/business/PropertyManager.h"
#include <algorithm>

PropertyManager::PropertyManager(double vacancyThreshold, double maintenanceBudget)
    : vacancyRateThreshold(vacancyThreshold), maintenanceBudgetPercentage(maintenanceBudget) {}

PropertyManager::PropertyManager(const PropertyManager& other)
    : properties(other.properties), propertyContracts(other.propertyContracts),
      propertyMaintenance(other.propertyMaintenance),
      vacancyRateThreshold(other.vacancyRateThreshold),
      maintenanceBudgetPercentage(other.maintenanceBudgetPercentage) {}

PropertyManager& PropertyManager::operator=(const PropertyManager& other) {
    if (this != &other) {
        properties = other.properties;
        propertyContracts = other.propertyContracts;
        propertyMaintenance = other.propertyMaintenance;
        vacancyRateThreshold = other.vacancyRateThreshold;
        maintenanceBudgetPercentage = other.maintenanceBudgetPercentage;
    }
    return *this;
}

PropertyManager::PropertyManager(PropertyManager&& other) noexcept
    : properties(std::move(other.properties)), propertyContracts(std::move(other.propertyContracts)),
      propertyMaintenance(std::move(other.propertyMaintenance)),
      vacancyRateThreshold(other.vacancyRateThreshold),
      maintenanceBudgetPercentage(other.maintenanceBudgetPercentage) {}

PropertyManager& PropertyManager::operator=(PropertyManager&& other) noexcept {
    if (this != &other) {
        properties = std::move(other.properties);
        propertyContracts = std::move(other.propertyContracts);
        propertyMaintenance = std::move(other.propertyMaintenance);
        vacancyRateThreshold = other.vacancyRateThreshold;
        maintenanceBudgetPercentage = other.maintenanceBudgetPercentage;
    }
    return *this;
}

PropertyManager::~PropertyManager() {}

double PropertyManager::getVacancyRateThreshold() const { return vacancyRateThreshold; }
double PropertyManager::getMaintenanceBudgetPercentage() const { return maintenanceBudgetPercentage; }

void PropertyManager::setVacancyRateThreshold(double newThreshold) {
    vacancyRateThreshold = std::max(0.0, std::min(100.0, newThreshold));
}

void PropertyManager::setMaintenanceBudgetPercentage(double newPercentage) {
    maintenanceBudgetPercentage = std::max(0.0, newPercentage);
}

bool PropertyManager::addProperty(std::shared_ptr<Property> property) {
    if (!property) return false;
    
    properties.push_back(property);
    return true;
}

bool PropertyManager::removeProperty(const std::string& propertyId) {
    auto it = std::find_if(properties.begin(), properties.end(),
        [&](const std::shared_ptr<Property>& prop) { return prop->getId() == propertyId; });
    
    if (it != properties.end()) {
        properties.erase(it);
        propertyContracts.erase(propertyId);
        propertyMaintenance.erase(propertyId);
        return true;
    }
    return false;
}

double PropertyManager::calculatePropertyVacancyRate(const std::string& propertyId) const {
    auto contractIt = propertyContracts.find(propertyId);
    if (contractIt == propertyContracts.end()) return 100.0;
    
    auto contract = contractIt->second;
    if (contract && contract->isContractValid()) return 0.0;
    
    return 100.0;
}

double PropertyManager::calculateMaintenanceBudget(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    return property->getRentalPrice() * 12 * (maintenanceBudgetPercentage / 100.0);
}

std::vector<std::shared_ptr<Property>> PropertyManager::getVacantProperties() const {
    std::vector<std::shared_ptr<Property>> vacant;
    
    for (const auto& property : properties) {
        if (calculatePropertyVacancyRate(property->getId()) == 100.0) {
            vacant.push_back(property);
        }
    }
    
    return vacant;
}

std::vector<std::shared_ptr<Property>> PropertyManager::getPropertiesNeedingMaintenance() const {
    std::vector<std::shared_ptr<Property>> needsMaintenance;
    
    for (const auto& entry : propertyMaintenance) {
        for (const auto& request : entry.second) {
            if (request->requiresImmediateAttention() && request->getStatus() != "completed") {
                auto property = request->getProperty();
                if (property) {
                    needsMaintenance.push_back(property);
                    break;
                }
            }
        }
    }
    
    return needsMaintenance;
}

double PropertyManager::calculateTotalPortfolioValue() const {
    double totalValue = 0.0;
    for (const auto& property : properties) {
        totalValue += property->getRentalPrice() * 100;
    }
    return totalValue;
}

double PropertyManager::calculateAverageRent() const {
    if (properties.empty()) return 0.0;
    
    double totalRent = 0.0;
    int count = 0;
    
    for (const auto& property : properties) {
        totalRent += property->getRentalPrice();
        count++;
    }
    
    return totalRent / count;
}

std::vector<std::shared_ptr<Property>> PropertyManager::getUnderperformingProperties() const {
    std::vector<std::shared_ptr<Property>> underperforming;
    
    for (const auto& property : properties) {
        double vacancyRate = calculatePropertyVacancyRate(property->getId());
        if (vacancyRate > vacancyRateThreshold) {
            underperforming.push_back(property);
        }
    }
    
    return underperforming;
}

bool PropertyManager::shouldConsiderSale(std::shared_ptr<Property> property) const {
    if (!property) return false;
    
    double vacancyRate = calculatePropertyVacancyRate(property->getId());
    double maintenanceCost = calculateMaintenanceBudget(property);
    double annualRent = property->getRentalPrice() * 12;
    
    return vacancyRate > 50.0 || maintenanceCost > annualRent * 0.3;
}

double PropertyManager::calculateROI(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    
    double annualRent = property->getRentalPrice() * 12;
    double propertyValue = property->getRentalPrice() * 100;
    double maintenanceCost = calculateMaintenanceBudget(property);
    
    if (propertyValue <= 0) return 0.0;
    return ((annualRent - maintenanceCost) / propertyValue) * 100.0;
}