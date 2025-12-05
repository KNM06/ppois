#ifndef MAINTENANCESCHEDULE_H
#define MAINTENANCESCHEDULE_H

#include <string>
#include <memory>
#include <chrono>
#include <vector>
#include "../../include/core/Property.h"
#include "../../include/maintenance/MaintenanceTask.h"

class MaintenanceSchedule {
private:
    std::string scheduleId;
    std::shared_ptr<Property> property;
    std::string scheduleType;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point endDate;
    std::string frequency;
    std::vector<std::shared_ptr<MaintenanceTask>> scheduledTasks;
    double budgetAllocated;
    double budgetUsed;
    std::string status;
    std::string assignedTeam;

public:
    MaintenanceSchedule(const std::string& id, std::shared_ptr<Property> prop,
                       const std::string& type, const std::chrono::system_clock::time_point& start,
                       const std::chrono::system_clock::time_point& end, const std::string& frequency,
                       const std::vector<std::shared_ptr<MaintenanceTask>>& tasks,
                       double allocated, double used, const std::string& status,
                       const std::string& team);
    
    MaintenanceSchedule(const MaintenanceSchedule& other);
    MaintenanceSchedule& operator=(const MaintenanceSchedule& other);
    MaintenanceSchedule(MaintenanceSchedule&& other) noexcept;
    MaintenanceSchedule& operator=(MaintenanceSchedule&& other) noexcept;
    ~MaintenanceSchedule();

    std::string getScheduleId() const;
    Property* getProperty() const;
    std::string getScheduleType() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEndDate() const;
    std::string getFrequency() const;
    std::vector<std::shared_ptr<MaintenanceTask>> getScheduledTasks() const;
    double getBudgetAllocated() const;
    double getBudgetUsed() const;
    std::string getStatus() const;
    std::string getAssignedTeam() const;

    void setStatus(const std::string& newStatus);
    void setBudgetUsed(double newUsed);
    void setAssignedTeam(const std::string& newTeam);
    void addTask(std::shared_ptr<MaintenanceTask> task);

    bool isActive(const std::chrono::system_clock::time_point& currentDate) const;
    double calculateBudgetRemaining() const;
    bool isOverBudget() const;
    int getCompletedTaskCount() const;
    double calculateCompletionPercentage() const;
    bool requiresBudgetAdjustment() const;
    bool hasOverdueTasks(const std::chrono::system_clock::time_point& currentDate) const;
    double calculateAverageTaskDuration() const;
    bool isPreventiveMaintenance() const;
    std::vector<std::shared_ptr<MaintenanceTask>> getPendingTasks() const;
};

#endif