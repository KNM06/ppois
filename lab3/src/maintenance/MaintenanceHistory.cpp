#include "../../include/maintenance/MaintenanceHistory.h"
#include <chrono>
#include <algorithm>
#include <cmath>

MaintenanceHistory::MaintenanceHistory(const std::string& id, std::shared_ptr<Property> prop,
                                       const std::vector<std::shared_ptr<MaintenanceRequest>>& requests,
                                       const std::chrono::system_clock::time_point& lastMaintenance,
                                       const std::chrono::system_clock::time_point& nextScheduled,
                                       double totalCost, int totalRequests,
                                       const std::string& frequency, const std::string& condition)
    : historyId(id), property(prop), maintenanceRequests(requests),
      lastMaintenanceDate(lastMaintenance), nextScheduledMaintenance(nextScheduled),
      totalMaintenanceCost(totalCost), totalRequests(totalRequests),
      maintenanceFrequency(frequency), overallCondition(condition) {}

MaintenanceHistory::MaintenanceHistory(const MaintenanceHistory& other)
    : historyId(other.historyId), property(other.property),
      maintenanceRequests(other.maintenanceRequests),
      lastMaintenanceDate(other.lastMaintenanceDate),
      nextScheduledMaintenance(other.nextScheduledMaintenance),
      totalMaintenanceCost(other.totalMaintenanceCost),
      totalRequests(other.totalRequests), maintenanceFrequency(other.maintenanceFrequency),
      overallCondition(other.overallCondition) {}

MaintenanceHistory& MaintenanceHistory::operator=(const MaintenanceHistory& other) {
    if (this != &other) {
        historyId = other.historyId;
        property = other.property;
        maintenanceRequests = other.maintenanceRequests;
        lastMaintenanceDate = other.lastMaintenanceDate;
        nextScheduledMaintenance = other.nextScheduledMaintenance;
        totalMaintenanceCost = other.totalMaintenanceCost;
        totalRequests = other.totalRequests;
        maintenanceFrequency = other.maintenanceFrequency;
        overallCondition = other.overallCondition;
    }
    return *this;
}

MaintenanceHistory::MaintenanceHistory(MaintenanceHistory&& other) noexcept
    : historyId(std::move(other.historyId)), property(std::move(other.property)),
      maintenanceRequests(std::move(other.maintenanceRequests)),
      lastMaintenanceDate(other.lastMaintenanceDate),
      nextScheduledMaintenance(other.nextScheduledMaintenance),
      totalMaintenanceCost(other.totalMaintenanceCost), totalRequests(other.totalRequests),
      maintenanceFrequency(std::move(other.maintenanceFrequency)),
      overallCondition(std::move(other.overallCondition)) {}

MaintenanceHistory& MaintenanceHistory::operator=(MaintenanceHistory&& other) noexcept {
    if (this != &other) {
        historyId = std::move(other.historyId);
        property = std::move(other.property);
        maintenanceRequests = std::move(other.maintenanceRequests);
        lastMaintenanceDate = other.lastMaintenanceDate;
        nextScheduledMaintenance = other.nextScheduledMaintenance;
        totalMaintenanceCost = other.totalMaintenanceCost;
        totalRequests = other.totalRequests;
        maintenanceFrequency = std::move(other.maintenanceFrequency);
        overallCondition = std::move(other.overallCondition);
    }
    return *this;
}

MaintenanceHistory::~MaintenanceHistory() {}

std::string MaintenanceHistory::getHistoryId() const { return historyId; }
Property* MaintenanceHistory::getProperty() const { return property.get(); }
std::vector<std::shared_ptr<MaintenanceRequest>> MaintenanceHistory::getMaintenanceRequests() const { return maintenanceRequests; }
std::chrono::system_clock::time_point MaintenanceHistory::getLastMaintenanceDate() const { return lastMaintenanceDate; }
std::chrono::system_clock::time_point MaintenanceHistory::getNextScheduledMaintenance() const { return nextScheduledMaintenance; }
double MaintenanceHistory::getTotalMaintenanceCost() const { return totalMaintenanceCost; }
int MaintenanceHistory::getTotalRequests() const { return totalRequests; }
std::string MaintenanceHistory::getMaintenanceFrequency() const { return maintenanceFrequency; }
std::string MaintenanceHistory::getOverallCondition() const { return overallCondition; }

void MaintenanceHistory::setLastMaintenanceDate(const std::chrono::system_clock::time_point& newDate) {
    lastMaintenanceDate = newDate;
}

void MaintenanceHistory::setNextScheduledMaintenance(const std::chrono::system_clock::time_point& newDate) {
    nextScheduledMaintenance = newDate;
}

void MaintenanceHistory::setOverallCondition(const std::string& newCondition) {
    overallCondition = newCondition;
}

double MaintenanceHistory::calculateAverageMaintenanceCost() const {
    if (totalRequests == 0) return 0.0;
    return totalMaintenanceCost / totalRequests;
}

bool MaintenanceHistory::isMaintenanceOverdue(const std::chrono::system_clock::time_point& currentDate) const {
    return currentDate > nextScheduledMaintenance;
}

int MaintenanceHistory::getDaysSinceLastMaintenance(const std::chrono::system_clock::time_point& currentDate) const {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(currentDate - lastMaintenanceDate);
    return static_cast<int>(duration.count() / 24.0);
}

double MaintenanceHistory::calculateMaintenanceCostPerSqFt() const {
    if (!property || property->getArea() <= 0) return 0.0;
    return totalMaintenanceCost / property->getArea();
}

bool MaintenanceHistory::hasFrequentIssues() const {
    return totalRequests > 10 || maintenanceFrequency == "frequent";
}

std::vector<std::shared_ptr<MaintenanceRequest>> MaintenanceHistory::getEmergencyRequests() const {
    std::vector<std::shared_ptr<MaintenanceRequest>> emergencies;
    for (const auto& request : maintenanceRequests) {
        if (request->isEmergencyRequest()) {
            emergencies.push_back(request);
        }
    }
    return emergencies;
}

double MaintenanceHistory::calculatePreventiveMaintenanceRatio() const {
    int preventiveCount = 0;
    for (const auto& request : maintenanceRequests) {
        if (request->getCategory() == "preventive" || request->getCategory() == "routine") {
            preventiveCount++;
        }
    }
    
    if (totalRequests == 0) return 0.0;
    return (static_cast<double>(preventiveCount) / totalRequests) * 100.0;
}

std::string MaintenanceHistory::getMaintenanceEfficiency() const {
    double preventiveRatio = calculatePreventiveMaintenanceRatio();
    double costPerSqFt = calculateMaintenanceCostPerSqFt();
    
    if (preventiveRatio > 70.0 && costPerSqFt < 2.0) return "excellent";
    if (preventiveRatio > 50.0 && costPerSqFt < 5.0) return "good";
    if (preventiveRatio > 30.0 && costPerSqFt < 10.0) return "fair";
    return "needs_improvement";
}

bool MaintenanceHistory::requiresMajorRenovation() const {
    return overallCondition == "poor" || totalMaintenanceCost > 50000.0 || hasFrequentIssues();
}