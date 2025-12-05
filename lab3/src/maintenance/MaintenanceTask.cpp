#include "../../include/maintenance/MaintenanceTask.h"
#include <chrono>
#include <algorithm>
#include <cmath>

MaintenanceTask::MaintenanceTask(const std::string& id, std::shared_ptr<MaintenanceRequest> req,
                                 const std::string& description, const std::string& assignedTo,
                                 const std::chrono::system_clock::time_point& start,
                                 const std::chrono::system_clock::time_point& estimatedComplete,
                                 const std::chrono::system_clock::time_point& actualComplete,
                                 const std::string& status, double estimatedHours,
                                 double actualHours, const std::string& priority,
                                 const std::vector<std::string>& materials)
    : taskId(id), request(req), taskDescription(description), assignedTo(assignedTo),
      startDate(start), estimatedCompletion(estimatedComplete),
      actualCompletion(actualComplete), status(status),
      estimatedHours(estimatedHours), actualHours(actualHours), priority(priority),
      requiredMaterials(materials) {}

MaintenanceTask::MaintenanceTask(const MaintenanceTask& other)
    : taskId(other.taskId), request(other.request), taskDescription(other.taskDescription),
      assignedTo(other.assignedTo), startDate(other.startDate),
      estimatedCompletion(other.estimatedCompletion), actualCompletion(other.actualCompletion),
      status(other.status), estimatedHours(other.estimatedHours),
      actualHours(other.actualHours), priority(other.priority),
      requiredMaterials(other.requiredMaterials) {}

MaintenanceTask& MaintenanceTask::operator=(const MaintenanceTask& other) {
    if (this != &other) {
        taskId = other.taskId;
        request = other.request;
        taskDescription = other.taskDescription;
        assignedTo = other.assignedTo;
        startDate = other.startDate;
        estimatedCompletion = other.estimatedCompletion;
        actualCompletion = other.actualCompletion;
        status = other.status;
        estimatedHours = other.estimatedHours;
        actualHours = other.actualHours;
        priority = other.priority;
        requiredMaterials = other.requiredMaterials;
    }
    return *this;
}

MaintenanceTask::MaintenanceTask(MaintenanceTask&& other) noexcept
    : taskId(std::move(other.taskId)), request(std::move(other.request)),
      taskDescription(std::move(other.taskDescription)), assignedTo(std::move(other.assignedTo)),
      startDate(other.startDate), estimatedCompletion(other.estimatedCompletion),
      actualCompletion(other.actualCompletion), status(std::move(other.status)),
      estimatedHours(other.estimatedHours), actualHours(other.actualHours),
      priority(std::move(other.priority)), requiredMaterials(std::move(other.requiredMaterials)) {}

MaintenanceTask& MaintenanceTask::operator=(MaintenanceTask&& other) noexcept {
    if (this != &other) {
        taskId = std::move(other.taskId);
        request = std::move(other.request);
        taskDescription = std::move(other.taskDescription);
        assignedTo = std::move(other.assignedTo);
        startDate = other.startDate;
        estimatedCompletion = other.estimatedCompletion;
        actualCompletion = other.actualCompletion;
        status = std::move(other.status);
        estimatedHours = other.estimatedHours;
        actualHours = other.actualHours;
        priority = std::move(other.priority);
        requiredMaterials = std::move(other.requiredMaterials);
    }
    return *this;
}

MaintenanceTask::~MaintenanceTask() {}

std::string MaintenanceTask::getTaskId() const { return taskId; }
MaintenanceRequest* MaintenanceTask::getRequest() const { return request.get(); }
std::string MaintenanceTask::getTaskDescription() const { return taskDescription; }
std::string MaintenanceTask::getAssignedTo() const { return assignedTo; }
std::chrono::system_clock::time_point MaintenanceTask::getStartDate() const { return startDate; }
std::chrono::system_clock::time_point MaintenanceTask::getEstimatedCompletion() const { return estimatedCompletion; }
std::chrono::system_clock::time_point MaintenanceTask::getActualCompletion() const { return actualCompletion; }
std::string MaintenanceTask::getStatus() const { return status; }
double MaintenanceTask::getEstimatedHours() const { return estimatedHours; }
double MaintenanceTask::getActualHours() const { return actualHours; }
std::string MaintenanceTask::getPriority() const { return priority; }
std::vector<std::string> MaintenanceTask::getRequiredMaterials() const { return requiredMaterials; }

void MaintenanceTask::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void MaintenanceTask::setAssignedTo(const std::string& newAssignee) {
    assignedTo = newAssignee;
}

void MaintenanceTask::setActualCompletion(const std::chrono::system_clock::time_point& completion) {
    actualCompletion = completion;
}

void MaintenanceTask::setActualHours(double hours) {
    actualHours = std::max(0.0, hours);
}

bool MaintenanceTask::isOverdue(const std::chrono::system_clock::time_point& currentDate) const {
    return status != "completed" && currentDate > estimatedCompletion;
}

double MaintenanceTask::calculateTimeVariance() const {
    if (status != "completed") return 0.0;
    
    auto estimatedDuration = std::chrono::duration_cast<std::chrono::hours>(estimatedCompletion - startDate);
    auto actualDuration = std::chrono::duration_cast<std::chrono::hours>(actualCompletion - startDate);
    
    return actualDuration.count() - estimatedDuration.count();
}

bool MaintenanceTask::isCompleted() const {
    return status == "completed";
}

bool MaintenanceTask::requiresSpecializedTools() const {
    for (const auto& material : requiredMaterials) {
        if (material.find("specialized") != std::string::npos ||
            material.find("professional") != std::string::npos) {
            return true;
        }
    }
    return false;
}

double MaintenanceTask::calculateLaborCost(double hourlyRate) const {
    return actualHours * hourlyRate;
}

bool MaintenanceTask::canBeDelegated() const {
    return priority != "emergency" && !requiresSpecializedTools();
}

int MaintenanceTask::getDaysToComplete() const {
    if (status != "completed") return 0;
    
    auto duration = std::chrono::duration_cast<std::chrono::hours>(actualCompletion - startDate);
    return static_cast<int>(std::ceil(duration.count() / 24.0));
}

bool MaintenanceTask::isHighPriority() const {
    return priority == "high" || priority == "emergency";
}

double MaintenanceTask::calculateEfficiency() const {
    if (estimatedHours <= 0 || actualHours <= 0) return 0.0;
    return (estimatedHours / actualHours) * 100.0;
}