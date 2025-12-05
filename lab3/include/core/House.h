#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class House : public Property {
private:
    int numberOfFloors;
    double landArea;
    bool hasGarden;
    bool hasGarage;
    int garageCapacity;
    int yearBuilt;
    std::string architecturalStyle;
    double propertyTax;
    bool hasSwimmingPool;

public:
    House(const std::string& propId, double propArea, double price,
          std::unique_ptr<Address> propAddress,
          std::unique_ptr<PropertyStatus> propStatus,
          std::unique_ptr<PropertyType> propType,
          int floors, double land, bool garden, bool garage,
          int garageCapacity, int year, const std::string& style,
          double tax, bool pool);
    
    House(const House& other);
    House& operator=(const House& other);
    House(House&& other) noexcept;
    House& operator=(House&& other) noexcept;
    ~House();

    int getNumberOfFloors() const;
    double getLandArea() const;
    bool getHasGarden() const;
    bool getHasGarage() const;
    int getGarageCapacity() const;
    int getYearBuilt() const;
    std::string getArchitecturalStyle() const;
    double getPropertyTax() const;
    bool getHasSwimmingPool() const;

    void setPropertyTax(double newTax);
    void updateGarageCapacity(int newCapacity);

    double calculateBuildingToLandRatio() const;
    bool isNewConstruction(int currentYear) const;
    double calculateAnnualPropertyTax() const;
    bool hasMultipleParkingSpaces() const;
    double calculateLandValuePerSqMeter(double totalLandValue) const;
    bool requiresMajorRenovation(int currentYear) const;
    double calculatePremiumFeaturesValue() const;
};

#endif