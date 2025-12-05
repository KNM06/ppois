#ifndef DAMAGEASSESSOR_H
#define DAMAGEASSESSOR_H

#include "../../include/core/RentalItem.h"
#include "../../include/business/InsuranceCalculator.h"
#include "../../include/utils/DateUtils.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class RentalItem;
class InsuranceCalculator;

class DamageAssessor {
private:
    std::unordered_map<std::string, double> damageRateMultipliers;
    std::unordered_map<std::string, double> categoryRepairCosts;
    std::shared_ptr<InsuranceCalculator> insuranceCalculator;
    std::unordered_map<std::string, std::vector<std::string>> repairVendors;

public:
    DamageAssessor();
    DamageAssessor(std::shared_ptr<InsuranceCalculator> insuranceCalc);
    
    double assessDamageCost(const std::string& category, double itemValue, 
                          const std::string& damageType, const std::string& damageSeverity) const;
    bool isDamageRepairable(const std::string& category, const std::string& damageType) const;
    double calculateDepreciationAfterDamage(double originalValue, double repairCost, 
                                          const std::string& damageType) const;
    std::string determineDamageResponsibility(const std::string& customerHistory, 
                                            const std::string& damageType) const;
    double calculateEnvironmentalCleanupCost(const std::string& category, 
                                           const std::string& damageType) const;
    bool requiresProfessionalAssessment(const std::string& category, double estimatedDamage) const;
    std::vector<std::string> getRecommendedRepairVendors(const std::string& category) const;
    double calculateTotalLossThreshold(const std::string& category, double itemValue) const;
    
    void updateDamageRateMultiplier(const std::string& damageType, double multiplier);
    std::shared_ptr<InsuranceCalculator> getInsuranceCalculator() const;
    void setInsuranceCalculator(std::shared_ptr<InsuranceCalculator> calculator);
    void addRepairVendor(const std::string& category, const std::string& vendor);
};

#endif