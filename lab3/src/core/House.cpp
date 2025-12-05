#include "../../include/core/House.h"
#include <algorithm>
#include <cmath>

House::House(const std::string& propId, double propArea, double price,
             std::unique_ptr<Address> propAddress,
             std::unique_ptr<PropertyStatus> propStatus,
             std::unique_ptr<PropertyType> propType,
             int floors, double land, bool garden, bool garage,
             int garageCap, int year, const std::string& style,
             double tax, bool pool)
    : Property(propId, propArea, price, std::move(propAddress),
               std::move(propStatus), std::move(propType)),
      numberOfFloors(floors), landArea(land), hasGarden(garden),
      hasGarage(garage), garageCapacity(garageCap), yearBuilt(year),
      architecturalStyle(style), propertyTax(tax), hasSwimmingPool(pool) {}

House::House(const House& other)
    : Property(other), numberOfFloors(other.numberOfFloors),
      landArea(other.landArea), hasGarden(other.hasGarden),
      hasGarage(other.hasGarage), garageCapacity(other.garageCapacity),
      yearBuilt(other.yearBuilt), architecturalStyle(other.architecturalStyle),
      propertyTax(other.propertyTax), hasSwimmingPool(other.hasSwimmingPool) {}

House& House::operator=(const House& other) {
    if (this != &other) {
        Property::operator=(other);
        numberOfFloors = other.numberOfFloors;
        landArea = other.landArea;
        hasGarden = other.hasGarden;
        hasGarage = other.hasGarage;
        garageCapacity = other.garageCapacity;
        yearBuilt = other.yearBuilt;
        architecturalStyle = other.architecturalStyle;
        propertyTax = other.propertyTax;
        hasSwimmingPool = other.hasSwimmingPool;
    }
    return *this;
}

House::House(House&& other) noexcept
    : Property(std::move(other)), numberOfFloors(other.numberOfFloors),
      landArea(other.landArea), hasGarden(other.hasGarden),
      hasGarage(other.hasGarage), garageCapacity(other.garageCapacity),
      yearBuilt(other.yearBuilt), architecturalStyle(std::move(other.architecturalStyle)),
      propertyTax(other.propertyTax), hasSwimmingPool(other.hasSwimmingPool) {}

House& House::operator=(House&& other) noexcept {
    if (this != &other) {
        Property::operator=(std::move(other));
        numberOfFloors = other.numberOfFloors;
        landArea = other.landArea;
        hasGarden = other.hasGarden;
        hasGarage = other.hasGarage;
        garageCapacity = other.garageCapacity;
        yearBuilt = other.yearBuilt;
        architecturalStyle = std::move(other.architecturalStyle);
        propertyTax = other.propertyTax;
        hasSwimmingPool = other.hasSwimmingPool;
    }
    return *this;
}

House::~House() {}

int House::getNumberOfFloors() const { return numberOfFloors; }
double House::getLandArea() const { return landArea; }
bool House::getHasGarden() const { return hasGarden; }
bool House::getHasGarage() const { return hasGarage; }
int House::getGarageCapacity() const { return garageCapacity; }
int House::getYearBuilt() const { return yearBuilt; }
std::string House::getArchitecturalStyle() const { return architecturalStyle; }
double House::getPropertyTax() const { return propertyTax; }
bool House::getHasSwimmingPool() const { return hasSwimmingPool; }

void House::setPropertyTax(double newTax) {
    propertyTax = std::max(0.0, newTax);
}

void House::updateGarageCapacity(int newCapacity) {
    garageCapacity = std::max(0, newCapacity);
}

double House::calculateBuildingToLandRatio() const {
    if (landArea <= 0) return 0.0;
    return (getArea() / landArea) * 100.0;
}

bool House::isNewConstruction(int currentYear) const {
    return (currentYear - yearBuilt) <= 5;
}

double House::calculateAnnualPropertyTax() const {
    return propertyTax * 12.0;
}

bool House::hasMultipleParkingSpaces() const {
    return garageCapacity >= 2;
}

double House::calculateLandValuePerSqMeter(double totalLandValue) const {
    if (landArea <= 0) return 0.0;
    return totalLandValue / landArea;
}

bool House::requiresMajorRenovation(int currentYear) const {
    int houseAge = currentYear - yearBuilt;
    return houseAge > 25 && !hasSwimmingPool && !hasGarage;
}

double House::calculatePremiumFeaturesValue() const {
    double premiumValue = 0.0;
    if (hasSwimmingPool) premiumValue += getRentalPrice() * 0.15;
    if (hasGarage) premiumValue += getRentalPrice() * 0.10;
    if (hasGarden) premiumValue += getRentalPrice() * 0.08;
    if (numberOfFloors > 1) premiumValue += getRentalPrice() * 0.05;
    return premiumValue;
}