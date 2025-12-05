#include "../../include/business/ComplianceManager.h"
#include <algorithm>

ComplianceManager::ComplianceManager(std::shared_ptr<RentalSystem> rentalSys, std::shared_ptr<EnvironmentalCompliance> envCompliance)
    : rentalSystem(rentalSys), environmentalCompliance(envCompliance) {
    
    regulatoryRequirements["SAFETY"] = {"Proper labeling", "Safety instructions", "Protective equipment"};
    regulatoryRequirements["ENVIRONMENTAL"] = {"Waste disposal", "Emissions control", "Recycling"};
    regulatoryRequirements["OPERATIONAL"] = {"Maintenance records", "Operator training", "Insurance coverage"};
    
    highRiskCategories = {"CONSTRUCTION", "VEHICLE", "CHEMICALS"};
}

bool ComplianceManager::performComplianceCheck(const std::string& itemId, const std::string& regulationType) {
    ComplianceCheck check;
    check.checkId = "COMP" + std::to_string(complianceChecks.size() + 1000);
    check.regulationType = regulationType;
    
    std::string category = "GENERAL";
    if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    else if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    check.itemCategory = category;
    
    check.checkDate = "2024-01-01";
    
    double complianceScore = calculateComplianceScore(category);
    check.passed = complianceScore >= 0.8;
    
    if (!check.passed) {
        check.violations.push_back("Compliance score below threshold: " + std::to_string(complianceScore));
    }
    
    auto reqIt = regulatoryRequirements.find(regulationType);
    if (reqIt != regulatoryRequirements.end()) {
        for (const auto& requirement : reqIt->second) {
            if (requirement.find("Safety") != std::string::npos && category == "CONSTRUCTION") {
                check.violations.push_back("Missing safety requirement: " + requirement);
            }
        }
    }
    
    complianceChecks[check.checkId] = check;
    return check.passed;
}

double ComplianceManager::calculateComplianceScore(const std::string& category) const {
    double baseScore = 0.8;
    
    bool isHighRisk = std::find(highRiskCategories.begin(), highRiskCategories.end(), category) != highRiskCategories.end();
    if (isHighRisk) {
        baseScore -= 0.1;
    }
    
    int passedChecks = 0;
    int totalChecks = 0;
    
    for (const auto& [checkId, check] : complianceChecks) {
        if (check.itemCategory == category) {
            totalChecks++;
            if (check.passed) {
                passedChecks++;
            }
        }
    }
    
    if (totalChecks > 0) {
        double checkScore = static_cast<double>(passedChecks) / totalChecks;
        baseScore = (baseScore + checkScore) / 2.0;
    }
    
    if (environmentalCompliance && environmentalCompliance->requiresEnvironmentalDisclosure(category)) {
        baseScore += 0.05;
    }
    
    return std::min(1.0, std::max(0.0, baseScore));
}

bool ComplianceManager::meetsRegulatoryRequirements(const std::string& itemId, const std::string& regulation) const {
    std::string category = "GENERAL";
    if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    
    double complianceScore = calculateComplianceScore(category);
    bool meetsBaseRequirements = complianceScore >= 0.7;
    
    auto reqIt = regulatoryRequirements.find(regulation);
    if (reqIt != regulatoryRequirements.end()) {
        bool hasSpecificRequirements = !reqIt->second.empty();
        return meetsBaseRequirements && hasSpecificRequirements;
    }
    
    return meetsBaseRequirements;
}

std::vector<std::string> ComplianceManager::generateComplianceReport() const {
    std::vector<std::string> report;
    
    int totalChecks = 0;
    int passedChecks = 0;
    std::unordered_map<std::string, int> categoryViolations;
    
    for (const auto& [checkId, check] : complianceChecks) {
        totalChecks++;
        if (check.passed) {
            passedChecks++;
        } else {
            categoryViolations[check.itemCategory]++;
        }
    }
    
    double passRate = totalChecks > 0 ? static_cast<double>(passedChecks) / totalChecks : 0.0;
    report.push_back("Compliance Report");
    report.push_back("Total Checks: " + std::to_string(totalChecks));
    report.push_back("Passed: " + std::to_string(passedChecks));
    report.push_back("Pass Rate: " + std::to_string(passRate * 100) + "%");
    
    for (const auto& [category, violations] : categoryViolations) {
        report.push_back(category + " violations: " + std::to_string(violations));
    }
    
    if (passRate < 0.9) {
        report.push_back("ALERT: Compliance below target level");
    }
    
    return report;
}

double ComplianceManager::calculateNonCompliancePenalty(const std::string& regulationType) const {
    double basePenalty = 1000.0;
    
    if (regulationType == "SAFETY") {
        basePenalty = 5000.0;
    } else if (regulationType == "ENVIRONMENTAL") {
        basePenalty = 3000.0;
    }
    
    int violationCount = 0;
    for (const auto& [checkId, check] : complianceChecks) {
        if (!check.passed && check.regulationType == regulationType) {
            violationCount++;
        }
    }
    
    return basePenalty * (1.0 + violationCount * 0.5);
}

bool ComplianceManager::requiresSpecialLicense(const std::string& itemCategory) const {
    return std::find(highRiskCategories.begin(), highRiskCategories.end(), itemCategory) != highRiskCategories.end();
}

void ComplianceManager::updateRegulatoryRequirements(const std::string& regulation, const std::vector<std::string>& requirements) {
    regulatoryRequirements[regulation] = requirements;
}

std::vector<std::string> ComplianceManager::getComplianceViolations() const {
    std::vector<std::string> violations;
    
    for (const auto& [checkId, check] : complianceChecks) {
        if (!check.passed) {
            for (const auto& violation : check.violations) {
                violations.push_back(check.itemCategory + ": " + violation);
            }
        }
    }
    
    return violations;
}

std::shared_ptr<RentalSystem> ComplianceManager::getRentalSystem() const {
    return rentalSystem;
}

std::shared_ptr<EnvironmentalCompliance> ComplianceManager::getEnvironmentalCompliance() const {
    return environmentalCompliance;
}

std::vector<std::string> ComplianceManager::getRegulatoryRequirements(const std::string& regulation) const {
    auto it = regulatoryRequirements.find(regulation);
    return it != regulatoryRequirements.end() ? it->second : std::vector<std::string>();
}

void ComplianceManager::setRentalSystem(std::shared_ptr<RentalSystem> system) {
    rentalSystem = system;
}

void ComplianceManager::setEnvironmentalCompliance(std::shared_ptr<EnvironmentalCompliance> compliance) {
    environmentalCompliance = compliance;
}

void ComplianceManager::addHighRiskCategory(const std::string& category) {
    if (std::find(highRiskCategories.begin(), highRiskCategories.end(), category) == highRiskCategories.end()) {
        highRiskCategories.push_back(category);
    }
}