#include "../../include/maintenance/MaintenanceRequest.h"
#include <chrono>
#include <algorithm>
#include <cmath>

MaintenanceRequest::MaintenanceRequest()
    : requestId(""), property(nullptr), tenant(nullptr),
      issueDescription(""), category(""), priority("normal"),
      requestDate(std::chrono::system_clock::now()),
      scheduledDate(std::chrono::system_clock::now()),
      status("pending"), estimatedCost(0.0), actualCost(0.0),
      assignedVendor("") {}

MaintenanceRequest::MaintenanceRequest(const std::string& id, std::shared_ptr<Property> prop,
                                       std::shared_ptr<Tenant> ten, const std::string& description,
                                       const std::string& category, const std::string& priority,
                                       const std::chrono::system_clock::time_point& requestDate,
                                       const std::chrono::system_clock::time_point& scheduledDate,
                                       const std::string& status, double estimated, double actual,
                                       const std::string& vendor)
    : requestId(id), property(prop), tenant(ten), issueDescription(description),
      category(category), priority(priority), requestDate(requestDate),
      scheduledDate(scheduledDate), status(status), estimatedCost(estimated),
      actualCost(actual), assignedVendor(vendor) {}

MaintenanceRequest::MaintenanceRequest(const MaintenanceRequest& other)
    : requestId(other.requestId), property(other.property), tenant(other.tenant),
      issueDescription(other.issueDescription), category(other.category),
      priority(other.priority), requestDate(other.requestDate),
      scheduledDate(other.scheduledDate), status(other.status),
      estimatedCost(other.estimatedCost), actualCost(other.actualCost),
      assignedVendor(other.assignedVendor) {}

MaintenanceRequest& MaintenanceRequest::operator=(const MaintenanceRequest& other) {
    if (this != &other) {
        requestId = other.requestId;
        property = other.property;
        tenant = other.tenant;
        issueDescription = other.issueDescription;
        category = other.category;
        priority = other.priority;
        requestDate = other.requestDate;
        scheduledDate = other.scheduledDate;
        status = other.status;
        estimatedCost = other.estimatedCost;
        actualCost = other.actualCost;
        assignedVendor = other.assignedVendor;
    }
    return *this;
}

MaintenanceRequest::MaintenanceRequest(MaintenanceRequest&& other) noexcept
    : requestId(std::move(other.requestId)), property(std::move(other.property)),
      tenant(std::move(other.tenant)), issueDescription(std::move(other.issueDescription)),
      category(std::move(other.category)), priority(std::move(other.priority)),
      requestDate(other.requestDate), scheduledDate(other.scheduledDate),
      status(std::move(other.status)), estimatedCost(other.estimatedCost),
      actualCost(other.actualCost), assignedVendor(std::move(other.assignedVendor)) {}

MaintenanceRequest& MaintenanceRequest::operator=(MaintenanceRequest&& other) noexcept {
    if (this != &other) {
        requestId = std::move(other.requestId);
        property = std::move(other.property);
        tenant = std::move(other.tenant);
        issueDescription = std::move(other.issueDescription);
        category = std::move(other.category);
        priority = std::move(other.priority);
        requestDate = other.requestDate;
        scheduledDate = other.scheduledDate;
        status = std::move(other.status);
        estimatedCost = other.estimatedCost;
        actualCost = other.actualCost;
        assignedVendor = std::move(other.assignedVendor);
    }
    return *this;
}

MaintenanceRequest::~MaintenanceRequest() {}

std::string MaintenanceRequest::getRequestId() const { return requestId; }
std::shared_ptr<Property> MaintenanceRequest::getProperty() const { return property; }
Tenant* MaintenanceRequest::getTenant() const { return tenant.get(); }
std::string MaintenanceRequest::getIssueDescription() const { return issueDescription; }
std::string MaintenanceRequest::getCategory() const { return category; }
std::string MaintenanceRequest::getPriority() const { return priority; }
std::chrono::system_clock::time_point MaintenanceRequest::getRequestDate() const { return requestDate; }
std::chrono::system_clock::time_point MaintenanceRequest::getScheduledDate() const { return scheduledDate; }
std::string MaintenanceRequest::getStatus() const { return status; }
double MaintenanceRequest::getEstimatedCost() const { return estimatedCost; }
double MaintenanceRequest::getActualCost() const { return actualCost; }
std::string MaintenanceRequest::getAssignedVendor() const { return assignedVendor; }

void MaintenanceRequest::setStatus(const std::string& newStatus) {
    status = newStatus;
}

void MaintenanceRequest::setActualCost(double cost) {
    actualCost = std::max(0.0, cost);
}

void MaintenanceRequest::setAssignedVendor(const std::string& vendor) {
    assignedVendor = vendor;
}

bool MaintenanceRequest::isEmergencyRequest() const {
    return priority == "emergency" || priority == "high";
}

bool MaintenanceRequest::isScheduled() const {
    return status == "scheduled" || status == "in_progress";
}

bool MaintenanceRequest::isCompleted() const {
    return status == "completed";
}

double MaintenanceRequest::calculateCostVariance() const {
    return actualCost - estimatedCost;
}

int MaintenanceRequest::getDaysSinceRequest(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - requestDate);
    return static_cast<int>(duration.count() / 24.0);
}

bool MaintenanceRequest::requiresImmediateAttention() const {
    return isEmergencyRequest() || getDaysSinceRequest(std::chrono::system_clock::now()) > 7;
}

bool MaintenanceRequest::isWithinBudget() const {
    return actualCost <= estimatedCost * 1.1;
}

bool MaintenanceRequest::canBeDeferred() const {
    return !isEmergencyRequest() && 
           category != "plumbing" && 
           category != "electrical" &&
           category != "heating" &&
           category != "security";
}