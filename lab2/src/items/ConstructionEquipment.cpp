#include "../../include/items/ConstructionEquipment.h"
#include <cmath>

ConstructionEquipment::ConstructionEquipment(const std::string& id, const std::string& name, 
                                             const std::string& eqClass, double eqWeight)
    : RentalItem(id, name, "CONSTRUCTION", 120.0), 
      equipmentClass(eqClass), weight(eqWeight), powerOutput(0.0), fuelType("DIESEL"),
      requiresSpecialLicense(false), hourlyUsageRate(15.0), maxOperatingHours(2000) {
    
    if (equipmentClass == "HEAVY" || equipmentClass == "CRANE") {
        requiresSpecialLicense = true;
        basePricePerDay = 200.0; 
    }
}

double ConstructionEquipment::calculateTransportationCost(double distance) const {
    double baseTransportCost = 50.0; 
    double weightMultiplier = 1.0;
    if (weight > 5000) weightMultiplier = 3.0; 
    else if (weight > 2000) weightMultiplier = 2.0;
    else if (weight > 1000) weightMultiplier = 1.5;
    
    double distanceCost = distance * 0.8; 
    
    double specialTransportFee = 0.0;
    if (equipmentClass == "CRANE" || weight > 3000) {
        specialTransportFee = 200.0;
    }
    
    return baseTransportCost + (distanceCost * weightMultiplier) + specialTransportFee;
}

bool ConstructionEquipment::validateOperatorLicense(const std::string& licenseType) const {
    if (!requiresSpecialLicense) {
        return true; 
    }
    
    if (equipmentClass == "HEAVY" && licenseType == "HEAVY_EQUIPMENT") {
        return true;
    } else if (equipmentClass == "CRANE" && licenseType == "CRANE_OPERATOR") {
        return true;
    } else if (equipmentClass == "EXCAVATOR" && licenseType == "EXCAVATOR_OPERATOR") {
        return true;
    }
    
    return false;
}

double ConstructionEquipment::calculateFuelConsumption(int hours) const {
    double baseConsumptionRate = 5.0; 
    
    double powerMultiplier = 1.0 + (powerOutput / 100.0);
    
    double fuelMultiplier = 1.0;
    if (fuelType == "DIESEL") {
        fuelMultiplier = 0.9; 
    } else if (fuelType == "ELECTRIC") {
        fuelMultiplier = 0.3; 
        baseConsumptionRate = 15.0; 
    }
    
    return baseConsumptionRate * powerMultiplier * fuelMultiplier * hours;
}

int ConstructionEquipment::getSafetyInspectionInterval() const {
    int baseInterval = 90; 
    
    if (equipmentClass == "CRANE") {
        baseInterval = 30;
    } else if (equipmentClass == "HEAVY") {
        baseInterval = 60;
    } else if (equipmentClass == "PRESSURE") {
        baseInterval = 45;
    }
    
    double usageFactor = static_cast<double>(totalRentalDays) / 365.0;
    if (usageFactor > 0.7) {
        baseInterval = static_cast<int>(baseInterval * 0.7); 
    }
    
    return std::max(15, baseInterval); 
}

double ConstructionEquipment::calculateUsageBasedDepreciation(int hoursUsed) const {
    double baseDepreciation = RentalItem::calculateDepreciation();
    
    
    double hourlyDepreciationRate = 0.0;
    
    if (equipmentClass == "HEAVY") {
        hourlyDepreciationRate = 0.0002; 
    } else if (equipmentClass == "CRANE") {
        hourlyDepreciationRate = 0.0003; 
    } else {
        hourlyDepreciationRate = 0.0001; 
    }
    
    double usageDepreciation = basePricePerDay * hourlyDepreciationRate * hoursUsed;
    
    return baseDepreciation + usageDepreciation;
}

bool ConstructionEquipment::requiresCertifiedOperator() const {
    return equipmentClass == "CRANE" || equipmentClass == "HEAVY" || weight > 5000;
}