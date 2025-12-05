#include "../../include/business/InspectionManager.h"
#include <chrono>
#include <algorithm>
#include <cmath>

InspectionManager::InspectionManager(int frequencyDays,
                                     double threshold,
                                     const std::vector<std::string>& checklist)
    : inspectionFrequencyDays(std::max(1, frequencyDays)),
      passThreshold(std::clamp(threshold, 0.0, 100.0)),
      inspectionChecklist(checklist) {}

InspectionManager::InspectionManager(const InspectionManager& other)
    : properties(other.properties),
      lastInspectionDates(other.lastInspectionDates),
      inspectionFrequencyDays(other.inspectionFrequencyDays),
      passThreshold(other.passThreshold),
      inspectionChecklist(other.inspectionChecklist) {}

InspectionManager& InspectionManager::operator=(const InspectionManager& other) {
    if (this != &other) {
        properties = other.properties;
        lastInspectionDates = other.lastInspectionDates;
        inspectionFrequencyDays = other.inspectionFrequencyDays;
        passThreshold = other.passThreshold;
        inspectionChecklist = other.inspectionChecklist;
    }
    return *this;
}

InspectionManager::InspectionManager(InspectionManager&& other) noexcept
    : properties(std::move(other.properties)),
      lastInspectionDates(std::move(other.lastInspectionDates)),
      inspectionFrequencyDays(other.inspectionFrequencyDays),
      passThreshold(other.passThreshold),
      inspectionChecklist(std::move(other.inspectionChecklist)) {}

InspectionManager& InspectionManager::operator=(InspectionManager&& other) noexcept {
    if (this != &other) {
        properties = std::move(other.properties);
        lastInspectionDates = std::move(other.lastInspectionDates);
        inspectionFrequencyDays = other.inspectionFrequencyDays;
        passThreshold = other.passThreshold;
        inspectionChecklist = std::move(other.inspectionChecklist);
    }
    return *this;
}

InspectionManager::~InspectionManager() = default;

int InspectionManager::getInspectionFrequencyDays() const { return inspectionFrequencyDays; }
double InspectionManager::getPassThreshold() const { return passThreshold; }
std::vector<std::string> InspectionManager::getInspectionChecklist() const { return inspectionChecklist; }

void InspectionManager::setInspectionFrequencyDays(int newFrequency) {
    inspectionFrequencyDays = std::max(1, newFrequency);
}

void InspectionManager::setPassThreshold(double newThreshold) {
    passThreshold = std::clamp(newThreshold, 0.0, 100.0);
}

void InspectionManager::addChecklistItem(const std::string& item) {
    inspectionChecklist.push_back(item);
}

bool InspectionManager::scheduleInspection(std::shared_ptr<Property> property,
                                           const std::chrono::system_clock::time_point& inspectionDate,
                                           std::shared_ptr<Tenant> /*tenant*/) {
    if (!property) return false;

    auto it_prop = std::find_if(properties.begin(), properties.end(),
                                [&property](const std::shared_ptr<Property>& p) {
                                    return p && property && p->getId() == property->getId();
                                });
    if (it_prop == properties.end()) {
        properties.push_back(property);
    }

    lastInspectionDates[property->getId()] = inspectionDate;
    return true;
}

bool InspectionManager::isInspectionDue(std::shared_ptr<Property> property,
                                       const std::chrono::system_clock::time_point& currentDate) const {
    if (!property) return false;

    auto it = lastInspectionDates.find(property->getId());
    if (it == lastInspectionDates.end()) return true;

    auto durationHours = std::chrono::duration_cast<std::chrono::hours>(currentDate - it->second).count();
    if (durationHours < 0) return false; 

    int daysSinceInspection = static_cast<int>(durationHours / 24);
    return daysSinceInspection >= inspectionFrequencyDays;
}

double InspectionManager::conductInspection(std::shared_ptr<Property> property,
                                            const std::vector<std::string>& inspectionResults) {
    if (!property) return 0.0;
    if (inspectionChecklist.empty()) return 0.0;
    if (inspectionResults.size() != inspectionChecklist.size()) return 0.0;

    int passedItems = 0;
    for (size_t i = 0; i < inspectionResults.size(); ++i) {
        const auto& r = inspectionResults[i];
        if (r == "pass" || r == "satisfactory") ++passedItems;
    }

    double score = (static_cast<double>(passedItems) / inspectionChecklist.size()) * 100.0;
    lastInspectionDates[property->getId()] = std::chrono::system_clock::now();

    auto it_prop = std::find_if(properties.begin(), properties.end(),
                                [&property](const std::shared_ptr<Property>& p) {
                                    return p && property && p->getId() == property->getId();
                                });
    if (it_prop == properties.end()) properties.push_back(property);

    return score;
}

std::vector<std::shared_ptr<Property>> InspectionManager::getOverdueInspections(
    const std::chrono::system_clock::time_point& currentDate) const {

    std::vector<std::shared_ptr<Property>> overdue;
    overdue.reserve(properties.size());

    for (const auto& prop : properties) {
        if (!prop) continue;
        if (isInspectionDue(prop, currentDate)) {
            overdue.push_back(prop);
        }
    }

    return overdue;
}

bool InspectionManager::requiresFollowUp(double inspectionScore) const {
    return inspectionScore < passThreshold;
}

int InspectionManager::getDaysUntilNextInspection(std::shared_ptr<Property> property,
                                                 const std::chrono::system_clock::time_point& currentDate) const {
    if (!property) return 0;

    auto it = lastInspectionDates.find(property->getId());
    if (it == lastInspectionDates.end()) {
        return 0;
    }

    auto nextInspectionDate = it->second + std::chrono::hours(static_cast<long long>(inspectionFrequencyDays) * 24LL);
    auto diffHours = std::chrono::duration_cast<std::chrono::hours>(nextInspectionDate - currentDate).count();

    if (diffHours <= 0) return 0;
    int days = static_cast<int>((diffHours + 23) / 24);
    return days;
}

std::string InspectionManager::generateInspectionReport(std::shared_ptr<Property> property, double score) const {
    if (!property) return "Invalid property";

    std::string status = (score >= passThreshold) ? "PASS" : "FAIL";
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.2f", score);
    return "Inspection Report for " + property->getId() + ": Score " + std::string(buf) + "%, Status: " + status;
}

double InspectionManager::calculatePropertyConditionScore(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;

    double baseScore = 80.0;
    auto it = lastInspectionDates.find(property->getId());
    if (it != lastInspectionDates.end()) {
        auto duration = std::chrono::duration_cast<std::chrono::hours>(
            std::chrono::system_clock::now() - it->second).count();
        if (duration > 0) {
            int monthsSinceInspection = static_cast<int>(duration / (24 * 30));
            baseScore -= monthsSinceInspection * 2.0;
        }
    } else {
        baseScore -= 10.0;
    }

    return std::clamp(baseScore, 0.0, 100.0);
}

bool InspectionManager::isPropertyCompliant(std::shared_ptr<Property> property) const {
    return calculatePropertyConditionScore(property) >= passThreshold;
}
