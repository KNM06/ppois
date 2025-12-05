#include "../../include/business/InsuranceCalculator.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/utils/ValidationUtils.h"
#include <algorithm>
#include <cmath>

InsuranceCalculator::InsuranceCalculator() 
    : baseInsuranceRate(0.03), damageAssessor(nullptr) {
    
    categoryRiskFactors["VEHICLE"] = 1.8;
    categoryRiskFactors["ELECTRONICS"] = 1.3;
    categoryRiskFactors["CONSTRUCTION"] = 2.2;
    categoryRiskFactors["TOOLS"] = 1.1;
    categoryRiskFactors["SPORTS"] = 1.5;
    categoryRiskFactors["LUXURY"] = 2.5;
    
    customerRiskMultipliers["NEW"] = 1.5;
    customerRiskMultipliers["REGULAR"] = 1.0;
    customerRiskMultipliers["VIP"] = 0.8;
    customerRiskMultipliers["BUSINESS"] = 0.9;
    customerRiskMultipliers["STUDENT"] = 1.3;
    
    locationRiskFactors["HIGH_CRIME"] = 1.5;
    locationRiskFactors["BUSINESS_DISTRICT"] = 0.9;
    locationRiskFactors["RESIDENTIAL"] = 1.0;
    locationRiskFactors["INDUSTRIAL"] = 1.3;
}

InsuranceCalculator::InsuranceCalculator(std::shared_ptr<DamageAssessor> assessor)
    : baseInsuranceRate(0.03), damageAssessor(assessor) {
    
    categoryRiskFactors["VEHICLE"] = 1.8;
    categoryRiskFactors["ELECTRONICS"] = 1.3;
    categoryRiskFactors["CONSTRUCTION"] = 2.2;
    categoryRiskFactors["TOOLS"] = 1.1;
    categoryRiskFactors["SPORTS"] = 1.5;
    categoryRiskFactors["LUXURY"] = 2.5;
    
    customerRiskMultipliers["NEW"] = 1.5;
    customerRiskMultipliers["REGULAR"] = 1.0;
    customerRiskMultipliers["VIP"] = 0.8;
    customerRiskMultipliers["BUSINESS"] = 0.9;
    customerRiskMultipliers["STUDENT"] = 1.3;
}

double InsuranceCalculator::calculateInsurancePremium(const std::string& category, double itemValue, 
                                                    const std::string& customerType, int rentalDays) const {
    double basePremium = itemValue * baseInsuranceRate;
    
    auto categoryIt = categoryRiskFactors.find(category);
    double categoryMultiplier = (categoryIt != categoryRiskFactors.end()) ? categoryIt->second : 1.0;
    basePremium *= categoryMultiplier;
    
    auto customerIt = customerRiskMultipliers.find(customerType);
    double customerMultiplier = (customerIt != customerRiskMultipliers.end()) ? customerIt->second : 1.0;
    basePremium *= customerMultiplier;
    
    double durationMultiplier = 1.0 + (rentalDays * 0.005);
    basePremium *= durationMultiplier;
    
    double minPremium = 10.0;
    return std::max(minPremium, basePremium);
}

double InsuranceCalculator::calculateDamageDeductible(const std::string& category, double itemValue) const {
    double baseDeductible = itemValue * 0.1;
    
    double categoryMultiplier = 1.0;
    if (category == "VEHICLE" || category == "CONSTRUCTION") {
        categoryMultiplier = 0.15;
    } else if (category == "LUXURY" || category == "ELECTRONICS") {
        categoryMultiplier = 0.12;
    } else {
        categoryMultiplier = 0.08;
    }
    
    double deductible = itemValue * categoryMultiplier;
    
    double minDeductible = 50.0;
    double maxDeductible = 1000.0;
    
    return std::max(minDeductible, std::min(maxDeductible, deductible));
}

bool InsuranceCalculator::requiresAdditionalCoverage(const std::string& category, double itemValue) const {
    if (itemValue > 5000.0) {
        return true;
    }
    
    if (category == "CONSTRUCTION" || category == "VEHICLE") {
        return true;
    }
    
    if (category == "LUXURY" && itemValue > 2000.0) {
        return true;
    }
    
    if (category == "ELECTRONICS" && itemValue > 1500.0) {
        return true;
    }
    
    return false;
}

double InsuranceCalculator::calculateTheftProtectionPremium(const std::string& category, double itemValue) const {
    double baseTheftRate = 0.015;
    
    double categoryMultiplier = 1.0;
    if (category == "ELECTRONICS" || category == "LUXURY") {
        categoryMultiplier = 1.8;
    } else if (category == "VEHICLE") {
        categoryMultiplier = 2.0;
    } else if (category == "TOOLS") {
        categoryMultiplier = 1.2;
    }
    
    double theftPremium = itemValue * baseTheftRate * categoryMultiplier;
    
    double minTheftPremium = 15.0;
    return std::max(minTheftPremium, theftPremium);
}

double InsuranceCalculator::calculateLiabilityCoverage(const std::string& category, const std::string& customerType) const {
    double baseLiability = 100000.0;
    
    double categoryMultiplier = 1.0;
    if (category == "CONSTRUCTION" || category == "VEHICLE") {
        categoryMultiplier = 3.0;
    } else if (category == "SPORTS" || category == "ELECTRONICS") {
        categoryMultiplier = 1.5;
    }
    
    double customerMultiplier = 1.0;
    if (customerType == "BUSINESS") {
        customerMultiplier = 2.0;
    } else if (customerType == "NEW") {
        customerMultiplier = 1.2;
    }
    
    return baseLiability * categoryMultiplier * customerMultiplier;
}

bool InsuranceCalculator::validateInsuranceClaim(double claimAmount, double itemValue, const std::string& damageType) const {
    if (claimAmount <= 0 || itemValue <= 0) {
        return false;
    }
    
    if (claimAmount > itemValue) {
        return false;
    }
    
    if (damageType == "NORMAL_WEAR" || damageType == "MINOR_SCRATCH") {
        return claimAmount > (itemValue * 0.05);
    }
    
    if (damageType == "THEFT") {
        return true;
    }
    
    if (damageType == "ACCIDENTAL" || damageType == "MAJOR_DAMAGE") {
        return claimAmount <= (itemValue * 0.8);
    }
    
    return false;
}

double InsuranceCalculator::calculateLocationRiskPremium(const std::string& category, const std::string& location) const {
    double basePremium = calculateInsurancePremium(category, 1000.0, "REGULAR", 1);
    
    auto locationIt = locationRiskFactors.find(location);
    double locationMultiplier = (locationIt != locationRiskFactors.end()) ? locationIt->second : 1.0;
    
    return basePremium * locationMultiplier;
}

std::unordered_map<std::string, double> InsuranceCalculator::getRiskAssessment(const std::string& category) const {
    std::unordered_map<std::string, double> riskAssessment;
    
    riskAssessment["Base Risk"] = baseInsuranceRate;
    
    auto categoryIt = categoryRiskFactors.find(category);
    if (categoryIt != categoryRiskFactors.end()) {
        riskAssessment["Category Risk"] = categoryIt->second;
    }
    
    riskAssessment["Theft Risk"] = calculateTheftProtectionPremium(category, 1000.0) / 1000.0;
    riskAssessment["Damage Risk"] = calculateDamageDeductible(category, 1000.0) / 1000.0;
    
    return riskAssessment;
}

std::shared_ptr<DamageAssessor> InsuranceCalculator::getDamageAssessor() const {
    return damageAssessor;
}

void InsuranceCalculator::updateCategoryRiskFactor(const std::string& category, double riskFactor) {
    categoryRiskFactors[category] = std::max(0.1, std::min(5.0, riskFactor));
}

void InsuranceCalculator::setDamageAssessor(std::shared_ptr<DamageAssessor> assessor) {
    damageAssessor = assessor;
}

void InsuranceCalculator::setLocationRiskFactor(const std::string& location, double riskFactor) {
    locationRiskFactors[location] = std::max(0.5, std::min(3.0, riskFactor));
}