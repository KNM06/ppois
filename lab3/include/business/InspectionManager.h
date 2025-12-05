#ifndef INSPECTIONMANAGER_H
#define INSPECTIONMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"

class InspectionManager {
private:
    std::vector<std::shared_ptr<Property>> properties;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> lastInspectionDates;
    int inspectionFrequencyDays;
    double passThreshold;
    std::vector<std::string> inspectionChecklist;

public:
    InspectionManager(int frequencyDays, double threshold, const std::vector<std::string>& checklist);
    
    InspectionManager(const InspectionManager& other);
    InspectionManager& operator=(const InspectionManager& other);
    InspectionManager(InspectionManager&& other) noexcept;
    InspectionManager& operator=(InspectionManager&& other) noexcept;
    ~InspectionManager();

    int getInspectionFrequencyDays() const;
    double getPassThreshold() const;
    std::vector<std::string> getInspectionChecklist() const;

    void setInspectionFrequencyDays(int newFrequency);
    void setPassThreshold(double newThreshold);
    void addChecklistItem(const std::string& item);

    bool scheduleInspection(std::shared_ptr<Property> property,
                           const std::chrono::system_clock::time_point& inspectionDate,
                           std::shared_ptr<Tenant> tenant);
    bool isInspectionDue(std::shared_ptr<Property> property,
                        const std::chrono::system_clock::time_point& currentDate) const;
    double conductInspection(std::shared_ptr<Property> property,
                            const std::vector<std::string>& inspectionResults);
    std::vector<std::shared_ptr<Property>> getOverdueInspections(
        const std::chrono::system_clock::time_point& currentDate) const;
    bool requiresFollowUp(double inspectionScore) const;
    int getDaysUntilNextInspection(std::shared_ptr<Property> property,
                                  const std::chrono::system_clock::time_point& currentDate) const;
    std::string generateInspectionReport(std::shared_ptr<Property> property, double score) const;
    double calculatePropertyConditionScore(std::shared_ptr<Property> property) const;
    bool isPropertyCompliant(std::shared_ptr<Property> property) const;
};

#endif