#ifndef MAINTENANCEMANAGER_H
#define MAINTENANCEMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "../../include/maintenance/MaintenanceRequest.h"
#include "../../include/core/Property.h"

class MaintenanceManager {
private:
    std::vector<std::shared_ptr<MaintenanceRequest>> maintenanceRequests;
    std::unordered_map<std::string, std::vector<std::shared_ptr<MaintenanceRequest>>> propertyMaintenanceHistory;
    std::unordered_map<std::string, double> vendorRatings;
    double budgetThreshold;
    int maxConcurrentRequests;
    std::chrono::system_clock::time_point lastInspectionDate;

public:
    MaintenanceManager(double budgetThreshold, int maxConcurrent,
                      const std::chrono::system_clock::time_point& lastInspection);
    
    MaintenanceManager(const MaintenanceManager& other);
    MaintenanceManager& operator=(const MaintenanceManager& other);
    MaintenanceManager(MaintenanceManager&& other) noexcept;
    MaintenanceManager& operator=(MaintenanceManager&& other) noexcept;
    ~MaintenanceManager();

    double getBudgetThreshold() const;
    int getMaxConcurrentRequests() const;
    std::chrono::system_clock::time_point getLastInspectionDate() const;

    void setBudgetThreshold(double newThreshold);
    void setMaxConcurrentRequests(int newMax);
    void updateLastInspectionDate(const std::chrono::system_clock::time_point& newDate);

    bool createMaintenanceRequest(std::shared_ptr<Property> property,
                                 std::shared_ptr<Tenant> tenant,
                                 const std::string& description,
                                 const std::string& category,
                                 const std::string& priority);
    bool scheduleMaintenance(const std::string& requestId,
                            const std::chrono::system_clock::time_point& scheduledDate,
                            const std::string& vendor);
    bool completeMaintenance(const std::string& requestId, double actualCost);
    std::vector<std::shared_ptr<MaintenanceRequest>> getPendingRequests() const;
    std::vector<std::shared_ptr<MaintenanceRequest>> getEmergencyRequests() const;
    double calculateMaintenanceCosts(const std::chrono::system_clock::time_point& startDate,
                                    const std::chrono::system_clock::time_point& endDate) const;
    bool isPropertyUnderMaintenance(const std::string& propertyId) const;
    double calculateMaintenanceBudget(double propertyValue) const;
    bool needsRoutineInspection(const std::chrono::system_clock::time_point& currentDate) const;
    void rateVendor(const std::string& vendor, double rating);
    double getVendorRating(const std::string& vendor) const;
    std::vector<std::shared_ptr<MaintenanceRequest>> getOverdueMaintenance(const std::chrono::system_clock::time_point& currentDate) const;
};

#endif