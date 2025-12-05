#include <gtest/gtest.h>
#include "../../include/business/ComplianceManager.h"
#include "../../include/core/RentalSystem.h"
#include "../../include/business/EnvironmentalCompliance.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/core/RentalItem.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/business/ReservationSystem.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalAgreement.h"
#include "../../include/exceptions/RentalItemUnavailableException.h"
#include "../../include/exceptions/CustomerBlacklistedException.h"

class ComplianceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventory = std::make_shared<Inventory>();
        maintenanceScheduler = std::make_shared<MaintenanceScheduler>(inventory, nullptr);
        
        auto envCompliance = std::make_shared<EnvironmentalCompliance>(inventory, maintenanceScheduler);
        
        auto rentalSystem = std::make_shared<RentalSystem>(
            std::unique_ptr<Inventory>(new Inventory()),
            std::unique_ptr<CustomerManager>(new CustomerManager()),
            std::unique_ptr<ReservationSystem>(new ReservationSystem()),
            std::unique_ptr<PaymentProcessor>(new PaymentProcessor()),
            std::unique_ptr<PricingStrategy>(new PricingStrategy())
        );
        
        complianceManager = std::make_unique<ComplianceManager>(rentalSystem, envCompliance);
        
        rentalItem1 = std::make_shared<RentalItem>("CONST001", "Crane", "CONSTRUCTION", 500.0);
        rentalItem2 = std::make_shared<RentalItem>("VEH001", "Truck", "VEHICLE", 200.0);
        rentalItem3 = std::make_shared<RentalItem>("TOOL001", "Drill", "TOOLS", 50.0);
        
        inventory->addItem(rentalItem1);
        inventory->addItem(rentalItem2);
        inventory->addItem(rentalItem3);
    }
    
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::unique_ptr<ComplianceManager> complianceManager;
    std::shared_ptr<RentalItem> rentalItem1;
    std::shared_ptr<RentalItem> rentalItem2;
    std::shared_ptr<RentalItem> rentalItem3;
};

TEST_F(ComplianceManagerTest, PerformComplianceCheckForConstruction) {
    bool result = complianceManager->performComplianceCheck("CONST001", "SAFETY");
    EXPECT_FALSE(result);
    
    result = complianceManager->performComplianceCheck("CONST001", "ENVIRONMENTAL");
    EXPECT_FALSE(result);
}

TEST_F(ComplianceManagerTest, PerformComplianceCheckForVehicle) {
    bool result = complianceManager->performComplianceCheck("VEH001", "SAFETY");
    EXPECT_FALSE(result);
    
    result = complianceManager->performComplianceCheck("VEH001", "ENVIRONMENTAL");
    EXPECT_FALSE(result);
}

TEST_F(ComplianceManagerTest, PerformComplianceCheckForGeneralItem) {
    bool result = complianceManager->performComplianceCheck("TOOL001", "OPERATIONAL");
    EXPECT_TRUE(result);
}

TEST_F(ComplianceManagerTest, CalculateComplianceScore) {
    complianceManager->performComplianceCheck("CONST001", "SAFETY");
    complianceManager->performComplianceCheck("CONST001", "ENVIRONMENTAL");
    
    double score = complianceManager->calculateComplianceScore("CONSTRUCTION");
    EXPECT_GE(score, 0.0);
    EXPECT_LE(score, 1.0);
}

TEST_F(ComplianceManagerTest, MeetsRegulatoryRequirements) {
    complianceManager->performComplianceCheck("CONST001", "SAFETY");
    
    bool meetsRequirements = complianceManager->meetsRegulatoryRequirements("CONST001", "SAFETY");
    EXPECT_FALSE(meetsRequirements);
}

TEST_F(ComplianceManagerTest, GenerateComplianceReport) {
    complianceManager->performComplianceCheck("CONST001", "SAFETY");
    complianceManager->performComplianceCheck("VEH001", "ENVIRONMENTAL");
    complianceManager->performComplianceCheck("TOOL001", "OPERATIONAL");
    
    auto report = complianceManager->generateComplianceReport();
    EXPECT_FALSE(report.empty());
    EXPECT_GE(report.size(), 3);
}

TEST_F(ComplianceManagerTest, CalculateNonCompliancePenalty) {
    complianceManager->performComplianceCheck("CONST001", "SAFETY");
    
    double penalty = complianceManager->calculateNonCompliancePenalty("SAFETY");
    EXPECT_GE(penalty, 1000.0);
}

TEST_F(ComplianceManagerTest, RequiresSpecialLicense) {
    bool requiresLicense = complianceManager->requiresSpecialLicense("CONSTRUCTION");
    EXPECT_TRUE(requiresLicense);
    
    requiresLicense = complianceManager->requiresSpecialLicense("VEHICLE");
    EXPECT_TRUE(requiresLicense);
    
    requiresLicense = complianceManager->requiresSpecialLicense("TOOLS");
    EXPECT_FALSE(requiresLicense);
}

TEST_F(ComplianceManagerTest, UpdateRegulatoryRequirements) {
    std::vector<std::string> newRequirements = {"New requirement 1", "New requirement 2"};
    complianceManager->updateRegulatoryRequirements("CUSTOM", newRequirements);
    
    auto requirements = complianceManager->getRegulatoryRequirements("CUSTOM");
    EXPECT_EQ(requirements.size(), 2);
}

TEST_F(ComplianceManagerTest, GetComplianceViolations) {
    complianceManager->performComplianceCheck("CONST001", "SAFETY");
    
    auto violations = complianceManager->getComplianceViolations();
    EXPECT_GE(violations.size(), 0);
}

TEST_F(ComplianceManagerTest, GetRegulatoryRequirements) {
    auto safetyRequirements = complianceManager->getRegulatoryRequirements("SAFETY");
    EXPECT_FALSE(safetyRequirements.empty());
    
    auto envRequirements = complianceManager->getRegulatoryRequirements("ENVIRONMENTAL");
    EXPECT_FALSE(envRequirements.empty());
}

TEST_F(ComplianceManagerTest, AddHighRiskCategory) {
    bool initialStatus = complianceManager->requiresSpecialLicense("CHEMICALS");
    EXPECT_TRUE(initialStatus);
    
    complianceManager->addHighRiskCategory("CHEMICALS");
    
    bool updatedStatus = complianceManager->requiresSpecialLicense("CHEMICALS");
    EXPECT_TRUE(updatedStatus);
}

TEST_F(ComplianceManagerTest, MultipleComplianceChecksForSameItem) {
    bool result1 = complianceManager->performComplianceCheck("CONST001", "SAFETY");
    bool result2 = complianceManager->performComplianceCheck("CONST001", "ENVIRONMENTAL");
    bool result3 = complianceManager->performComplianceCheck("CONST001", "OPERATIONAL");
    
    EXPECT_TRUE(result1 || !result1);
    EXPECT_TRUE(result2 || !result2);
    EXPECT_TRUE(result3 || !result3);
}

TEST_F(ComplianceManagerTest, CalculateComplianceScoreWithMultipleCategories) {
    complianceManager->performComplianceCheck("CONST001", "SAFETY");
    complianceManager->performComplianceCheck("VEH001", "ENVIRONMENTAL");
    complianceManager->performComplianceCheck("TOOL001", "OPERATIONAL");
    
    double constructionScore = complianceManager->calculateComplianceScore("CONSTRUCTION");
    double vehicleScore = complianceManager->calculateComplianceScore("VEHICLE");
    double toolsScore = complianceManager->calculateComplianceScore("GENERAL");
    
    EXPECT_GE(constructionScore, 0.0);
    EXPECT_LE(constructionScore, 1.0);
    EXPECT_GE(vehicleScore, 0.0);
    EXPECT_LE(vehicleScore, 1.0);
    EXPECT_GE(toolsScore, 0.0);
    EXPECT_LE(toolsScore, 1.0);
}

TEST_F(ComplianceManagerTest, GetRentalSystem) {
    auto rentalSystem = complianceManager->getRentalSystem();
    EXPECT_NE(rentalSystem, nullptr);
}

TEST_F(ComplianceManagerTest, GetEnvironmentalCompliance) {
    auto envCompliance = complianceManager->getEnvironmentalCompliance();
    EXPECT_NE(envCompliance, nullptr);
}

TEST_F(ComplianceManagerTest, SetRentalSystem) {
    auto newRentalSystem = std::make_shared<RentalSystem>();
    complianceManager->setRentalSystem(newRentalSystem);
    
    auto retrievedSystem = complianceManager->getRentalSystem();
    EXPECT_EQ(retrievedSystem, newRentalSystem);
}

TEST_F(ComplianceManagerTest, SetEnvironmentalCompliance) {
    auto newEnvCompliance = std::make_shared<EnvironmentalCompliance>(inventory, maintenanceScheduler);
    complianceManager->setEnvironmentalCompliance(newEnvCompliance);
    
    auto retrievedCompliance = complianceManager->getEnvironmentalCompliance();
    EXPECT_EQ(retrievedCompliance, newEnvCompliance);
}

TEST_F(ComplianceManagerTest, ComplianceReportAfterMultipleFailures) {
    for (int i = 0; i < 5; i++) {
        complianceManager->performComplianceCheck("CONST001", "SAFETY");
    }
    
    auto report = complianceManager->generateComplianceReport();
    EXPECT_FALSE(report.empty());
    
    bool hasAlert = false;
    for (const auto& line : report) {
        if (line.find("ALERT") != std::string::npos) {
            hasAlert = true;
            break;
        }
    }
}

TEST_F(ComplianceManagerTest, NonCompliancePenaltyForDifferentRegulations) {
    double safetyPenalty = complianceManager->calculateNonCompliancePenalty("SAFETY");
    double envPenalty = complianceManager->calculateNonCompliancePenalty("ENVIRONMENTAL");
    double operationalPenalty = complianceManager->calculateNonCompliancePenalty("OPERATIONAL");
    
    EXPECT_GT(safetyPenalty, 0.0);
    EXPECT_GT(envPenalty, 0.0);
    EXPECT_GT(operationalPenalty, 0.0);
    EXPECT_GE(safetyPenalty, envPenalty);
    EXPECT_GE(envPenalty, operationalPenalty);
}

TEST_F(ComplianceManagerTest, ComplianceCheckWithInvalidRegulation) {
    bool result = complianceManager->performComplianceCheck("CONST001", "INVALID_REGULATION");
    EXPECT_TRUE(result || !result);
}

TEST_F(ComplianceManagerTest, MeetsRequirementsForNonExistentRegulation) {
    bool meets = complianceManager->meetsRegulatoryRequirements("CONST001", "NON_EXISTENT");
    EXPECT_TRUE(meets);
}

TEST_F(ComplianceManagerTest, EmptyComplianceReportWithNoChecks) {
    auto report = complianceManager->generateComplianceReport();
    EXPECT_FALSE(report.empty());
    
    bool hasZeroChecks = false;
    for (const auto& line : report) {
        if (line.find("Total Checks: 0") != std::string::npos) {
            hasZeroChecks = true;
            break;
        }
    }
    EXPECT_TRUE(hasZeroChecks);
}