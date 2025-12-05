#include "../../include/business/EmployeeManager.h"

EmployeeManager::EmployeeManager(std::shared_ptr<MaintenanceScheduler> maintSched, std::shared_ptr<ReservationSystem> resSys)
    : maintenanceScheduler(maintSched), reservationSystem(resSys) {}

void EmployeeManager::addEmployee(const Employee& employee) {
    employees[employee.employeeId] = employee;
}

bool EmployeeManager::assignTaskToEmployee(const std::string& taskId, const std::string& employeeId) {
    if (employees.find(employeeId) == employees.end()) {
        return false;
    }
    
    taskAssignments[taskId] = employeeId;
    employees[employeeId].assignedTasksCount++;
    return true;
}

std::vector<std::string> EmployeeManager::findQualifiedEmployees(const std::string& skillRequired) const {
    std::vector<std::string> qualifiedEmployees;
    
    for (const auto& [employeeId, employee] : employees) {
        for (const auto& skill : employee.skills) {
            if (skill == skillRequired) {
                qualifiedEmployees.push_back(employeeId);
                break;
            }
        }
    }
    
    return qualifiedEmployees;
}

double EmployeeManager::calculateTeamEfficiency() const {
    if (employees.empty()) {
        return 0.0;
    }
    
    double totalEfficiency = 0.0;
    int count = 0;
    
    for (const auto& [employeeId, employee] : employees) {
        totalEfficiency += employee.efficiencyRating;
        count++;
    }
    
    double avgEfficiency = totalEfficiency / count;
    double workloadFactor = 1.0;
    
    for (const auto& [employeeId, employee] : employees) {
        if (employee.assignedTasksCount > 5) {
            workloadFactor *= 0.9;
        }
    }
    
    return avgEfficiency * workloadFactor;
}

std::string EmployeeManager::getEmployeeWorkload(const std::string& employeeId) const {
    if (employees.find(employeeId) == employees.end()) {
        return "Employee not found";
    }
    
    const Employee& employee = employees.at(employeeId);
    double workloadPercentage = (static_cast<double>(employee.assignedTasksCount) / 10.0) * 100.0;
    
    if (workloadPercentage > 100.0) {
        return "Overloaded: " + std::to_string(static_cast<int>(workloadPercentage)) + "%";
    } else if (workloadPercentage > 80.0) {
        return "High: " + std::to_string(static_cast<int>(workloadPercentage)) + "%";
    } else if (workloadPercentage > 50.0) {
        return "Moderate: " + std::to_string(static_cast<int>(workloadPercentage)) + "%";
    } else {
        return "Low: " + std::to_string(static_cast<int>(workloadPercentage)) + "%";
    }
}

void EmployeeManager::updateEmployeePerformance(const std::string& employeeId, double completedTasks) {
    if (employees.find(employeeId) != employees.end()) {
        Employee& employee = employees[employeeId];
        double successRate = completedTasks / std::max(1, employee.assignedTasksCount);
        employee.efficiencyRating = successRate * 0.8 + employee.efficiencyRating * 0.2;
        employee.assignedTasksCount = 0;
    }
}