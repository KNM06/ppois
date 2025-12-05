#include "../../include/maintenance/MaintenanceStatus.h"
#include <chrono>
#include <algorithm>

MaintenanceStatus::MaintenanceStatus(const std::string& id, const std::string& code,
                                     const std::string& description, bool active, bool completed,
                                     bool requiresFollowUp, const std::chrono::system_clock::time_point& statusDate,
                                     const std::string& nextAction, int estimatedCompletion,
                                     const std::string& resolutionNotes)
    : statusId(id), statusCode(code), description(description), isActive(active),
      isCompleted(completed), requiresFollowUp(requiresFollowUp), statusDate(statusDate),
      nextAction(nextAction), estimatedCompletionDays(estimatedCompletion),
      resolutionNotes(resolutionNotes) {}

MaintenanceStatus::MaintenanceStatus(const MaintenanceStatus& other)
    : statusId(other.statusId), statusCode(other.statusCode), description(other.description),
      isActive(other.isActive), isCompleted(other.isCompleted),
      requiresFollowUp(other.requiresFollowUp), statusDate(other.statusDate),
      nextAction(other.nextAction), estimatedCompletionDays(other.estimatedCompletionDays),
      resolutionNotes(other.resolutionNotes) {}

MaintenanceStatus& MaintenanceStatus::operator=(const MaintenanceStatus& other) {
    if (this != &other) {
        statusId = other.statusId;
        statusCode = other.statusCode;
        description = other.description;
        isActive = other.isActive;
        isCompleted = other.isCompleted;
        requiresFollowUp = other.requiresFollowUp;
        statusDate = other.statusDate;
        nextAction = other.nextAction;
        estimatedCompletionDays = other.estimatedCompletionDays;
        resolutionNotes = other.resolutionNotes;
    }
    return *this;
}

MaintenanceStatus::MaintenanceStatus(MaintenanceStatus&& other) noexcept
    : statusId(std::move(other.statusId)), statusCode(std::move(other.statusCode)),
      description(std::move(other.description)), isActive(other.isActive),
      isCompleted(other.isCompleted), requiresFollowUp(other.requiresFollowUp),
      statusDate(other.statusDate), nextAction(std::move(other.nextAction)),
      estimatedCompletionDays(other.estimatedCompletionDays),
      resolutionNotes(std::move(other.resolutionNotes)) {}

MaintenanceStatus& MaintenanceStatus::operator=(MaintenanceStatus&& other) noexcept {
    if (this != &other) {
        statusId = std::move(other.statusId);
        statusCode = std::move(other.statusCode);
        description = std::move(other.description);
        isActive = other.isActive;
        isCompleted = other.isCompleted;
        requiresFollowUp = other.requiresFollowUp;
        statusDate = other.statusDate;
        nextAction = std::move(other.nextAction);
        estimatedCompletionDays = other.estimatedCompletionDays;
        resolutionNotes = std::move(other.resolutionNotes);
    }
    return *this;
}

MaintenanceStatus::~MaintenanceStatus() {}

std::string MaintenanceStatus::getStatusId() const { return statusId; }
std::string MaintenanceStatus::getStatusCode() const { return statusCode; }
std::string MaintenanceStatus::getDescription() const { return description; }
bool MaintenanceStatus::getIsActive() const { return isActive; }
bool MaintenanceStatus::getIsCompleted() const { return isCompleted; }
bool MaintenanceStatus::getRequiresFollowUp() const { return requiresFollowUp; }
std::chrono::system_clock::time_point MaintenanceStatus::getStatusDate() const { return statusDate; }
std::string MaintenanceStatus::getNextAction() const { return nextAction; }
int MaintenanceStatus::getEstimatedCompletionDays() const { return estimatedCompletionDays; }
std::string MaintenanceStatus::getResolutionNotes() const { return resolutionNotes; }

void MaintenanceStatus::setStatusCode(const std::string& newCode) {
    statusCode = newCode;
}

void MaintenanceStatus::setIsCompleted(bool completed) {
    isCompleted = completed;
    isActive = !completed;
}

void MaintenanceStatus::setNextAction(const std::string& action) {
    nextAction = action;
}

bool MaintenanceStatus::isWorkInProgress() const {
    return isActive && !isCompleted && statusCode != "pending";
}

bool MaintenanceStatus::isPendingApproval() const {
    return statusCode == "pending_approval" || statusCode == "awaiting_authorization";
}

bool MaintenanceStatus::requiresVendorAttention() const {
    return statusCode == "assigned" || statusCode == "in_progress" || 
           statusCode == "awaiting_parts";
}

int MaintenanceStatus::getDaysInStatus(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - statusDate);
    return static_cast<int>(duration.count() / 24.0);
}

bool MaintenanceStatus::isDelayed() const {
    return getDaysInStatus(std::chrono::system_clock::now()) > estimatedCompletionDays && isActive;
}

bool MaintenanceStatus::canBeClosed() const {
    return isCompleted && !requiresFollowUp && !resolutionNotes.empty();
}

std::string MaintenanceStatus::getStatusCategory() const {
    if (isCompleted) return "completed";
    if (isPendingApproval()) return "pending";
    if (isWorkInProgress()) return "in_progress";
    if (statusCode == "scheduled") return "scheduled";
    return "new";
}

bool MaintenanceStatus::requiresCustomerNotification() const {
    return statusCode == "completed" || statusCode == "delayed" || requiresFollowUp;
}

double MaintenanceStatus::calculateCompletionPercentage() const {
    if (isCompleted) return 100.0;
    if (isWorkInProgress()) return 50.0;
    if (isPendingApproval()) return 25.0;
    return 0.0;
}