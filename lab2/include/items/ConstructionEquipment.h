#ifndef CONSTRUCTIONEQUIPMENT_H
#define CONSTRUCTIONEQUIPMENT_H

#include "../../include/core/RentalItem.h"
#include <string>

class ConstructionEquipment : public RentalItem {
private:
    std::string equipmentClass;
    double weight;
    double powerOutput;
    std::string fuelType;
    bool requiresSpecialLicense;
    double hourlyUsageRate;
    int maxOperatingHours;

public:
    ConstructionEquipment(const std::string& id, const std::string& name, 
                         const std::string& equipmentClass, double weight);
    
    double calculateTransportationCost(double distance) const;
    bool validateOperatorLicense(const std::string& licenseType) const;
    double calculateFuelConsumption(int hours) const;
    int getSafetyInspectionInterval() const;
    double calculateUsageBasedDepreciation(int hoursUsed) const;
    bool requiresCertifiedOperator() const;
    
    std::string getEquipmentClass() const { return equipmentClass; }
    double getWeight() const { return weight; }
    bool getRequiresSpecialLicense() const { return requiresSpecialLicense; }
};

#endif