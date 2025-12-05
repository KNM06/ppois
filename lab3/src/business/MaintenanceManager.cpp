#include "../../include/business/MaintenanceManager.h"
#include <chrono>
#include <algorithm>
#include <cmath>

MaintenanceManager::MaintenanceManager(double budgetThreshold, int maxConcurrent,
                                       const std::chrono::system_clock::time_point& lastInspection)
    : budgetThreshold(budgetThreshold), maxConcurrentRequests(maxConcurrent),
      lastInspectionDate(lastInspection) {}

MaintenanceManager::MaintenanceManager(const MaintenanceManager& other)
    : maintenanceRequests(other.maintenanceRequests),
      propertyMaintenanceHistory(other.propertyMaintenanceHistory),
      vendorRatings(other.vendorRatings), budgetThreshold(other.budgetThreshold),
      maxConcurrentRequests(other.maxConcurrentRequests),
      lastInspectionDate(other.lastInspectionDate) {}

MaintenanceManager& MaintenanceManager::operator=(const MaintenanceManager& other) {
    if (this != &other) {
        maintenanceRequests = other.maintenanceRequests;
        propertyMaintenanceHistory = other.propertyMaintenanceHistory;
        vendorRatings = other.vendorRatings;
        budgetThreshold = other.budgetThreshold;
        maxConcurrentRequests = other.maxConcurrentRequests;
        lastInspectionDate = other.lastInspectionDate;
    }
    return *this;
}

MaintenanceManager::MaintenanceManager(MaintenanceManager&& other) noexcept
    : maintenanceRequests(std::move(other.maintenanceRequests)),
      propertyMaintenanceHistory(std::move(other.propertyMaintenanceHistory)),
      vendorRatings(std::move(other.vendorRatings)), budgetThreshold(other.budgetThreshold),
      maxConcurrentRequests(other.maxConcurrentRequests),
      lastInspectionDate(other.lastInspectionDate) {}

MaintenanceManager& MaintenanceManager::operator=(MaintenanceManager&& other) noexcept {
    if (this != &other) {
        maintenanceRequests = std::move(other.maintenanceRequests);
        propertyMaintenanceHistory = std::move(other.propertyMaintenanceHistory);
        vendorRatings = std::move(other.vendorRatings);
        budgetThreshold = other.budgetThreshold;
        maxConcurrentRequests = other.maxConcurrentRequests;
        lastInspectionDate = other.lastInspectionDate;
    }
    return *this;
}

MaintenanceManager::~MaintenanceManager() {}

double MaintenanceManager::getBudgetThreshold() const { return budgetThreshold; }
int MaintenanceManager::getMaxConcurrentRequests() const { return maxConcurrentRequests; }
std::chrono::system_clock::time_point MaintenanceManager::getLastInspectionDate() const { return lastInspectionDate; }

void MaintenanceManager::setBudgetThreshold(double newThreshold) {
    budgetThreshold = std::max(0.0, newThreshold);
}

void MaintenanceManager::setMaxConcurrentRequests(int newMax) {
    maxConcurrentRequests = std::max(1, newMax);
}

void MaintenanceManager::updateLastInspectionDate(const std::chrono::system_clock::time_point& newDate) {
    lastInspectionDate = newDate;
}

bool MaintenanceManager::createMaintenanceRequest(std::shared_ptr<Property> property,
                                                 std::shared_ptr<Tenant> tenant,
                                                 const std::string& description,
                                                 const std::string& category,
                                                 const std::string& priority) {
    if (!property) return false;
    
    auto request = std::make_shared<MaintenanceRequest>(
        "MREQ" + std::to_string(maintenanceRequests.size() + 1),
        property, tenant, description, category, priority,
        std::chrono::system_clock::now(),
        std::chrono::system_clock::time_point(),
        "pending", 0.0, 0.0, ""
    );
    
    maintenanceRequests.push_back(request);
    propertyMaintenanceHistory[property->getId()].push_back(request);
    return true;
}

bool MaintenanceManager::scheduleMaintenance(const std::string& requestId,
                                            const std::chrono::system_clock::time_point& scheduledDate,
                                            const std::string& vendor) {
    for (auto& request : maintenanceRequests) {
        if (request->getRequestId() == requestId) {
            request->setStatus("scheduled");
            request->setAssignedVendor(vendor);
            return true;
        }
    }
    return false;
}

bool MaintenanceManager::completeMaintenance(const std::string& requestId, double actualCost) {
    for (auto& request : maintenanceRequests) {
        if (request->getRequestId() == requestId) {
            request->setStatus("completed");
            request->setActualCost(actualCost);
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<MaintenanceRequest>> MaintenanceManager::getPendingRequests() const {
    std::vector<std::shared_ptr<MaintenanceRequest>> pending;
    for (const auto& request : maintenanceRequests) {
        if (request->getStatus() == "pending" || request->getStatus() == "scheduled") {
            pending.push_back(request);
        }
    }
    return pending;
}

std::vector<std::shared_ptr<MaintenanceRequest>> MaintenanceManager::getEmergencyRequests() const {
    std::vector<std::shared_ptr<MaintenanceRequest>> emergency;
    for (const auto& request : maintenanceRequests) {
        if (request->isEmergencyRequest() && request->getStatus() != "completed") {
            emergency.push_back(request);
        }
    }
    return emergency;
}

double MaintenanceManager::calculateMaintenanceCosts(const std::chrono::system_clock::time_point& startDate,
                                                    const std::chrono::system_clock::time_point& endDate) const {
    double totalCost = 0.0;
    for (const auto& request : maintenanceRequests) {
        if (request->getStatus() == "completed" &&
            request->getRequestDate() >= startDate &&
            request->getRequestDate() <= endDate) {
            totalCost += request->getActualCost();
        }
    }
    return totalCost;
}

bool MaintenanceManager::isPropertyUnderMaintenance(const std::string& propertyId) const {
    auto it = propertyMaintenanceHistory.find(propertyId);
    if (it == propertyMaintenanceHistory.end()) return false;
    
    for (const auto& request : it->second) {
        if (request->getStatus() != "completed") {
            return true;
        }
    }
    return false;
}

double MaintenanceManager::calculateMaintenanceBudget(double propertyValue) const {
    return propertyValue * 0.01;
}

bool MaintenanceManager::needsRoutineInspection(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - lastInspectionDate);
    int daysSinceInspection = static_cast<int>(duration.count() / 24.0);
    return daysSinceInspection > 90;
}

void MaintenanceManager::rateVendor(const std::string& vendor, double rating) {
    vendorRatings[vendor] = std::max(1.0, std::min(5.0, rating));
}

double MaintenanceManager::getVendorRating(const std::string& vendor) const {
    auto it = vendorRatings.find(vendor);
    return it != vendorRatings.end() ? it->second : 0.0;
}

std::vector<std::shared_ptr<MaintenanceRequest>> MaintenanceManager::getOverdueMaintenance(const std::chrono::system_clock::time_point& currentDate) const {
    std::vector<std::shared_ptr<MaintenanceRequest>> overdue;
    for (const auto& request : maintenanceRequests) {
        if (request->requiresImmediateAttention() && request->getStatus() != "completed") {
            overdue.push_back(request);
        }
    }
    return overdue;
}