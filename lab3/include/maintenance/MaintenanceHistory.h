#ifndef MAINTENANCEHISTORY_H
#define MAINTENANCEHISTORY_H

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/maintenance/MaintenanceRequest.h"

class MaintenanceHistory {
private:
    std::string historyId;
    std::shared_ptr<Property> property;
    std::vector<std::shared_ptr<MaintenanceRequest>> maintenanceRequests;
    std::chrono::system_clock::time_point lastMaintenanceDate;
    std::chrono::system_clock::time_point nextScheduledMaintenance;
    double totalMaintenanceCost;
    int totalRequests;
    std::string maintenanceFrequency;
    std::string overallCondition;

public:
    MaintenanceHistory(const std::string& id, std::shared_ptr<Property> prop,
                      const std::vector<std::shared_ptr<MaintenanceRequest>>& requests,
                      const std::chrono::system_clock::time_point& lastMaintenance,
                      const std::chrono::system_clock::time_point& nextScheduled,
                      double totalCost, int totalRequests,
                      const std::string& frequency, const std::string& condition);
    
    MaintenanceHistory(const MaintenanceHistory& other);
    MaintenanceHistory& operator=(const MaintenanceHistory& other);
    MaintenanceHistory(MaintenanceHistory&& other) noexcept;
    MaintenanceHistory& operator=(MaintenanceHistory&& other) noexcept;
    ~MaintenanceHistory();

    std::string getHistoryId() const;
    Property* getProperty() const;
    std::vector<std::shared_ptr<MaintenanceRequest>> getMaintenanceRequests() const;
    std::chrono::system_clock::time_point getLastMaintenanceDate() const;
    std::chrono::system_clock::time_point getNextScheduledMaintenance() const;
    double getTotalMaintenanceCost() const;
    int getTotalRequests() const;
    std::string getMaintenanceFrequency() const;
    std::string getOverallCondition() const;

    void setLastMaintenanceDate(const std::chrono::system_clock::time_point& newDate);
    void setNextScheduledMaintenance(const std::chrono::system_clock::time_point& newDate);
    void setOverallCondition(const std::string& newCondition);

    double calculateAverageMaintenanceCost() const;
    bool isMaintenanceOverdue(const std::chrono::system_clock::time_point& currentDate) const;
    int getDaysSinceLastMaintenance(const std::chrono::system_clock::time_point& currentDate) const;
    double calculateMaintenanceCostPerSqFt() const;
    bool hasFrequentIssues() const;
    std::vector<std::shared_ptr<MaintenanceRequest>> getEmergencyRequests() const;
    double calculatePreventiveMaintenanceRatio() const;
    std::string getMaintenanceEfficiency() const;
    bool requiresMajorRenovation() const;
};

#endif