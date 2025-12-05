#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include "../business/MaintenanceScheduler.h"
#include "../business/ReservationSystem.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

struct Employee {
    std::string employeeId;
    std::string name;
    std::string role;
    std::vector<std::string> skills;
    double efficiencyRating;
    int assignedTasksCount;
};

class EmployeeManager {
private:
    std::shared_ptr<MaintenanceScheduler> maintenanceScheduler;
    std::shared_ptr<ReservationSystem> reservationSystem;
    std::unordered_map<std::string, Employee> employees;
    std::unordered_map<std::string, std::string> taskAssignments;

public:
    EmployeeManager(std::shared_ptr<MaintenanceScheduler> maintSched, std::shared_ptr<ReservationSystem> resSys);
    
    void addEmployee(const Employee& employee);
    bool assignTaskToEmployee(const std::string& taskId, const std::string& employeeId);
    std::vector<std::string> findQualifiedEmployees(const std::string& skillRequired) const;
    double calculateTeamEfficiency() const;
    std::string getEmployeeWorkload(const std::string& employeeId) const;
    void updateEmployeePerformance(const std::string& employeeId, double completedTasks);
};

#endif