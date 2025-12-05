#ifndef APARTMENT_H
#define APARTMENT_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class Apartment : public Property {
private:
    int floorNumber;
    int numberOfBedrooms;
    int numberOfBathrooms;
    bool hasBalcony;
    bool hasParking;
    std::string apartmentNumber;
    double maintenanceFee;
    std::string buildingName;

public:
    Apartment(const std::string& propId, double propArea, double price,
              std::unique_ptr<Address> propAddress,
              std::unique_ptr<PropertyStatus> propStatus,
              std::unique_ptr<PropertyType> propType,
              int floor, int bedrooms, int bathrooms,
              bool balcony, bool parking, const std::string& aptNumber,
              double maintenance, const std::string& building);
    
    Apartment(const Apartment& other);
    Apartment& operator=(const Apartment& other);
    Apartment(Apartment&& other) noexcept;
    Apartment& operator=(Apartment&& other) noexcept;
    ~Apartment();

    int getFloorNumber() const;
    int getNumberOfBedrooms() const;
    int getNumberOfBathrooms() const;
    bool getHasBalcony() const;
    bool getHasParking() const;
    std::string getApartmentNumber() const;
    double getMaintenanceFee() const;
    std::string getBuildingName() const;

    void setMaintenanceFee(double newFee);
    void setApartmentNumber(const std::string& newNumber);

    double calculateTotalMonthlyCost() const;
    bool isOnUpperFloor(int minFloor) const;
    bool meetsBedroomRequirements(int minBedrooms, int maxBedrooms) const;
    double calculatePricePerBedroom() const;
    bool hasPremiumFeatures() const;
    bool isSuitableForFamilies() const;
    double calculateSpaceEfficiency() const;
};

#endif