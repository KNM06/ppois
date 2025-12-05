#ifndef INSURANCECALCULATOR_H
#define INSURANCECALCULATOR_H

#include "../../include/business/DamageAssessor.h"
#include "../../include/utils/ValidationUtils.h"
#include <string>
#include <unordered_map>
#include <memory>

class DamageAssessor;

class InsuranceCalculator {
private:
    std::unordered_map<std::string, double> categoryRiskFactors;
    std::unordered_map<std::string, double> customerRiskMultipliers;
    std::shared_ptr<DamageAssessor> damageAssessor;
    double baseInsuranceRate;
    std::unordered_map<std::string, double> locationRiskFactors;

public:
    InsuranceCalculator();
    InsuranceCalculator(std::shared_ptr<DamageAssessor> assessor);
    
    double calculateInsurancePremium(const std::string& category, double itemValue, 
                                   const std::string& customerType, int rentalDays) const;
    double calculateDamageDeductible(const std::string& category, double itemValue) const;
    bool requiresAdditionalCoverage(const std::string& category, double itemValue) const;
    double calculateTheftProtectionPremium(const std::string& category, double itemValue) const;
    double calculateLiabilityCoverage(const std::string& category, const std::string& customerType) const;
    bool validateInsuranceClaim(double claimAmount, double itemValue, const std::string& damageType) const;
    double calculateLocationRiskPremium(const std::string& category, const std::string& location) const;
    std::unordered_map<std::string, double> getRiskAssessment(const std::string& category) const;
    
    double getBaseInsuranceRate() const { return baseInsuranceRate; }
    std::shared_ptr<DamageAssessor> getDamageAssessor() const;
    
    void updateCategoryRiskFactor(const std::string& category, double riskFactor);
    void setDamageAssessor(std::shared_ptr<DamageAssessor> assessor);
    void setLocationRiskFactor(const std::string& location, double riskFactor);
};

#endif