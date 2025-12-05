#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/InsuranceCalculator.h"
#include "../../include/utils/DateUtils.h"
#include <algorithm>
#include <cmath>

DamageAssessor::DamageAssessor() 
    : insuranceCalculator(nullptr) {
    
    damageRateMultipliers["MINOR_SCRATCH"] = 0.02;
    damageRateMultipliers["MAJOR_SCRATCH"] = 0.08;
    damageRateMultipliers["DENT"] = 0.05;
    damageRateMultipliers["CRACK"] = 0.15;
    damageRateMultipliers["BROKEN"] = 0.25;
    damageRateMultipliers["WATER_DAMAGE"] = 0.35;
    damageRateMultipliers["ELECTRICAL_FAILURE"] = 0.20;
    damageRateMultipliers["MECHANICAL_FAILURE"] = 0.18;
    damageRateMultipliers["THEFT"] = 1.0;
    damageRateMultipliers["LOST"] = 1.0;
    
    categoryRepairCosts["VEHICLE"] = 200.0;
    categoryRepairCosts["ELECTRONICS"] = 150.0;
    categoryRepairCosts["CONSTRUCTION"] = 180.0;
    categoryRepairCosts["TOOLS"] = 80.0;
    categoryRepairCosts["SPORTS"] = 100.0;
    categoryRepairCosts["LUXURY"] = 300.0;
    
    repairVendors["VEHICLE"] = {"AutoFix Pro", "CarCare Center"};
    repairVendors["ELECTRONICS"] = {"TechRepair Inc", "Gadget Masters"};
    repairVendors["CONSTRUCTION"] = {"HeavyEquipment Repair", "Industrial Services"};
}

DamageAssessor::DamageAssessor(std::shared_ptr<InsuranceCalculator> insuranceCalc)
    : insuranceCalculator(insuranceCalc) {
    
    damageRateMultipliers["MINOR_SCRATCH"] = 0.02;
    damageRateMultipliers["MAJOR_SCRATCH"] = 0.08;
    damageRateMultipliers["DENT"] = 0.05;
    damageRateMultipliers["CRACK"] = 0.15;
    damageRateMultipliers["BROKEN"] = 0.25;
    damageRateMultipliers["WATER_DAMAGE"] = 0.35;
    damageRateMultipliers["ELECTRICAL_FAILURE"] = 0.20;
    damageRateMultipliers["MECHANICAL_FAILURE"] = 0.18;
    damageRateMultipliers["THEFT"] = 1.0;
    damageRateMultipliers["LOST"] = 1.0;
    
    categoryRepairCosts["VEHICLE"] = 200.0;
    categoryRepairCosts["ELECTRONICS"] = 150.0;
    categoryRepairCosts["CONSTRUCTION"] = 180.0;
    categoryRepairCosts["TOOLS"] = 80.0;
    categoryRepairCosts["SPORTS"] = 100.0;
    categoryRepairCosts["LUXURY"] = 300.0;
}

double DamageAssessor::assessDamageCost(const std::string& category, double itemValue, 
                                      const std::string& damageType, const std::string& damageSeverity) const {
    double baseRepairCost = 0.0;
    auto categoryIt = categoryRepairCosts.find(category);
    if (categoryIt != categoryRepairCosts.end()) {
        baseRepairCost = categoryIt->second;
    }
    
    double damageMultiplier = 1.0;
    auto damageIt = damageRateMultipliers.find(damageType);
    if (damageIt != damageRateMultipliers.end()) {
        damageMultiplier = damageIt->second;
    }
    
    double severityMultiplier = 1.0;
    if (damageSeverity == "MINOR") {
        severityMultiplier = 0.5;
    } else if (damageSeverity == "MODERATE") {
        severityMultiplier = 1.0;
    } else if (damageSeverity == "SEVERE") {
        severityMultiplier = 2.0;
    } else if (damageSeverity == "TOTAL") {
        return itemValue;
    }
    
    double repairCost = baseRepairCost * damageMultiplier * severityMultiplier;
    
    double maxRepairCost = itemValue * 0.8;
    return std::min(maxRepairCost, repairCost);
}

bool DamageAssessor::isDamageRepairable(const std::string& category, const std::string& damageType) const {
    if (damageType == "THEFT" || damageType == "LOST") {
        return false;
    }
    
    if (damageType == "TOTAL_DESTRUCTION") {
        return false;
    }
    
    if (category == "ELECTRONICS" && damageType == "WATER_DAMAGE") {
        return false;
    }
    
    if (category == "CONSTRUCTION" && damageType == "STRUCTURAL_FAILURE") {
        return false;
    }
    
    double repairCost = assessDamageCost(category, 1000.0, damageType, "MODERATE");
    double itemValue = 1000.0;
    bool economicallyFeasible = repairCost < (itemValue * 0.6);
    
    return economicallyFeasible;
}

double DamageAssessor::calculateDepreciationAfterDamage(double originalValue, double repairCost, 
                                                      const std::string& damageType) const {
    double depreciationRate = 0.0;
    
    if (damageType == "MINOR_SCRATCH") {
        depreciationRate = 0.05;
    } else if (damageType == "MAJOR_SCRATCH" || damageType == "DENT") {
        depreciationRate = 0.15;
    } else if (damageType == "CRACK" || damageType == "MECHANICAL_FAILURE") {
        depreciationRate = 0.25;
    } else if (damageType == "BROKEN" || damageType == "ELECTRICAL_FAILURE") {
        depreciationRate = 0.35;
    } else if (damageType == "WATER_DAMAGE") {
        depreciationRate = 0.45;
    } else if (damageType == "STRUCTURAL_FAILURE") {
        depreciationRate = 0.7;
    }
    
    double repairDepreciation = (repairCost / originalValue) * 0.3;
    
    double totalDepreciation = depreciationRate + repairDepreciation;
    
    return originalValue * std::min(0.8, totalDepreciation);
}

std::string DamageAssessor::determineDamageResponsibility(const std::string& customerHistory, 
                                                        const std::string& damageType) const {
    if (damageType == "NORMAL_WEAR") {
        return "COMPANY";
    }
    
    if (damageType == "MANUFACTURING_DEFECT") {
        return "MANUFACTURER";
    }
    
    if (customerHistory == "NEGLIGENT" || customerHistory == "MULTIPLE_INCIDENTS") {
        return "CUSTOMER";
    }
    
    if (damageType == "ACCIDENT" && customerHistory == "GOOD") {
        return "SHARED";
    }
    
    if (damageType == "THEFT" || damageType == "LOST") {
        return "CUSTOMER";
    }
    
    return "INVESTIGATION_REQUIRED";
}

double DamageAssessor::calculateEnvironmentalCleanupCost(const std::string& category, 
                                                       const std::string& damageType) const {
    double cleanupCost = 0.0;
    
    if (damageType == "OIL_LEAK" || damageType == "FUEL_SPILL") {
        cleanupCost = 500.0;
        if (category == "CONSTRUCTION" || category == "VEHICLE") {
            cleanupCost *= 1.5;
        }
    } else if (damageType == "CHEMICAL_SPILL") {
        cleanupCost = 1000.0;
    } else if (damageType == "HAZARDOUS_MATERIAL") {
        cleanupCost = 2000.0;
    } else if (damageType == "BATTERY_ACID") {
        cleanupCost = 300.0;
    }
    
    double disposalCost = 0.0;
    if (damageType == "ELECTRONIC_WASTE") {
        disposalCost = 150.0;
    } else if (damageType == "HAZARDOUS_WASTE") {
        disposalCost = 400.0;
    }
    
    return cleanupCost + disposalCost;
}

bool DamageAssessor::requiresProfessionalAssessment(const std::string& category, double estimatedDamage) const {
    if (estimatedDamage > 1000.0) {
        return true;
    }
    
    if (category == "LUXURY" || category == "SPECIALIZED") {
        return true;
    }
    
    if (category == "VEHICLE" && estimatedDamage > 500.0) {
        return true;
    }
    
    if (category == "CONSTRUCTION") {
        return true;
    }
    
    return estimatedDamage > 200.0;
}

std::vector<std::string> DamageAssessor::getRecommendedRepairVendors(const std::string& category) const {
    auto it = repairVendors.find(category);
    if (it != repairVendors.end()) {
        return it->second;
    }
    return {"General Repair Service"};
}

double DamageAssessor::calculateTotalLossThreshold(const std::string& category, double itemValue) const {
    double threshold = itemValue * 0.7;
    
    if (category == "ELECTRONICS") {
        threshold = itemValue * 0.6;
    } else if (category == "CONSTRUCTION") {
        threshold = itemValue * 0.8;
    } else if (category == "LUXURY") {
        threshold = itemValue * 0.5;
    }
    
    return threshold;
}

void DamageAssessor::updateDamageRateMultiplier(const std::string& damageType, double multiplier) {
    damageRateMultipliers[damageType] = std::max(0.01, std::min(1.0, multiplier));
}

std::shared_ptr<InsuranceCalculator> DamageAssessor::getInsuranceCalculator() const {
    return insuranceCalculator;
}

void DamageAssessor::setInsuranceCalculator(std::shared_ptr<InsuranceCalculator> calculator) {
    insuranceCalculator = calculator;
}

void DamageAssessor::addRepairVendor(const std::string& category, const std::string& vendor) {
    repairVendors[category].push_back(vendor);
}