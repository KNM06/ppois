#ifndef COMMERCIALSPACE_H
#define COMMERCIALSPACE_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"
#include "../../include/core/Address.h"
#include "../../include/core/PropertyStatus.h"
#include "../../include/core/PropertyType.h"

class CommercialSpace : public Property {
private:
    std::string businessType;
    bool hasStorefront;
    int parkingSpaces;
    double commonAreaMaintenance;
    bool hasLoadingDock;
    std::string zoningType;
    int maxOccupancy;
    double annualTax;
    bool isGroundFloor;

public:
    CommercialSpace(const std::string& propId, double propArea, double price,
                    std::unique_ptr<Address> propAddress,
                    std::unique_ptr<PropertyStatus> propStatus,
                    std::unique_ptr<PropertyType> propType,
                    const std::string& businessType, bool storefront,
                    int parking, double maintenance, bool loadingDock,
                    const std::string& zoning, int occupancy, double tax,
                    bool groundFloor);
    
    CommercialSpace(const CommercialSpace& other);
    CommercialSpace& operator=(const CommercialSpace& other);
    CommercialSpace(CommercialSpace&& other) noexcept;
    CommercialSpace& operator=(CommercialSpace&& other) noexcept;
    ~CommercialSpace();

    std::string getBusinessType() const;
    bool getHasStorefront() const;
    int getParkingSpaces() const;
    double getCommonAreaMaintenance() const;
    bool getHasLoadingDock() const;
    std::string getZoningType() const;
    int getMaxOccupancy() const;
    double getAnnualTax() const;
    bool getIsGroundFloor() const;

    void setBusinessType(const std::string& newType);
    void setAnnualTax(double newTax);

    double calculateTotalOccupancyCost() const;
    bool isSuitableForRetail() const;
    bool isSuitableForOffice() const;
    bool meetsZoningRequirements(const std::string& intendedUse) const;
    double calculateParkingRatio() const;
    bool hasAdequateParking(int expectedCustomers) const;
    double calculateTaxDeduction() const;
    bool isPremiumLocation() const;
};

#endif