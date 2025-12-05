#include "../../include/items/Vehicle.h"
#include <cmath>
#include <algorithm>

Vehicle::Vehicle(const std::string& id, const std::string& name, 
                 const std::string& license, const std::string& type, int vehicleYear)
    : RentalItem(id, name, "VEHICLE", 75.0), 
      licensePlate(license), vehicleType(type), year(vehicleYear),
      mileage(0.0), fuelType("PETROL"), fuelEfficiency(8.5), 
      passengerCapacity(5), transmissionType("MANUAL") {}

double Vehicle::calculateFuelCostForTrip(double distance) const {
    double fuelPricePerLiter = 1.75; 
    
    double adjustedEfficiency = fuelEfficiency;
    if (fuelType == "DIESEL") {
        adjustedEfficiency *= 0.8; 
        fuelPricePerLiter = 1.65;
    } else if (fuelType == "ELECTRIC") {
        adjustedEfficiency = 20.0; 
        fuelPricePerLiter = 0.15; 
    } else if (fuelType == "HYBRID") {
        adjustedEfficiency *= 0.6;
    }
    
    double fuelConsumed = (distance / 100.0) * adjustedEfficiency;
    return fuelConsumed * fuelPricePerLiter;
}

bool Vehicle::requiresEmissionCheck() const {
    int currentYear = 2024; 
    int vehicleAge = currentYear - year;
    
    if (vehicleAge >= 3) {
        if (fuelType == "DIESEL" && vehicleAge >= 2) {
            return true;
        }
        return true;
    }
    
    return false;
}

double Vehicle::calculateDepreciation() const {
    double baseDepreciation = RentalItem::calculateDepreciation();
    
    double mileageDepreciation = (mileage / 100000.0) * basePricePerDay * 0.5;
    
    int currentYear = 2024; 
    int vehicleAge = currentYear - year;
    double ageDepreciation = vehicleAge * basePricePerDay * 0.1;
    
    return baseDepreciation + mileageDepreciation + ageDepreciation;
}

bool Vehicle::validateDriverAge(int driverAge) const {
    int minAge = 18;
    
    if (vehicleType == "MOTORCYCLE") {
        minAge = 16; 
    } else if (vehicleType == "LUXURY" || vehicleType == "SPORTS") {
        minAge = 25; 
    } else if (vehicleType == "COMMERCIAL") {
        minAge = 21; 
    }
    
    if (driverAge < 25 && (vehicleType == "SUV" || vehicleType == "VAN")) {
        return false; 
    }
    
    return driverAge >= minAge;
}

double Vehicle::calculateInsurancePremium() const {
    double basePremium = RentalItem::calculateInsurancePremium();
    
    double riskMultiplier = 1.0;
    
    int currentYear = 2024;
    int vehicleAge = currentYear - year;
    if (vehicleAge > 10) riskMultiplier *= 1.5;
    else if (vehicleAge > 5) riskMultiplier *= 1.2;
    
    if (vehicleType == "SPORTS") riskMultiplier *= 2.0;
    else if (vehicleType == "MOTORCYCLE") riskMultiplier *= 1.8;
    else if (vehicleType == "LUXURY") riskMultiplier *= 1.6;
    
    if (passengerCapacity > 7) riskMultiplier *= 1.3;
    
    return basePremium * riskMultiplier;
}

int Vehicle::getRecommendedServiceInterval() const {
    int baseInterval = 15000; 
    
    
    if (transmissionType == "AUTOMATIC") {
        baseInterval = 12000; 
    }
    
    if (fuelType == "DIESEL") {
        baseInterval = 10000; 
    } else if (fuelType == "ELECTRIC") {
        baseInterval = 30000; 
    }
    
    if (mileage > 100000) {
        baseInterval = static_cast<int>(baseInterval * 0.8); 
    }
    
    return baseInterval;
}