#include "../../include/core/Apartment.h"
#include <algorithm>

Apartment::Apartment(const std::string& propId, double propArea, double price,
                     std::unique_ptr<Address> propAddress,
                     std::unique_ptr<PropertyStatus> propStatus,
                     std::unique_ptr<PropertyType> propType,
                     int floor, int bedrooms, int bathrooms,
                     bool balcony, bool parking, const std::string& aptNumber,
                     double maintenance, const std::string& building)
    : Property(propId, propArea, price, std::move(propAddress),
               std::move(propStatus), std::move(propType)),
      floorNumber(floor), numberOfBedrooms(bedrooms), numberOfBathrooms(bathrooms),
      hasBalcony(balcony), hasParking(parking), apartmentNumber(aptNumber),
      maintenanceFee(maintenance), buildingName(building) {}

Apartment::Apartment(const Apartment& other)
    : Property(other), floorNumber(other.floorNumber),
      numberOfBedrooms(other.numberOfBedrooms), numberOfBathrooms(other.numberOfBathrooms),
      hasBalcony(other.hasBalcony), hasParking(other.hasParking),
      apartmentNumber(other.apartmentNumber), maintenanceFee(other.maintenanceFee),
      buildingName(other.buildingName) {}

Apartment& Apartment::operator=(const Apartment& other) {
    if (this != &other) {
        Property::operator=(other);
        floorNumber = other.floorNumber;
        numberOfBedrooms = other.numberOfBedrooms;
        numberOfBathrooms = other.numberOfBathrooms;
        hasBalcony = other.hasBalcony;
        hasParking = other.hasParking;
        apartmentNumber = other.apartmentNumber;
        maintenanceFee = other.maintenanceFee;
        buildingName = other.buildingName;
    }
    return *this;
}

Apartment::Apartment(Apartment&& other) noexcept
    : Property(std::move(other)), floorNumber(other.floorNumber),
      numberOfBedrooms(other.numberOfBedrooms), numberOfBathrooms(other.numberOfBathrooms),
      hasBalcony(other.hasBalcony), hasParking(other.hasParking),
      apartmentNumber(std::move(other.apartmentNumber)), maintenanceFee(other.maintenanceFee),
      buildingName(std::move(other.buildingName)) {}

Apartment& Apartment::operator=(Apartment&& other) noexcept {
    if (this != &other) {
        Property::operator=(std::move(other));
        floorNumber = other.floorNumber;
        numberOfBedrooms = other.numberOfBedrooms;
        numberOfBathrooms = other.numberOfBathrooms;
        hasBalcony = other.hasBalcony;
        hasParking = other.hasParking;
        apartmentNumber = std::move(other.apartmentNumber);
        maintenanceFee = other.maintenanceFee;
        buildingName = std::move(other.buildingName);
    }
    return *this;
}

Apartment::~Apartment() {}

int Apartment::getFloorNumber() const { return floorNumber; }
int Apartment::getNumberOfBedrooms() const { return numberOfBedrooms; }
int Apartment::getNumberOfBathrooms() const { return numberOfBathrooms; }
bool Apartment::getHasBalcony() const { return hasBalcony; }
bool Apartment::getHasParking() const { return hasParking; }
std::string Apartment::getApartmentNumber() const { return apartmentNumber; }
double Apartment::getMaintenanceFee() const { return maintenanceFee; }
std::string Apartment::getBuildingName() const { return buildingName; }

void Apartment::setMaintenanceFee(double newFee) {
    maintenanceFee = std::max(0.0, newFee);
}

void Apartment::setApartmentNumber(const std::string& newNumber) {
    apartmentNumber = newNumber;
}

double Apartment::calculateTotalMonthlyCost() const {
    return getRentalPrice() + maintenanceFee;
}

bool Apartment::isOnUpperFloor(int minFloor) const {
    return floorNumber >= minFloor;
}

bool Apartment::meetsBedroomRequirements(int minBedrooms, int maxBedrooms) const {
    return numberOfBedrooms >= minBedrooms && numberOfBedrooms <= maxBedrooms;
}

double Apartment::calculatePricePerBedroom() const {
    if (numberOfBedrooms <= 0) return 0.0;
    return getRentalPrice() / numberOfBedrooms;
}

bool Apartment::hasPremiumFeatures() const {
    return hasBalcony && hasParking && numberOfBathrooms >= 2;
}

bool Apartment::isSuitableForFamilies() const {
    return numberOfBedrooms >= 2 && numberOfBathrooms >= 1 && hasParking;
}

double Apartment::calculateSpaceEfficiency() const {
    double totalArea = getArea();
    if (totalArea <= 0) return 0.0;
    
    double livingArea = totalArea - (numberOfBedrooms * 12.0) - (numberOfBathrooms * 6.0);
    return (livingArea / totalArea) * 100.0;
}