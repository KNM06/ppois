#include "../../include/items/Electronics.h"
#include <algorithm>
#include <cmath>

Electronics::Electronics(const std::string& id, const std::string& name, 
                         const std::string& type, const std::string& brand)
    : RentalItem(id, name, "ELECTRONICS", 45.0), 
      deviceType(type), manufacturer(brand), model("Standard"),
      powerConsumption(0.0), hasInsurance(true), warrantyMonths(12) {
    
    if (deviceType == "LAPTOP" || deviceType == "TABLET") {
        basePricePerDay = 35.0;
    } else if (deviceType == "CAMERA" || deviceType == "DRONE") {
        basePricePerDay = 60.0;
        accessories = {"Battery", "Charger", "Case"};
    } else if (deviceType == "AUDIO_EQUIPMENT") {
        basePricePerDay = 25.0;
    } else if (deviceType == "VR_EQUIPMENT") {
        basePricePerDay = 80.0;
    }
}

double Electronics::calculatePowerCost(int usageHours, double electricityRate) const {
    double dailyConsumption = powerConsumption * usageHours / 1000.0; 
    
    double accessoryConsumption = 0.0;
    for (const auto& accessory : accessories) {
        if (accessory == "EXTRA_BATTERY") accessoryConsumption += 0.1;
        else if (accessory == "SPEAKER") accessoryConsumption += 0.05;
        else if (accessory == "DISPLAY") accessoryConsumption += 0.2;
    }
    
    double totalConsumption = dailyConsumption + accessoryConsumption;
    return totalConsumption * electricityRate;
}

bool Electronics::requiresTechnicalSupport() const {
    if (deviceType == "VR_EQUIPMENT" || deviceType == "DRONE") {
        return true; 
    }
    
    if (totalRentalDays > 730) { 
        return true;
    }
    
    for (const auto& accessory : accessories) {
        if (accessory == "PROFESSIONAL_LENS" || accessory == "SPECIALIZED_SOFTWARE") {
            return true;
        }
    }
    
    return false;
}

double Electronics::calculateAccessoryReplacementCost() const {
    double totalCost = 0.0;
    
    for (const auto& accessory : accessories) {
        if (accessory == "BATTERY") totalCost += 50.0;
        else if (accessory == "CHARGER") totalCost += 25.0;
        else if (accessory == "CASE") totalCost += 30.0;
        else if (accessory == "PROFESSIONAL_LENS") totalCost += 300.0;
        else if (accessory == "MEMORY_CARD") totalCost += 40.0;
        else if (accessory == "TRIPOD") totalCost += 80.0;
        else totalCost += 20.0; 
    }
    
    double wearAndTearFactor = 1.0 + (static_cast<double>(totalRentalDays) / 365.0 * 0.3);
    
    return totalCost * wearAndTearFactor;
}

int Electronics::getRecommendedCalibrationInterval() const {
    int baseInterval = 180; 
    
    if (deviceType == "MEASUREMENT" || deviceType == "MEDICAL") {
        baseInterval = 90;
    } else if (deviceType == "AUDIO_PROFESSIONAL") {
        baseInterval = 120;
    } else if (deviceType == "PHOTOGRAPHY") {
        baseInterval = 365; 
    }
    
    if (totalRentalDays > 100) {
        baseInterval = static_cast<int>(baseInterval * 0.8);
    }
    
    return baseInterval;
}

bool Electronics::validateCompatibility(const std::string& userTechLevel) const {
    
    if (userTechLevel == "BEGINNER") {
        if (deviceType == "PROFESSIONAL_CAMERA" || 
            deviceType == "VR_EQUIPMENT" ||
            deviceType == "AUDIO_MIXER") {
            return false;
        }
    } else if (userTechLevel == "INTERMEDIATE") {
        return true;
    } else if (userTechLevel == "EXPERT") {
        return true;
    }
    
    for (const auto& accessory : accessories) {
        if (accessory == "ADVANCED_SOFTWARE" && userTechLevel != "EXPERT") {
            return false;
        }
    }
    
    return true;
}

double Electronics::calculateDataTransferCost(double dataSizeGB) const {
    double baseTransferCost = 0.0;
    
    if (deviceType == "CAMERA" || deviceType == "DRONE") {
        baseTransferCost = 0.10; 
    } else if (deviceType == "LAPTOP" || deviceType == "TABLET") {
        baseTransferCost = 0.05; 
    } else {
        baseTransferCost = 0.02; 
    }
    
    double totalCost = dataSizeGB * baseTransferCost;
    if (totalCost < 5.0) {
        totalCost = 5.0; 
    }
    
    return totalCost;
}