#ifndef TOOLS_H
#define TOOLS_H

#include "../../include/core/RentalItem.h"
#include <string>
#include <vector>

class Tools : public RentalItem {
private:
    std::string toolType;
    std::string material;
    double weight;
    bool isPowerTool;
    double powerConsumption;
    std::vector<std::string> compatibleAccessories;
    int durabilityScore;

public:
    Tools(const std::string& id, const std::string& name, 
          const std::string& type, const std::string& toolMaterial);
    
    double calculateSharpeningCost() const;
    bool requiresSpecialStorage() const;
    double calculateWearAndTear(int usageHours) const;
    int getRecommendedMaintenanceInterval() const;
    bool validateUserExperience(const std::string& userSkillLevel) const;
    double calculateAccessoryCompatibilityCost(const std::string& accessory) const;
    
    std::string getToolType() const { return toolType; }
    std::string getMaterial() const { return material; }
    bool getIsPowerTool() const { return isPowerTool; }
    int getDurabilityScore() const { return durabilityScore; }
};

#endif