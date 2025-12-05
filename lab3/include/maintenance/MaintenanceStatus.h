#ifndef MAINTENANCESTATUS_H
#define MAINTENANCESTATUS_H

#include <string>
#include <memory>
#include <chrono>

class MaintenanceStatus {
private:
    std::string statusId;
    std::string statusCode;
    std::string description;
    bool isActive;
    bool isCompleted;
    bool requiresFollowUp;
    std::chrono::system_clock::time_point statusDate;
    std::string nextAction;
    int estimatedCompletionDays;
    std::string resolutionNotes;

public:
    MaintenanceStatus(const std::string& id, const std::string& code,
                     const std::string& description, bool active, bool completed,
                     bool requiresFollowUp, const std::chrono::system_clock::time_point& statusDate,
                     const std::string& nextAction, int estimatedCompletion,
                     const std::string& resolutionNotes);
    
    MaintenanceStatus(const MaintenanceStatus& other);
    MaintenanceStatus& operator=(const MaintenanceStatus& other);
    MaintenanceStatus(MaintenanceStatus&& other) noexcept;
    MaintenanceStatus& operator=(MaintenanceStatus&& other) noexcept;
    ~MaintenanceStatus();

    std::string getStatusId() const;
    std::string getStatusCode() const;
    std::string getDescription() const;
    bool getIsActive() const;
    bool getIsCompleted() const;
    bool getRequiresFollowUp() const;
    std::chrono::system_clock::time_point getStatusDate() const;
    std::string getNextAction() const;
    int getEstimatedCompletionDays() const;
    std::string getResolutionNotes() const;

    void setStatusCode(const std::string& newCode);
    void setIsCompleted(bool completed);
    void setNextAction(const std::string& action);

    bool isWorkInProgress() const;
    bool isPendingApproval() const;
    bool requiresVendorAttention() const;
    int getDaysInStatus(const std::chrono::system_clock::time_point& currentDate) const;
    bool isDelayed() const;
    bool canBeClosed() const;
    std::string getStatusCategory() const;
    bool requiresCustomerNotification() const;
    double calculateCompletionPercentage() const;
};

#endif