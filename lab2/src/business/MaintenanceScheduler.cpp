#include "../../include/business/MaintenanceScheduler.h"
#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/utils/DateUtils.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <ctime>

MaintenanceScheduler::MaintenanceScheduler() 
    : inventory(nullptr), damageAssessor(nullptr) {
    
    categoryMaintenanceCosts["VEHICLE"] = 150.0;
    categoryMaintenanceCosts["ELECTRONICS"] = 100.0;
    categoryMaintenanceCosts["CONSTRUCTION"] = 200.0;
    categoryMaintenanceCosts["TOOLS"] = 80.0;
    categoryMaintenanceCosts["SPORTS"] = 60.0;
    
    maintenanceTeams["VEHICLE"] = {"MECH001", "MECH002"};
    maintenanceTeams["ELECTRONICS"] = {"TECH001", "TECH002"};
    maintenanceTeams["CONSTRUCTION"] = {"ENG001", "ENG002"};
}

MaintenanceScheduler::MaintenanceScheduler(std::shared_ptr<Inventory> inv, std::shared_ptr<DamageAssessor> assessor)
    : inventory(inv), damageAssessor(assessor) {
    
    categoryMaintenanceCosts["VEHICLE"] = 150.0;
    categoryMaintenanceCosts["ELECTRONICS"] = 100.0;
    categoryMaintenanceCosts["CONSTRUCTION"] = 200.0;
    categoryMaintenanceCosts["TOOLS"] = 80.0;
    categoryMaintenanceCosts["SPORTS"] = 60.0;
    
    maintenanceTeams["VEHICLE"] = {"MECH001", "MECH002"};
    maintenanceTeams["ELECTRONICS"] = {"TECH001", "TECH002"};
    maintenanceTeams["CONSTRUCTION"] = {"ENG001", "ENG002"};
}

bool MaintenanceScheduler::scheduleMaintenance(const std::string& itemId, const std::string& maintenanceType) {
    if (!inventory) {
        return false;
    }
    
    MaintenanceTask task;
    task.taskId = "MT" + std::to_string(maintenanceTasks.size() + 1000);
    task.itemId = itemId;
    task.maintenanceType = maintenanceType;
    task.scheduledDate = DateUtils::addDaysToDate(DateUtils::getCurrentDate(), 7);
    task.cost = calculateMaintenanceCost(itemId, maintenanceType);
    task.estimatedDurationHours = 4;
    task.isCompleted = false;
    
    auto teams = getMaintenanceTeam("GENERAL");
    if (!teams.empty()) {
        task.technicianId = teams[0];
    }
    
    maintenanceTasks[task.taskId] = task;
    return true;
}

double MaintenanceScheduler::calculateMaintenanceCost(const std::string& itemId, const std::string& maintenanceType) const {
    double baseCost = 50.0;
    
    if (maintenanceType == "PREVENTIVE") {
        baseCost = 75.0;
    } else if (maintenanceType == "CORRECTIVE") {
        baseCost = 120.0;
    } else if (maintenanceType == "EMERGENCY") {
        baseCost = 200.0;
    }
    
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    else if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    
    auto categoryIt = categoryMaintenanceCosts.find(category);
    if (categoryIt != categoryMaintenanceCosts.end()) {
        baseCost = categoryIt->second;
    }
    
    int maintenanceCount = getMaintenanceCount(itemId);
    double usageMultiplier = 1.0 + (maintenanceCount * 0.1);
    
    return baseCost * usageMultiplier;
}

int MaintenanceScheduler::predictNextMaintenance(const std::string& itemId) const {
    int baseInterval = 90;
    
    int maintenanceCount = getMaintenanceCount(itemId);
    if (maintenanceCount > 5) {
        baseInterval = 60;
    } else if (maintenanceCount > 10) {
        baseInterval = 45;
    }
    
    std::string category = "GENERAL";
    if (itemId.find("CONST") != std::string::npos) {
        baseInterval = 30;
    } else if (itemId.find("VEH") != std::string::npos) {
        baseInterval = 60;
    }
    
    return baseInterval;
}

double MaintenanceScheduler::calculateEquipmentDowntime(const std::string& itemId) const {
    double baseDowntime = 24.0;
    
    std::string category = "GENERAL";
    if (itemId.find("CONST") != std::string::npos) {
        baseDowntime = 48.0;
    } else if (itemId.find("ELEC") != std::string::npos) {
        baseDowntime = 12.0;
    }
    
    int maintenanceCount = getMaintenanceCount(itemId);
    double efficiencyMultiplier = 1.0 - (maintenanceCount * 0.05);
    
    return baseDowntime * std::max(0.5, efficiencyMultiplier);
}

void MaintenanceScheduler::optimizeMaintenanceSchedule() {
    for (auto& [taskId, task] : maintenanceTasks) {
        if (!task.isCompleted) {
            double currentCost = task.cost;
            double optimizedCost = currentCost * 0.9;
            task.cost = optimizedCost;
            
            if (task.estimatedDurationHours > 8) {
                task.estimatedDurationHours = static_cast<int>(task.estimatedDurationHours * 0.8);
            }
        }
    }
}

bool MaintenanceScheduler::completeMaintenanceTask(const std::string& taskId, double actualCost) {
    auto taskIt = maintenanceTasks.find(taskId);
    if (taskIt != maintenanceTasks.end() && !taskIt->second.isCompleted) {
        taskIt->second.isCompleted = true;
        taskIt->second.completedDate = DateUtils::getCurrentDate();
        taskIt->second.cost = actualCost;
        
        maintenanceHistory[taskIt->second.itemId]++;
        return true;
    }
    return false;
}

double MaintenanceScheduler::calculateMaintenanceBudget(int monthsAhead) const {
    double monthlyBudget = 1000.0;
    
    int scheduledTasks = 0;
    for (const auto& [taskId, task] : maintenanceTasks) {
        if (!task.isCompleted) {
            scheduledTasks++;
        }
    }
    
    double budget = monthlyBudget * monthsAhead;
    budget += scheduledTasks * 50.0;
    
    if (inventory) {
        double inventoryValue = inventory->calculateTotalInventoryValue();
        budget += inventoryValue * 0.02;
    }
    
    return budget;
}

std::vector<std::string> MaintenanceScheduler::getOverdueMaintenance() const {
    std::vector<std::string> overdueTasks;
    
    std::string currentDate = "2024-01-20";
    
    for (const auto& [taskId, task] : maintenanceTasks) {
        if (!task.isCompleted && task.scheduledDate < currentDate) {
            overdueTasks.push_back(taskId);
        }
    }
    
    return overdueTasks;
}

double MaintenanceScheduler::calculatePreventiveMaintenanceSavings(const std::string& itemId) const {
    auto historyIt = maintenanceHistory.find(itemId);
    if (historyIt == maintenanceHistory.end()) {
        return 0.0;
    }
    
    int maintenanceCount = historyIt->second;
    double baseSavings = 500.0;
    
    double maintenanceMultiplier = 1.0 + (maintenanceCount * 0.1);
    double lifespanExtensionSavings = 200.0 * maintenanceCount;
    double downtimeSavings = calculateEquipmentDowntime(itemId) * 50.0;
    
    return (baseSavings * maintenanceMultiplier) + lifespanExtensionSavings + downtimeSavings;
}

std::vector<std::string> MaintenanceScheduler::getMaintenanceTeam(const std::string& category) const {
    auto it = maintenanceTeams.find(category);
    if (it != maintenanceTeams.end()) {
        return it->second;
    }
    return {"GEN001"};
}

double MaintenanceScheduler::calculateMaintenanceEfficiency(const std::string& teamId) const {
    int completedTasks = 0;
    double totalCost = 0.0;
    
    for (const auto& [taskId, task] : maintenanceTasks) {
        if (task.technicianId == teamId && task.isCompleted) {
            completedTasks++;
            totalCost += task.cost;
        }
    }
    
    if (completedTasks == 0) return 0.0;
    
    double averageCost = totalCost / completedTasks;
    double efficiency = 1000.0 / (averageCost + 1.0);
    
    return std::min(100.0, efficiency);
}

int MaintenanceScheduler::getMaintenanceCount(const std::string& itemId) const {
    auto it = maintenanceHistory.find(itemId);
    return (it != maintenanceHistory.end()) ? it->second : 0;
}

std::shared_ptr<Inventory> MaintenanceScheduler::getInventory() const {
    return inventory;
}

std::shared_ptr<DamageAssessor> MaintenanceScheduler::getDamageAssessor() const {
    return damageAssessor;
}

void MaintenanceScheduler::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void MaintenanceScheduler::setDamageAssessor(std::shared_ptr<DamageAssessor> assessor) {
    damageAssessor = assessor;
}

void MaintenanceScheduler::assignMaintenanceTeam(const std::string& category, const std::vector<std::string>& team) {
    maintenanceTeams[category] = team;
}