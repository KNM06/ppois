#ifndef ELECTRONICS_H
#define ELECTRONICS_H

#include "../../include/core/RentalItem.h"
#include <string>
#include <vector>

class Electronics : public RentalItem {
private:
    std::string deviceType;
    std::string manufacturer;
    std::string model;
    std::vector<std::string> accessories;
    double powerConsumption;
    bool hasInsurance;
    int warrantyMonths;

public:
    Electronics(const std::string& id, const std::string& name, 
                const std::string& type, const std::string& brand);
    double calculatePowerCost(int usageHours, double electricityRate) const;
    bool requiresTechnicalSupport() const;
    double calculateAccessoryReplacementCost() const;
    int getRecommendedCalibrationInterval() const;
    bool validateCompatibility(const std::string& userTechLevel) const;
    double calculateDataTransferCost(double dataSizeGB) const;
    
    std::string getDeviceType() const { return deviceType; }
    std::string getManufacturer() const { return manufacturer; }
    bool getHasInsurance() const { return hasInsurance; }
};

#endif