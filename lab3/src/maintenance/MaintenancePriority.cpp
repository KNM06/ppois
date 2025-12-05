#include "../../include/maintenance/MaintenancePriority.h"
#include <algorithm>

MaintenancePriority::MaintenancePriority(const std::string& id, const std::string& level,
                                         const std::string& description, int responseTime,
                                         int resolutionTime, double costMultiplier,
                                         bool emergencyContact, const std::string& escalation,
                                         const std::string& affectedSystems)
    : priorityId(id), priorityLevel(level), description(description),
      responseTimeHours(responseTime), resolutionTimeDays(resolutionTime),
      costMultiplier(costMultiplier), requiresEmergencyContact(emergencyContact),
      escalationProcedure(escalation), affectedSystems(affectedSystems) {}

MaintenancePriority::MaintenancePriority(const MaintenancePriority& other)
    : priorityId(other.priorityId), priorityLevel(other.priorityLevel),
      description(other.description), responseTimeHours(other.responseTimeHours),
      resolutionTimeDays(other.resolutionTimeDays), costMultiplier(other.costMultiplier),
      requiresEmergencyContact(other.requiresEmergencyContact),
      escalationProcedure(other.escalationProcedure), affectedSystems(other.affectedSystems) {}

MaintenancePriority& MaintenancePriority::operator=(const MaintenancePriority& other) {
    if (this != &other) {
        priorityId = other.priorityId;
        priorityLevel = other.priorityLevel;
        description = other.description;
        responseTimeHours = other.responseTimeHours;
        resolutionTimeDays = other.resolutionTimeDays;
        costMultiplier = other.costMultiplier;
        requiresEmergencyContact = other.requiresEmergencyContact;
        escalationProcedure = other.escalationProcedure;
        affectedSystems = other.affectedSystems;
    }
    return *this;
}

MaintenancePriority::MaintenancePriority(MaintenancePriority&& other) noexcept
    : priorityId(std::move(other.priorityId)), priorityLevel(std::move(other.priorityLevel)),
      description(std::move(other.description)), responseTimeHours(other.responseTimeHours),
      resolutionTimeDays(other.resolutionTimeDays), costMultiplier(other.costMultiplier),
      requiresEmergencyContact(other.requiresEmergencyContact),
      escalationProcedure(std::move(other.escalationProcedure)),
      affectedSystems(std::move(other.affectedSystems)) {}

MaintenancePriority& MaintenancePriority::operator=(MaintenancePriority&& other) noexcept {
    if (this != &other) {
        priorityId = std::move(other.priorityId);
        priorityLevel = std::move(other.priorityLevel);
        description = std::move(other.description);
        responseTimeHours = other.responseTimeHours;
        resolutionTimeDays = other.resolutionTimeDays;
        costMultiplier = other.costMultiplier;
        requiresEmergencyContact = other.requiresEmergencyContact;
        escalationProcedure = std::move(other.escalationProcedure);
        affectedSystems = std::move(other.affectedSystems);
    }
    return *this;
}

MaintenancePriority::~MaintenancePriority() {}

std::string MaintenancePriority::getPriorityId() const { return priorityId; }
std::string MaintenancePriority::getPriorityLevel() const { return priorityLevel; }
std::string MaintenancePriority::getDescription() const { return description; }
int MaintenancePriority::getResponseTimeHours() const { return responseTimeHours; }
int MaintenancePriority::getResolutionTimeDays() const { return resolutionTimeDays; }
double MaintenancePriority::getCostMultiplier() const { return costMultiplier; }
bool MaintenancePriority::getRequiresEmergencyContact() const { return requiresEmergencyContact; }
std::string MaintenancePriority::getEscalationProcedure() const { return escalationProcedure; }
std::string MaintenancePriority::getAffectedSystems() const { return affectedSystems; }

void MaintenancePriority::setResponseTimeHours(int newTime) {
    responseTimeHours = std::max(1, newTime);
}

void MaintenancePriority::setResolutionTimeDays(int newTime) {
    resolutionTimeDays = std::max(1, newTime);
}

void MaintenancePriority::setCostMultiplier(double newMultiplier) {
    costMultiplier = std::max(1.0, newMultiplier);
}

bool MaintenancePriority::isEmergencyPriority() const {
    return priorityLevel == "emergency" || responseTimeHours <= 4;
}

bool MaintenancePriority::isHighPriority() const {
    return priorityLevel == "high" || responseTimeHours <= 24;
}

double MaintenancePriority::calculateAdjustedCost(double baseCost) const {
    return baseCost * costMultiplier;
}

bool MaintenancePriority::requiresImmediateAttention() const {
    return isEmergencyPriority() || requiresEmergencyContact;
}

bool MaintenancePriority::canBeScheduled() const {
    return !isEmergencyPriority() && resolutionTimeDays > 1;
}

bool MaintenancePriority::affectsCriticalSystems() const {
    return affectedSystems.find("electrical") != std::string::npos ||
           affectedSystems.find("plumbing") != std::string::npos ||
           affectedSystems.find("heating") != std::string::npos ||
           affectedSystems.find("security") != std::string::npos;
}

int MaintenancePriority::calculateSLACompliance(int actualResponseHours) const {
    if (actualResponseHours <= responseTimeHours) return 100;
    if (actualResponseHours <= responseTimeHours * 2) return 75;
    if (actualResponseHours <= responseTimeHours * 3) return 50;
    return 0;
}

std::string MaintenancePriority::getRiskLevel() const {
    if (isEmergencyPriority() && affectsCriticalSystems()) return "critical";
    if (isHighPriority() || affectsCriticalSystems()) return "high";
    if (priorityLevel == "medium") return "medium";
    return "low";
}

bool MaintenancePriority::requiresManagementApproval() const {
    return isEmergencyPriority() || costMultiplier > 2.0;
}