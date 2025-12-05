#include <gtest/gtest.h>
#include "../../include/business/TrainingCoordinator.h"
#include "../../include/business/EmployeeManager.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/ReservationSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class TrainingCoordinatorTest : public ::testing::Test {
protected:
    std::shared_ptr<EmployeeManager> employeeManager;
    std::shared_ptr<TrainingCoordinator> trainingCoordinator;

    Employee emp1 = {"EMP001", "Alice", "TECH", {"HEAVY_EQUIPMENT", "SAFETY"}, 0.9, 0};
    Employee emp2 = {"EMP002", "Bob", "SALES", {"CUSTOMER_SERVICE"}, 0.5, 0};
    Employee emp3 = {"EMP003", "Charlie", "MECH", {"SAFETY"}, 0.7, 8};

    void SetUp() override {
        auto maintSched = std::make_shared<MaintenanceScheduler>();
        auto resSys = std::make_shared<ReservationSystem>();
        employeeManager = std::make_shared<EmployeeManager>(maintSched, resSys);
        
        employeeManager->addEmployee(emp1);
        employeeManager->addEmployee(emp2);
        employeeManager->addEmployee(emp3);

        trainingCoordinator = std::make_shared<TrainingCoordinator>(employeeManager);
        
        trainingCoordinator->addEmployeeCertification("EMP001", "ELECTRICAL");
    }
};

TEST_F(TrainingCoordinatorTest, ScheduleTrainingSessionSuccessfully) {
    std::vector<std::string> participants = {"EMP001", "EMP002"};
    bool scheduled = trainingCoordinator->scheduleTrainingSession("SAFETY", participants, "2024-01-15", 8);
    EXPECT_TRUE(scheduled);
    EXPECT_EQ(trainingCoordinator->getTrainingSessionCount(), 1u);
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingCostForKnownType) {
    double cost = trainingCoordinator->calculateTrainingCost("SAFETY", 5);
    EXPECT_NEAR(cost, 180.0, 0.001);
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingCostForUnknownType) {
    double cost = trainingCoordinator->calculateTrainingCost("UNKNOWN", 2);
    EXPECT_NEAR(cost, 75.0, 0.001);
}

TEST_F(TrainingCoordinatorTest, ValidateEmployeeCertificationReturnsTrue) {
    trainingCoordinator->addEmployeeCertification("EMP001", "HEAVY_EQUIPMENT");
    bool valid = trainingCoordinator->validateEmployeeCertification("EMP001", "HEAVY_EQUIPMENT");
    EXPECT_TRUE(valid);
}

TEST_F(TrainingCoordinatorTest, ValidateEmployeeCertificationReturnsFalseForMissingCert) {
    bool valid = trainingCoordinator->validateEmployeeCertification("EMP001", "HEAVY_EQUIPMENT");
    EXPECT_FALSE(valid);
}

TEST_F(TrainingCoordinatorTest, ValidateEmployeeCertificationReturnsFalseForUnknownEmployee) {
    bool valid = trainingCoordinator->validateEmployeeCertification("EMP999", "HEAVY_EQUIPMENT");
    EXPECT_FALSE(valid);
}

TEST_F(TrainingCoordinatorTest, GetRequiredTrainingReturnsAllRequiredForNewEmployee) {
    auto required = trainingCoordinator->getRequiredTraining("EMP002");
    EXPECT_EQ(required.size(), 3u);
    EXPECT_TRUE(std::find(required.begin(), required.end(), "HEAVY_EQUIPMENT") != required.end());
}

TEST_F(TrainingCoordinatorTest, GetRequiredTrainingReturnsMissingCertifications) {
    auto required = trainingCoordinator->getRequiredTraining("EMP001");
    EXPECT_EQ(required.size(), 2u);
    EXPECT_FALSE(std::find(required.begin(), required.end(), "ELECTRICAL") != required.end());
    EXPECT_TRUE(std::find(required.begin(), required.end(), "CONSTRUCTION_SAFETY") != required.end());
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingROIReturnsZeroForNonExistentSession) {
    double roi = trainingCoordinator->calculateTrainingROI("TRAIN9999");
    EXPECT_DOUBLE_EQ(roi, 0.0);
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingROIReturnsZeroForIncompleteSession) {
    std::vector<std::string> participants = {"EMP001"};
    trainingCoordinator->scheduleTrainingSession("SAFETY", participants, "2024-01-15", 8);
    double roi = trainingCoordinator->calculateTrainingROI("TRAIN1000");
    EXPECT_DOUBLE_EQ(roi, 0.0);
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingROIFORSafety) {
    std::vector<std::string> participants = {"EMP001", "EMP002"};
    trainingCoordinator->scheduleTrainingSession("SAFETY", participants, "2024-01-15", 8);
    
    auto sessions = trainingCoordinator->getTrainingSessions();
    auto it = sessions.find("TRAIN1000");
    const_cast<TrainingSession&>(it->second).completed = true;
    
    double roi = trainingCoordinator->calculateTrainingROI("TRAIN1000");
    EXPECT_NEAR(roi, 0, 0.0001);
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingROIFORTechnical) {
    std::vector<std::string> participants = {"EMP001"};
    trainingCoordinator->scheduleTrainingSession("TECHNICAL", participants, "2024-01-15", 8);
    
    auto sessions = trainingCoordinator->getTrainingSessions();
    auto it = sessions.find("TRAIN1000");
    const_cast<TrainingSession&>(it->second).completed = true;
    
    double roi = trainingCoordinator->calculateTrainingROI("TRAIN1000");
    EXPECT_NEAR(roi, 0, 0.0001);
}

TEST_F(TrainingCoordinatorTest, CalculateTrainingROIFOROther) {
    std::vector<std::string> participants = {"EMP001"};
    trainingCoordinator->scheduleTrainingSession("UNKNOWN_TYPE", participants, "2024-01-15", 8);
    
    auto sessions = trainingCoordinator->getTrainingSessions();
    auto it = sessions.find("TRAIN1000");
    const_cast<TrainingSession&>(it->second).completed = true;
    
    double roi = trainingCoordinator->calculateTrainingROI("TRAIN1000");
    EXPECT_NEAR(roi, 0, 0.0001);
}

TEST_F(TrainingCoordinatorTest, IsCertificationRequiredReturnsTrue) {
    EXPECT_TRUE(trainingCoordinator->isCertificationRequired("HEAVY_EQUIPMENT"));
}

TEST_F(TrainingCoordinatorTest, IsCertificationRequiredReturnsFalse) {
    EXPECT_FALSE(trainingCoordinator->isCertificationRequired("BASIC_FIRST_AID"));
}

TEST_F(TrainingCoordinatorTest, GetCertifiedEmployeesReturnsCorrectEmployees) {
    trainingCoordinator->addEmployeeCertification("EMP002", "HEAVY_EQUIPMENT");
    auto certified = trainingCoordinator->getCertifiedEmployees("HEAVY_EQUIPMENT");
    
    EXPECT_EQ(certified.size(), 1u);
    EXPECT_EQ(certified[0], "EMP002");
}

TEST_F(TrainingCoordinatorTest, GetCertifiedEmployeesReturnsEmptyVector) {
    auto certified = trainingCoordinator->getCertifiedEmployees("NON_EXISTENT");
    EXPECT_TRUE(certified.empty());
}

TEST_F(TrainingCoordinatorTest, GetEmployeeCertificationsReturnsCorrectCerts) {
    trainingCoordinator->addEmployeeCertification("EMP001", "FORKLIFT");
    auto certs = trainingCoordinator->getEmployeeCertifications("EMP001");
    
    EXPECT_EQ(certs.size(), 2u);
    EXPECT_TRUE(std::find(certs.begin(), certs.end(), "FORKLIFT") != certs.end());
}

TEST_F(TrainingCoordinatorTest, GetEmployeeCertificationsReturnsEmptyForUnknownEmployee) {
    auto certs = trainingCoordinator->getEmployeeCertifications("EMP999");
    EXPECT_TRUE(certs.empty());
}

TEST_F(TrainingCoordinatorTest, SetEmployeeManagerUpdatesManager) {
    auto newManager = std::make_shared<EmployeeManager>(std::make_shared<MaintenanceScheduler>(), std::make_shared<ReservationSystem>());
    trainingCoordinator->setEmployeeManager(newManager);
    EXPECT_EQ(trainingCoordinator->getEmployeeManager(), newManager);
}

TEST_F(TrainingCoordinatorTest, AddRequiredCertificationAddsNewCert) {
    trainingCoordinator->addRequiredCertification("FIRST_AID");
    EXPECT_TRUE(trainingCoordinator->isCertificationRequired("FIRST_AID"));
    
    trainingCoordinator->addRequiredCertification("ELECTRICAL");
    auto required = trainingCoordinator->getRequiredTraining("EMP002");
    EXPECT_EQ(required.size(), 4u);
}

TEST_F(TrainingCoordinatorTest, SetTrainingCostUpdatesCost) {
    trainingCoordinator->setTrainingCost("SAFETY", 500.0);
    double newCost = trainingCoordinator->calculateTrainingCost("SAFETY", 2);
    EXPECT_NEAR(newCost, 750.0, 0.001);
}