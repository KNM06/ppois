#ifndef QUALITYASSURANCE_H
#define QUALITYASSURANCE_H

#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

struct QualityInspection {
    std::string inspectionId;
    std::string itemId;
    std::string inspectionDate;
    std::string inspectorId;
    double qualityScore;
    std::vector<std::string> defects;
    bool passed;
};

class QualityAssurance {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::unordered_map<std::string, QualityInspection> inspections;
    std::unordered_map<std::string, double> categoryQualityStandards;
    std::vector<std::string> qualityChecklist;

public:
    QualityAssurance(std::shared_ptr<Inventory> inv, std::shared_ptr<DamageAssessor> assessor);
    
    bool performQualityInspection(const std::string& itemId, const std::string& inspectorId);
    double calculateItemQualityScore(const std::string& itemId) const;
    bool meetsQualityStandards(const std::string& itemId, const std::string& category) const;
    std::vector<std::string> generateQualityReport() const;
    double calculateQualityAssuranceCost(const std::string& category) const;
    bool requiresReinspection(const std::string& itemId) const;
    void updateQualityStandards(const std::string& category, double standard);
    std::vector<std::string> getFailedInspections() const;
    
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<DamageAssessor> getDamageAssessor() const;
    double getCategoryQualityStandard(const std::string& category) const;
    
    void setInventory(std::shared_ptr<Inventory> inv);
    void setDamageAssessor(std::shared_ptr<DamageAssessor> assessor);
    void addQualityChecklistItem(const std::string& checklistItem);
};

#endif