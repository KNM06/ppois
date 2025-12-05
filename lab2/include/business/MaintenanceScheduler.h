#ifndef MAINTENANCESCHEDULER_H
#define MAINTENANCESCHEDULER_H

#include "../../include/core/Inventory.h"
#include "../../include/business/DamageAssessor.h"
#include "../../include/utils/DateUtils.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Inventory;
class DamageAssessor;

struct MaintenanceTask {
    std::string taskId;
    std::string itemId;
    std::string maintenanceType;
    std::string scheduledDate;
    std::string completedDate;
    double cost;
    int estimatedDurationHours;
    std::string technicianId;
    bool isCompleted;
};

class MaintenanceScheduler {
private:
    std::unordered_map<std::string, MaintenanceTask> maintenanceTasks;
    std::unordered_map<std::string, int> maintenanceHistory;
    std::unordered_map<std::string, double> categoryMaintenanceCosts;
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<DamageAssessor> damageAssessor;
    std::unordered_map<std::string, std::vector<std::string>> maintenanceTeams;

public:
    MaintenanceScheduler();
    MaintenanceScheduler(std::shared_ptr<Inventory> inv, std::shared_ptr<DamageAssessor> assessor);
    
    bool scheduleMaintenance(const std::string& itemId, const std::string& maintenanceType);
    double calculateMaintenanceCost(const std::string& itemId, const std::string& maintenanceType) const;
    int predictNextMaintenance(const std::string& itemId) const;
    double calculateEquipmentDowntime(const std::string& itemId) const;
    void optimizeMaintenanceSchedule();
    bool completeMaintenanceTask(const std::string& taskId, double actualCost);
    double calculateMaintenanceBudget(int monthsAhead) const;
    std::vector<std::string> getOverdueMaintenance() const;
    double calculatePreventiveMaintenanceSavings(const std::string& itemId) const;
    std::vector<std::string> getMaintenanceTeam(const std::string& category) const;
    double calculateMaintenanceEfficiency(const std::string& teamId) const;
    
    int getMaintenanceCount(const std::string& itemId) const;
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<DamageAssessor> getDamageAssessor() const;
    
    void setInventory(std::shared_ptr<Inventory> inv);
    void setDamageAssessor(std::shared_ptr<DamageAssessor> assessor);
    void assignMaintenanceTeam(const std::string& category, const std::vector<std::string>& team);
};

#endif