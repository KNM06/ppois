#include <gtest/gtest.h>
#include "../../include/business/EmployeeManager.h"
#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/business/ReservationSystem.h"

class EmployeeManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto maintenanceScheduler = std::make_shared<MaintenanceScheduler>();
        auto reservationSystem = std::make_shared<ReservationSystem>();
        employeeManager = std::make_shared<EmployeeManager>(maintenanceScheduler, reservationSystem);
        
        Employee employee;
        employee.employeeId = "EMP001";
        employee.name = "John Smith";
        employee.role = "Technician";
        employee.skills = {"ELECTRICAL", "MECHANICAL"};
        employee.efficiencyRating = 0.85;
        employee.assignedTasksCount = 0;
        employeeManager->addEmployee(employee);
    }
    
    std::shared_ptr<EmployeeManager> employeeManager;
};

TEST_F(EmployeeManagerTest, AddEmployee) {
    Employee newEmployee;
    newEmployee.employeeId = "EMP002";
    newEmployee.name = "Jane Doe";
    employeeManager->addEmployee(newEmployee);
    
    auto qualified = employeeManager->findQualifiedEmployees("ELECTRICAL");
    EXPECT_GE(qualified.size(), 1);
}

TEST_F(EmployeeManagerTest, AssignTaskToEmployee) {
    bool success = employeeManager->assignTaskToEmployee("TASK001", "EMP001");
    EXPECT_TRUE(success);
}

TEST_F(EmployeeManagerTest, FindQualifiedEmployees) {
    auto qualified = employeeManager->findQualifiedEmployees("ELECTRICAL");
    EXPECT_FALSE(qualified.empty());
}

TEST_F(EmployeeManagerTest, CalculateTeamEfficiency) {
    double efficiency = employeeManager->calculateTeamEfficiency();
    EXPECT_GE(efficiency, 0.0);
    EXPECT_LE(efficiency, 100.0);
}

TEST_F(EmployeeManagerTest, GetEmployeeWorkload) {
    std::string workload = employeeManager->getEmployeeWorkload("EMP001");
    EXPECT_FALSE(workload.empty());
}