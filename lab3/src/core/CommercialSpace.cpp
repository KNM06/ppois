#include "../../include/core/CommercialSpace.h"
#include <algorithm>

CommercialSpace::CommercialSpace(const std::string& propId, double propArea, double price,
                                 std::unique_ptr<Address> propAddress,
                                 std::unique_ptr<PropertyStatus> propStatus,
                                 std::unique_ptr<PropertyType> propType,
                                 const std::string& businessType, bool storefront,
                                 int parking, double maintenance, bool loadingDock,
                                 const std::string& zoning, int occupancy, double tax,
                                 bool groundFloor)
    : Property(propId, propArea, price, std::move(propAddress),
               std::move(propStatus), std::move(propType)),
      businessType(businessType), hasStorefront(storefront),
      parkingSpaces(parking), commonAreaMaintenance(maintenance),
      hasLoadingDock(loadingDock), zoningType(zoning),
      maxOccupancy(occupancy), annualTax(tax), isGroundFloor(groundFloor) {}

CommercialSpace::CommercialSpace(const CommercialSpace& other)
    : Property(other), businessType(other.businessType),
      hasStorefront(other.hasStorefront), parkingSpaces(other.parkingSpaces),
      commonAreaMaintenance(other.commonAreaMaintenance),
      hasLoadingDock(other.hasLoadingDock), zoningType(other.zoningType),
      maxOccupancy(other.maxOccupancy), annualTax(other.annualTax),
      isGroundFloor(other.isGroundFloor) {}

CommercialSpace& CommercialSpace::operator=(const CommercialSpace& other) {
    if (this != &other) {
        Property::operator=(other);
        businessType = other.businessType;
        hasStorefront = other.hasStorefront;
        parkingSpaces = other.parkingSpaces;
        commonAreaMaintenance = other.commonAreaMaintenance;
        hasLoadingDock = other.hasLoadingDock;
        zoningType = other.zoningType;
        maxOccupancy = other.maxOccupancy;
        annualTax = other.annualTax;
        isGroundFloor = other.isGroundFloor;
    }
    return *this;
}

CommercialSpace::CommercialSpace(CommercialSpace&& other) noexcept
    : Property(std::move(other)), businessType(std::move(other.businessType)),
      hasStorefront(other.hasStorefront), parkingSpaces(other.parkingSpaces),
      commonAreaMaintenance(other.commonAreaMaintenance),
      hasLoadingDock(other.hasLoadingDock), zoningType(std::move(other.zoningType)),
      maxOccupancy(other.maxOccupancy), annualTax(other.annualTax),
      isGroundFloor(other.isGroundFloor) {}

CommercialSpace& CommercialSpace::operator=(CommercialSpace&& other) noexcept {
    if (this != &other) {
        Property::operator=(std::move(other));
        businessType = std::move(other.businessType);
        hasStorefront = other.hasStorefront;
        parkingSpaces = other.parkingSpaces;
        commonAreaMaintenance = other.commonAreaMaintenance;
        hasLoadingDock = other.hasLoadingDock;
        zoningType = std::move(other.zoningType);
        maxOccupancy = other.maxOccupancy;
        annualTax = other.annualTax;
        isGroundFloor = other.isGroundFloor;
    }
    return *this;
}

CommercialSpace::~CommercialSpace() {}

std::string CommercialSpace::getBusinessType() const { return businessType; }
bool CommercialSpace::getHasStorefront() const { return hasStorefront; }
int CommercialSpace::getParkingSpaces() const { return parkingSpaces; }
double CommercialSpace::getCommonAreaMaintenance() const { return commonAreaMaintenance; }
bool CommercialSpace::getHasLoadingDock() const { return hasLoadingDock; }
std::string CommercialSpace::getZoningType() const { return zoningType; }
int CommercialSpace::getMaxOccupancy() const { return maxOccupancy; }
double CommercialSpace::getAnnualTax() const { return annualTax; }
bool CommercialSpace::getIsGroundFloor() const { return isGroundFloor; }

void CommercialSpace::setBusinessType(const std::string& newType) {
    businessType = newType;
}

void CommercialSpace::setAnnualTax(double newTax) {
    annualTax = std::max(0.0, newTax);
}

double CommercialSpace::calculateTotalOccupancyCost() const {
    return getRentalPrice() + commonAreaMaintenance + (annualTax / 12.0);
}

bool CommercialSpace::isSuitableForRetail() const {
    return hasStorefront && isGroundFloor && zoningType.find("commercial") != std::string::npos;
}

bool CommercialSpace::isSuitableForOffice() const {
    return !hasStorefront && maxOccupancy >= 10 && parkingSpaces >= 5;
}

bool CommercialSpace::meetsZoningRequirements(const std::string& intendedUse) const {
    if (intendedUse == "retail") {
        return zoningType.find("commercial") != std::string::npos || 
               zoningType.find("retail") != std::string::npos;
    } else if (intendedUse == "office") {
        return zoningType.find("commercial") != std::string::npos || 
               zoningType.find("office") != std::string::npos;
    } else if (intendedUse == "industrial") {
        return zoningType.find("industrial") != std::string::npos;
    }
    return false;
}

double CommercialSpace::calculateParkingRatio() const {
    double area = getArea();
    if (area <= 0) return 0.0;
    return static_cast<double>(parkingSpaces) / (area / 100.0);
}

bool CommercialSpace::hasAdequateParking(int expectedCustomers) const {
    int requiredSpaces = expectedCustomers / 5;
    return parkingSpaces >= requiredSpaces;
}

double CommercialSpace::calculateTaxDeduction() const {
    if (businessType == "retail" || businessType == "restaurant") {
        return annualTax * 0.15;
    } else if (businessType == "office" || businessType == "medical") {
        return annualTax * 0.10;
    }
    return 0.0;
}

bool CommercialSpace::isPremiumLocation() const {
    return isGroundFloor && hasStorefront && hasLoadingDock && parkingSpaces >= 10;
}