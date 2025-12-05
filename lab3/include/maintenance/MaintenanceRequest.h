#ifndef MAINTENANCEREQUEST_H
#define MAINTENANCEREQUEST_H

#include <string>
#include <memory>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"

class MaintenanceRequest {
private:
    std::string requestId;
    std::shared_ptr<Property> property;
    std::shared_ptr<Tenant> tenant;
    std::string issueDescription;
    std::string category;
    std::string priority;
    std::chrono::system_clock::time_point requestDate;
    std::chrono::system_clock::time_point scheduledDate;
    std::string status;
    double estimatedCost;
    double actualCost;
    std::string assignedVendor;

public:
    MaintenanceRequest();
    MaintenanceRequest(const std::string& id, std::shared_ptr<Property> prop,
                      std::shared_ptr<Tenant> ten, const std::string& description,
                      const std::string& category, const std::string& priority,
                      const std::chrono::system_clock::time_point& requestDate,
                      const std::chrono::system_clock::time_point& scheduledDate,
                      const std::string& status, double estimated, double actual,
                      const std::string& vendor);
    
    MaintenanceRequest(const MaintenanceRequest& other);
    MaintenanceRequest& operator=(const MaintenanceRequest& other);
    MaintenanceRequest(MaintenanceRequest&& other) noexcept;
    MaintenanceRequest& operator=(MaintenanceRequest&& other) noexcept;
    ~MaintenanceRequest();

    std::string getRequestId() const;
    std::shared_ptr<Property> getProperty() const;

    Tenant* getTenant() const;
    std::string getIssueDescription() const;
    std::string getCategory() const;
    std::string getPriority() const;
    std::chrono::system_clock::time_point getRequestDate() const;
    std::chrono::system_clock::time_point getScheduledDate() const;
    std::string getStatus() const;
    double getEstimatedCost() const;
    double getActualCost() const;
    std::string getAssignedVendor() const;

    void setStatus(const std::string& newStatus);
    void setActualCost(double cost);
    void setAssignedVendor(const std::string& vendor);

    bool isEmergencyRequest() const;
    bool isScheduled() const;
    bool isCompleted() const;
    double calculateCostVariance() const;
    int getDaysSinceRequest(const std::chrono::system_clock::time_point& currentDate) const;
    bool requiresImmediateAttention() const;
    bool isWithinBudget() const;
    bool canBeDeferred() const;
};

#endif