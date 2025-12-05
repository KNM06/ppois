#include "../../include/business/QualityAssurance.h"
#include <algorithm>
#include <cmath>

QualityAssurance::QualityAssurance(std::shared_ptr<Inventory> inv, std::shared_ptr<DamageAssessor> assessor)
    : inventory(inv), damageAssessor(assessor) {
    
    categoryQualityStandards["VEHICLE"] = 0.9;
    categoryQualityStandards["ELECTRONICS"] = 0.85;
    categoryQualityStandards["CONSTRUCTION"] = 0.8;
    categoryQualityStandards["TOOLS"] = 0.75;
    categoryQualityStandards["LUXURY"] = 0.95;
    
    qualityChecklist = {"Functionality", "Safety", "Cleanliness", "Documentation"};
}

bool QualityAssurance::performQualityInspection(const std::string& itemId, const std::string& inspectorId) {
    QualityInspection inspection;
    inspection.inspectionId = "QAI" + std::to_string(inspections.size() + 1000);
    inspection.itemId = itemId;
    inspection.inspectionDate = "2024-01-01";
    inspection.inspectorId = inspectorId;
    
    double qualityScore = calculateItemQualityScore(itemId);
    inspection.qualityScore = qualityScore;
    
    auto standardIt = categoryQualityStandards.find("GENERAL");
    double requiredStandard = standardIt != categoryQualityStandards.end() ? standardIt->second : 0.7;
    
    inspection.passed = qualityScore >= requiredStandard;
    
    if (!inspection.passed) {
        inspection.defects.push_back("Quality score below standard");
    }
    
    inspections[inspection.inspectionId] = inspection;
    return inspection.passed;
}

double QualityAssurance::calculateItemQualityScore(const std::string& itemId) const {
    double baseScore = 0.8;
    
    if (damageAssessor) {
        double damageCost = damageAssessor->assessDamageCost("GENERAL", 1000.0, "MINOR_SCRATCH", "MINOR");
        baseScore -= damageCost / 1000.0 * 0.5;
    }
    
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    
    auto standardIt = categoryQualityStandards.find(category);
    if (standardIt != categoryQualityStandards.end()) {
        baseScore = (baseScore + standardIt->second) / 2.0;
    }
    
    int inspectionCount = 0;
    for (const auto& [id, inspection] : inspections) {
        if (inspection.itemId == itemId) {
            inspectionCount++;
        }
    }
    
    double inspectionBonus = inspectionCount * 0.02;
    return std::min(1.0, baseScore + inspectionBonus);
}

bool QualityAssurance::meetsQualityStandards(const std::string& itemId, const std::string& category) const {
    double qualityScore = calculateItemQualityScore(itemId);
    auto standardIt = categoryQualityStandards.find(category);
    double requiredStandard = standardIt != categoryQualityStandards.end() ? standardIt->second : 0.7;
    
    return qualityScore >= requiredStandard;
}

std::vector<std::string> QualityAssurance::generateQualityReport() const {
    std::vector<std::string> report;
    
    int passedCount = 0;
    int totalCount = 0;
    
    for (const auto& [id, inspection] : inspections) {
        totalCount++;
        if (inspection.passed) {
            passedCount++;
        }
    }
    
    double passRate = totalCount > 0 ? static_cast<double>(passedCount) / totalCount : 0.0;
    report.push_back("Quality Inspection Report");
    report.push_back("Total Inspections: " + std::to_string(totalCount));
    report.push_back("Passed: " + std::to_string(passedCount));
    report.push_back("Pass Rate: " + std::to_string(passRate * 100) + "%");
    
    if (passRate < 0.8) {
        report.push_back("ALERT: Quality standards below target");
    }
    
    return report;
}

double QualityAssurance::calculateQualityAssuranceCost(const std::string& category) const {
    double baseCost = 50.0;
    
    auto standardIt = categoryQualityStandards.find(category);
    if (standardIt != categoryQualityStandards.end()) {
        baseCost *= (1.0 + standardIt->second);
    }
    
    int inspectionCount = 0;
    for (const auto& [id, inspection] : inspections) {
        inspectionCount++;
    }
    
    double inspectionCost = inspectionCount * 25.0;
    return baseCost + inspectionCost;
}

bool QualityAssurance::requiresReinspection(const std::string& itemId) const {
    int inspectionCount = 0;
    bool lastInspectionFailed = false;
    
    for (const auto& [id, inspection] : inspections) {
        if (inspection.itemId == itemId) {
            inspectionCount++;
            if (!inspection.passed) {
                lastInspectionFailed = true;
            }
        }
    }
    
    return lastInspectionFailed || inspectionCount == 0;
}

void QualityAssurance::updateQualityStandards(const std::string& category, double standard) {
    categoryQualityStandards[category] = std::max(0.0, std::min(1.0, standard));
}

std::vector<std::string> QualityAssurance::getFailedInspections() const {
    std::vector<std::string> failedItems;
    
    for (const auto& [id, inspection] : inspections) {
        if (!inspection.passed) {
            failedItems.push_back(inspection.itemId);
        }
    }
    
    return failedItems;
}

std::shared_ptr<Inventory> QualityAssurance::getInventory() const {
    return inventory;
}

std::shared_ptr<DamageAssessor> QualityAssurance::getDamageAssessor() const {
    return damageAssessor;
}

double QualityAssurance::getCategoryQualityStandard(const std::string& category) const {
    auto it = categoryQualityStandards.find(category);
    return it != categoryQualityStandards.end() ? it->second : 0.7;
}

void QualityAssurance::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void QualityAssurance::setDamageAssessor(std::shared_ptr<DamageAssessor> assessor) {
    damageAssessor = assessor;
}

void QualityAssurance::addQualityChecklistItem(const std::string& checklistItem) {
    if (std::find(qualityChecklist.begin(), qualityChecklist.end(), checklistItem) == qualityChecklist.end()) {
        qualityChecklist.push_back(checklistItem);
    }
}