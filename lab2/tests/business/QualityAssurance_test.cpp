#include <gtest/gtest.h>
#include "../../include/business/QualityAssurance.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/InsuranceCalculator.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class QualityAssuranceTest : public ::testing::Test {
protected:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::shared_ptr<QualityAssurance> qualityAssurance;

    void SetUp() override {
        auto insuranceCalculator = std::make_shared<InsuranceCalculator>();
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        damageAssessor = std::make_shared<DamageAssessor>(insuranceCalculator);
        
        inventory = std::make_shared<Inventory>(maintenanceScheduler, damageAssessor);
        
        maintenanceScheduler->setInventory(inventory);
        maintenanceScheduler->setDamageAssessor(damageAssessor);
        
        inventory->addItem(std::make_shared<RentalItem>("ITEM001", "Drill", "TOOLS", 50.0));
        inventory->addItem(std::make_shared<RentalItem>("VEH001", "Truck", "VEHICLE", 100.0));
        inventory->addItem(std::make_shared<RentalItem>("ELEC001", "Laptop", "ELECTRONICS", 200.0));

        qualityAssurance = std::make_shared<QualityAssurance>(inventory, damageAssessor);
    }
};

TEST_F(QualityAssuranceTest, PerformQualityInspectionSuccess) {
    bool passed = qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    EXPECT_TRUE(passed);
}

TEST_F(QualityAssuranceTest, PerformQualityInspectionFailure) {
    qualityAssurance->updateQualityStandards("GENERAL", 0.95);
    bool passed = qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    EXPECT_FALSE(passed);
    
    auto failed = qualityAssurance->getFailedInspections();
    EXPECT_FALSE(failed.empty());
    EXPECT_EQ(failed[0], "ITEM001");
}

TEST_F(QualityAssuranceTest, CalculateItemQualityScoreBase) {
    double score = qualityAssurance->calculateItemQualityScore("ITEM001");
    EXPECT_NEAR(score, 0.775, 0.05);
}

TEST_F(QualityAssuranceTest, CalculateItemQualityScoreWithInspections) {
    qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    qualityAssurance->performQualityInspection("ITEM001", "INSP002");
    double score = qualityAssurance->calculateItemQualityScore("ITEM001");
    EXPECT_GT(score, 0.775); 
}

TEST_F(QualityAssuranceTest, MeetsQualityStandardsTrue) {
    bool meets = qualityAssurance->meetsQualityStandards("ITEM001", "TOOLS");
    EXPECT_TRUE(meets);
}

TEST_F(QualityAssuranceTest, MeetsQualityStandardsFalse) {
    qualityAssurance->updateQualityStandards("TOOLS", 0.95);
    bool meets = qualityAssurance->meetsQualityStandards("ITEM001", "TOOLS");
    EXPECT_FALSE(meets);
}

TEST_F(QualityAssuranceTest, GenerateQualityReportEmpty) {
    auto report = qualityAssurance->generateQualityReport();
    EXPECT_FALSE(report.empty());
    EXPECT_NE(report[0].find("Quality Inspection Report"), std::string::npos);
}

TEST_F(QualityAssuranceTest, GenerateQualityReportWithAlert) {
    qualityAssurance->updateQualityStandards("GENERAL", 1.0);
    qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    
    auto report = qualityAssurance->generateQualityReport();
    bool foundAlert = false;
    for (const auto& line : report) {
        if (line.find("ALERT") != std::string::npos) {
            foundAlert = true;
            break;
        }
    }
    EXPECT_TRUE(foundAlert);
}

TEST_F(QualityAssuranceTest, CalculateQualityAssuranceCost) {
    double cost = qualityAssurance->calculateQualityAssuranceCost("TOOLS");
    EXPECT_NEAR(cost, 87.5, 0.001);
}

TEST_F(QualityAssuranceTest, CalculateQualityAssuranceCostWithInspections) {
    qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    double cost = qualityAssurance->calculateQualityAssuranceCost("TOOLS");
    EXPECT_NEAR(cost, 87.5 + 25.0, 0.001);
}

TEST_F(QualityAssuranceTest, RequiresReinspectionTrueFailed) {
    qualityAssurance->updateQualityStandards("GENERAL", 1.0);
    qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    EXPECT_TRUE(qualityAssurance->requiresReinspection("ITEM001"));
}

TEST_F(QualityAssuranceTest, RequiresReinspectionTrueNoInspection) {
    EXPECT_TRUE(qualityAssurance->requiresReinspection("ITEM002"));
}

TEST_F(QualityAssuranceTest, RequiresReinspectionFalsePassed) {
    qualityAssurance->performQualityInspection("ITEM001", "INSP001");
    EXPECT_FALSE(qualityAssurance->requiresReinspection("ITEM001"));
}

TEST_F(QualityAssuranceTest, UpdateQualityStandardsClamps) {
    qualityAssurance->updateQualityStandards("TOOLS", 1.5);
    EXPECT_DOUBLE_EQ(qualityAssurance->getCategoryQualityStandard("TOOLS"), 1.0);
    
    qualityAssurance->updateQualityStandards("TOOLS", -0.5);
    EXPECT_DOUBLE_EQ(qualityAssurance->getCategoryQualityStandard("TOOLS"), 0.0);
}

TEST_F(QualityAssuranceTest, AddQualityChecklistItem) {
    qualityAssurance->addQualityChecklistItem("New Check");
    qualityAssurance->addQualityChecklistItem("New Check");
    SUCCEED();
}

TEST_F(QualityAssuranceTest, SettersAndGetters) {
    auto newInv = std::make_shared<Inventory>();
    qualityAssurance->setInventory(newInv);
    EXPECT_EQ(qualityAssurance->getInventory(), newInv);
    
    auto newAssessor = std::make_shared<DamageAssessor>();
    qualityAssurance->setDamageAssessor(newAssessor);
    EXPECT_EQ(qualityAssurance->getDamageAssessor(), newAssessor);
}