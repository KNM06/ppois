#ifndef COMPLIANCEMANAGER_H
#define COMPLIANCEMANAGER_H

#include "../../include/core/RentalSystem.h"
#include "../../include/business/EnvironmentalCompliance.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

struct ComplianceCheck {
    std::string checkId;
    std::string regulationType;
    std::string itemCategory;
    std::string checkDate;
    bool passed;
    std::vector<std::string> violations;
};

class ComplianceManager {
private:
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<EnvironmentalCompliance> environmentalCompliance;
    std::unordered_map<std::string, ComplianceCheck> complianceChecks;
    std::unordered_map<std::string, std::vector<std::string>> regulatoryRequirements;
    std::vector<std::string> highRiskCategories;

public:
    ComplianceManager(std::shared_ptr<RentalSystem> rentalSys, std::shared_ptr<EnvironmentalCompliance> envCompliance);
    
    bool performComplianceCheck(const std::string& itemId, const std::string& regulationType);
    double calculateComplianceScore(const std::string& category) const;
    bool meetsRegulatoryRequirements(const std::string& itemId, const std::string& regulation) const;
    std::vector<std::string> generateComplianceReport() const;
    double calculateNonCompliancePenalty(const std::string& regulationType) const;
    bool requiresSpecialLicense(const std::string& itemCategory) const;
    void updateRegulatoryRequirements(const std::string& regulation, const std::vector<std::string>& requirements);
    std::vector<std::string> getComplianceViolations() const;
    
    std::shared_ptr<RentalSystem> getRentalSystem() const;
    std::shared_ptr<EnvironmentalCompliance> getEnvironmentalCompliance() const;
    std::vector<std::string> getRegulatoryRequirements(const std::string& regulation) const;
    
    void setRentalSystem(std::shared_ptr<RentalSystem> system);
    void setEnvironmentalCompliance(std::shared_ptr<EnvironmentalCompliance> compliance);
    void addHighRiskCategory(const std::string& category);
};

#endif