#include "../../include/items/Tools.h"
#include <algorithm>
#include <cmath>

Tools::Tools(const std::string& id, const std::string& name, 
             const std::string& type, const std::string& toolMaterial)
    : RentalItem(id, name, "TOOLS", 25.0),
      toolType(type), material(toolMaterial), weight(0.0),
      isPowerTool(false), powerConsumption(0.0), durabilityScore(100) {
    
    if (toolType == "POWER_DRILL" || toolType == "SAW") {
        isPowerTool = true;
        basePricePerDay = 35.0;
        powerConsumption = 500.0; 
        durabilityScore = 80;
    } else if (toolType == "GARDEN" || toolType == "CONSTRUCTION") {
        basePricePerDay = 30.0;
        durabilityScore = 70;
    } else if (toolType == "PRECISION") {
        basePricePerDay = 40.0;
        durabilityScore = 90;
    }
    
    if (isPowerTool) {
        compatibleAccessories = {"Battery", "Charger", "Case"};
    } else {
        compatibleAccessories = {"Case", "Sharpening Kit"};
    }
}

double Tools::calculateSharpeningCost() const {
    if (toolType == "CUTTING" || toolType == "GARDEN") {
        double baseCost = 15.0;
        
        double materialMultiplier = 1.0;
        if (material == "DIAMOND" || material == "CARBIDE") {
            materialMultiplier = 2.5;
        } else if (material == "HIGH_SPEED_STEEL") {
            materialMultiplier = 1.8;
        } else if (material == "CARBON_STEEL") {
            materialMultiplier = 1.2;
        }
        
        double wearFactor = 1.0 - (durabilityScore / 100.0);
        
        return baseCost * materialMultiplier * (1.0 + wearFactor);
    }
    return 0.0; 
}

bool Tools::requiresSpecialStorage() const {
    if (toolType == "PRECISION" || material == "SENSITIVE") {
        return true; 
    }
    
    if (isPowerTool && powerConsumption > 1000) {
        return true; 
    }
    
    for (const auto& accessory : compatibleAccessories) {
        if (accessory == "Battery" || accessory == "LITHIUM_BATTERY") {
            return true;
        }
    }
    
    return false;
}

double Tools::calculateWearAndTear(int usageHours) const {
    double baseWearRate = 0.01; 
    
    if (toolType == "PRECISION") {
        baseWearRate = 0.02; 
    } else if (toolType == "HEAVY_DUTY") {
        baseWearRate = 0.015;
    } else if (toolType == "GARDEN") {
        baseWearRate = 0.005; 
    }
    
    double materialFactor = 1.0;
    if (material == "DIAMOND") materialFactor = 0.1;
    else if (material == "CARBIDE") materialFactor = 0.3;
    else if (material == "STAINLESS_STEEL") materialFactor = 0.7;
    else if (material == "PLASTIC") materialFactor = 1.5;
    
    double totalWear = baseWearRate * materialFactor * usageHours;
    
    double currentDurability = durabilityScore / 100.0;
    totalWear *= (2.0 - currentDurability); 
    
    return totalWear * basePricePerDay * 10; 
}

int Tools::getRecommendedMaintenanceInterval() const {
    int baseInterval = 180; 
    
    if (isPowerTool) {
        baseInterval = 90; 
    }
    
    if (toolType == "PRECISION") {
        baseInterval = 60; 
    }
    
    double usageIntensity = static_cast<double>(totalRentalDays) / 365.0;
    if (usageIntensity > 0.5) {
        baseInterval = static_cast<int>(baseInterval * 0.7);
    }
    
    if (durabilityScore < 50) {
        baseInterval = static_cast<int>(baseInterval * 0.5);
    }
    
    return std::max(30, baseInterval); 
}

bool Tools::validateUserExperience(const std::string& userSkillLevel) const {
    
    if (userSkillLevel == "BEGINNER") {
        if (toolType == "POWER_SAW" || toolType == "ANGLE_GRINDER" || 
            toolType == "NAIL_GUN") {
            return false;
        }
    } else if (userSkillLevel == "INTERMEDIATE") {
        return true;
    } else if (userSkillLevel == "EXPERT") {
        return true;
    }
    
    if (isPowerTool && powerConsumption > 1500 && userSkillLevel != "EXPERT") {
        return false;
    }
    
    return true;
}

double Tools::calculateAccessoryCompatibilityCost(const std::string& accessory) const {
    bool isCompatible = false;
    for (const auto& compAccessory : compatibleAccessories) {
        if (compAccessory == accessory) {
            isCompatible = true;
            break;
        }
    }
    
    if (!isCompatible) {
        return 0.0; 
    }
    
    double accessoryCost = 0.0;
    if (accessory == "Battery") {
        accessoryCost = 20.0;
    } else if (accessory == "Charger") {
        accessoryCost = 15.0;
    } else if (accessory == "Professional_Case") {
        accessoryCost = 25.0;
    } else if (accessory == "Sharpening_Kit") {
        accessoryCost = 30.0;
    } else if (accessory == "Laser_Guide") {
        accessoryCost = 40.0;
    } else {
        accessoryCost = 10.0; 
    }
    
    return accessoryCost;
}