#ifndef MAINTENANCEPRIORITY_H
#define MAINTENANCEPRIORITY_H

#include <string>
#include <memory>

class MaintenancePriority {
private:
    std::string priorityId;
    std::string priorityLevel;
    std::string description;
    int responseTimeHours;
    int resolutionTimeDays;
    double costMultiplier;
    bool requiresEmergencyContact;
    std::string escalationProcedure;
    std::string affectedSystems;

public:
    MaintenancePriority(const std::string& id, const std::string& level,
                       const std::string& description, int responseTime,
                       int resolutionTime, double costMultiplier,
                       bool emergencyContact, const std::string& escalation,
                       const std::string& affectedSystems);
    
    MaintenancePriority(const MaintenancePriority& other);
    MaintenancePriority& operator=(const MaintenancePriority& other);
    MaintenancePriority(MaintenancePriority&& other) noexcept;
    MaintenancePriority& operator=(MaintenancePriority&& other) noexcept;
    ~MaintenancePriority();

    std::string getPriorityId() const;
    std::string getPriorityLevel() const;
    std::string getDescription() const;
    int getResponseTimeHours() const;
    int getResolutionTimeDays() const;
    double getCostMultiplier() const;
    bool getRequiresEmergencyContact() const;
    std::string getEscalationProcedure() const;
    std::string getAffectedSystems() const;

    void setResponseTimeHours(int newTime);
    void setResolutionTimeDays(int newTime);
    void setCostMultiplier(double newMultiplier);

    bool isEmergencyPriority() const;
    bool isHighPriority() const;
    double calculateAdjustedCost(double baseCost) const;
    bool requiresImmediateAttention() const;
    bool canBeScheduled() const;
    bool affectsCriticalSystems() const;
    int calculateSLACompliance(int actualResponseHours) const;
    std::string getRiskLevel() const;
    bool requiresManagementApproval() const;
};

#endif