#include "../../include/maintenance/MaintenanceSchedule.h"
#include <chrono>
#include <algorithm>
#include <cmath>

MaintenanceSchedule::MaintenanceSchedule(const std::string& id, std::shared_ptr<Property> prop,
                                         const std::string& type, const std::chrono::system_clock::time_point& start,
                                         const std::chrono::system_clock::time_point& end, const std::string& frequency,
                                         const std::vector<std::shared_ptr<MaintenanceTask>>& tasks,
                                         double allocated, double used, const std::string& status,
                                         const std::string& team)
    : scheduleId(id), property(prop), scheduleType(type), startDate(start),
      endDate(end), frequency(frequency), scheduledTasks(tasks),
      budgetAllocated(allocated), budgetUsed(used), status(status),
      assignedTeam(team) {}

MaintenanceSchedule::MaintenanceSchedule(const MaintenanceSchedule& other)
    : scheduleId(other.scheduleId), property(other.property), scheduleType(other.scheduleType),
      startDate(other.startDate), endDate(other.endDate), frequency(other.frequency),
      scheduledTasks(other.scheduledTasks), budgetAllocated(other.budgetAllocated),
      budgetUsed(other.budgetUsed), status(other.status), assignedTeam(other.assignedTeam) {}

MaintenanceSchedule& MaintenanceSchedule::operator=(const MaintenanceSchedule& other) {
    if (this != &other) {
        scheduleId = other.scheduleId;
        property = other.property;
        scheduleType = other.scheduleType;
        startDate = other.startDate;
        endDate = other.endDate;
        frequency = other.frequency;
        scheduledTasks = other.scheduledTasks;
        budgetAllocated = other.budgetAllocated;
        budgetUsed = other.budgetUsed;
        status = other.status;
        assignedTeam = other.assignedTeam;
    }
    return *this;
}

MaintenanceSchedule::MaintenanceSchedule(MaintenanceSchedule&& other) noexcept
    : scheduleId(std::move(other.scheduleId)), property(std::move(other.property)),
      scheduleType(std::move(other.scheduleType)), startDate(other.startDate),
      endDate(other.endDate), frequency(std::move(other.frequency)),
      scheduledTasks(std::move(other.scheduledTasks)), budgetAllocated(other.budgetAllocated),
      budgetUsed(other.budgetUsed), status(std::move(other.status)),
      assignedTeam(std::move(other.assignedTeam)) {}

MaintenanceSchedule& MaintenanceSchedule::operator=(MaintenanceSchedule&& other) noexcept {
    if (this != &other) {
        scheduleId = std::move(other.scheduleId);
        property = std::move(other.property);
        scheduleType = std::move(other.scheduleType);
        startDate = other.startDate;
        endDate = other.endDate;
        frequency = std::move(other.frequency);
        scheduledTasks = std::move(other.scheduledTasks);
        budgetAllocated = other.budgetAllocated;
        budgetUsed = other.budgetUsed;
        status = std::move(other.status);
        assignedTeam = std::move(other.assignedTeam);
    }
    return *this;
}

MaintenanceSchedule::~MaintenanceSchedule() {}

std::string MaintenanceSchedule::getScheduleId() const { return scheduleId; }
Property* MaintenanceSchedule::getProperty() const { return property.get(); }
std::string MaintenanceSchedule::getScheduleType() const { return scheduleType; }
std::chrono::system_clock::time_point MaintenanceSchedule::getStartDate() const { return startDate; }
std::chrono::system_clock::time_point MaintenanceSchedule::getEndDate() const { return endDate; }
std::string MaintenanceSchedule::getFrequency() const { return frequency; }
std::vector<std::shared_ptr<MaintenanceTask>> MaintenanceSchedule::getScheduledTasks() const { return scheduledTasks; }
double MaintenanceSchedule::getBudgetAllocated() const { return budgetAllocated; }
double MaintenanceSchedule::getBudgetUsed() const { return budgetUsed; }
std::string MaintenanceSchedule::getStatus() const { return status; }
std::string MaintenanceSchedule::getAssignedTeam() const { return assignedTeam; }

void MaintenanceSchedule::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void MaintenanceSchedule::setBudgetUsed(double newUsed) {
    budgetUsed = std::max(0.0, newUsed);
}

void MaintenanceSchedule::setAssignedTeam(const std::string& newTeam) {
    assignedTeam = newTeam;
}

void MaintenanceSchedule::addTask(std::shared_ptr<MaintenanceTask> task) {
    scheduledTasks.push_back(task);
}

bool MaintenanceSchedule::isActive(const std::chrono::system_clock::time_point& currentDate) const {
    return currentDate >= startDate && currentDate <= endDate && status == "active";
}

double MaintenanceSchedule::calculateBudgetRemaining() const {
    return budgetAllocated - budgetUsed;
}

bool MaintenanceSchedule::isOverBudget() const {
    return budgetUsed > budgetAllocated;
}

int MaintenanceSchedule::getCompletedTaskCount() const {
    int count = 0;
    for (const auto& task : scheduledTasks) {
        if (task->isCompleted()) {
            count++;
        }
    }
    return count;
}

double MaintenanceSchedule::calculateCompletionPercentage() const {
    if (scheduledTasks.empty()) return 0.0;
    return (static_cast<double>(getCompletedTaskCount()) / scheduledTasks.size()) * 100.0;
}

bool MaintenanceSchedule::requiresBudgetAdjustment() const {
    return budgetUsed > budgetAllocated * 0.9 && calculateCompletionPercentage() < 80.0;
}

bool MaintenanceSchedule::hasOverdueTasks(const std::chrono::system_clock::time_point& currentDate) const {
    for (const auto& task : scheduledTasks) {
        if (task->isOverdue(currentDate)) {
            return true;
        }
    }
    return false;
}

double MaintenanceSchedule::calculateAverageTaskDuration() const {
    if (scheduledTasks.empty()) return 0.0;
    
    double totalHours = 0.0;
    int completedCount = 0;
    
    for (const auto& task : scheduledTasks) {
        if (task->isCompleted()) {
            totalHours += task->getActualHours();
            completedCount++;
        }
    }
    
    return completedCount > 0 ? totalHours / completedCount : 0.0;
}

bool MaintenanceSchedule::isPreventiveMaintenance() const {
    return scheduleType == "preventive" || scheduleType == "routine" || scheduleType == "scheduled";
}

std::vector<std::shared_ptr<MaintenanceTask>> MaintenanceSchedule::getPendingTasks() const {
    std::vector<std::shared_ptr<MaintenanceTask>> pending;
    for (const auto& task : scheduledTasks) {
        if (!task->isCompleted()) {
            pending.push_back(task);
        }
    }
    return pending;
}