#ifndef MAINTENANCETASK_H
#define MAINTENANCETASK_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/maintenance/MaintenanceRequest.h"

class MaintenanceTask {
private:
    std::string taskId;
    std::shared_ptr<MaintenanceRequest> request;
    std::string taskDescription;
    std::string assignedTo;
    std::chrono::system_clock::time_point startDate;
    std::chrono::system_clock::time_point estimatedCompletion;
    std::chrono::system_clock::time_point actualCompletion;
    std::string status;
    double estimatedHours;
    double actualHours;
    std::string priority;
    std::vector<std::string> requiredMaterials;

public:
    MaintenanceTask(const std::string& id, std::shared_ptr<MaintenanceRequest> req,
                   const std::string& description, const std::string& assignedTo,
                   const std::chrono::system_clock::time_point& start,
                   const std::chrono::system_clock::time_point& estimatedComplete,
                   const std::chrono::system_clock::time_point& actualComplete,
                   const std::string& status, double estimatedHours,
                   double actualHours, const std::string& priority,
                   const std::vector<std::string>& materials);
    
    MaintenanceTask(const MaintenanceTask& other);
    MaintenanceTask& operator=(const MaintenanceTask& other);
    MaintenanceTask(MaintenanceTask&& other) noexcept;
    MaintenanceTask& operator=(MaintenanceTask&& other) noexcept;
    ~MaintenanceTask();

    std::string getTaskId() const;
    MaintenanceRequest* getRequest() const;
    std::string getTaskDescription() const;
    std::string getAssignedTo() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEstimatedCompletion() const;
    std::chrono::system_clock::time_point getActualCompletion() const;
    std::string getStatus() const;
    double getEstimatedHours() const;
    double getActualHours() const;
    std::string getPriority() const;
    std::vector<std::string> getRequiredMaterials() const;

    void setStatus(const std::string& newStatus);
    void setAssignedTo(const std::string& newAssignee);
    void setActualCompletion(const std::chrono::system_clock::time_point& completion);
    void setActualHours(double hours);

    bool isOverdue(const std::chrono::system_clock::time_point& currentDate) const;
    double calculateTimeVariance() const;
    bool isCompleted() const;
    bool requiresSpecializedTools() const;
    double calculateLaborCost(double hourlyRate) const;
    bool canBeDelegated() const;
    int getDaysToComplete() const;
    bool isHighPriority() const;
    double calculateEfficiency() const;
};

#endif