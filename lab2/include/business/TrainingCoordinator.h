#ifndef TRAININGCOORDINATOR_H
#define TRAININGCOORDINATOR_H

#include "../../include/business/EmployeeManager.h"
#include "../../include/core/Customer.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

struct TrainingSession {
    std::string sessionId;
    std::string trainingType;
    std::vector<std::string> participantIds;
    std::string scheduledDate;
    int durationHours;
    double cost;
    bool completed;
};

class TrainingCoordinator {
private:
    std::shared_ptr<EmployeeManager> employeeManager;
    std::unordered_map<std::string, TrainingSession> trainingSessions;
    std::unordered_map<std::string, std::vector<std::string>> employeeCertifications;
    std::unordered_map<std::string, double> trainingCosts;
    std::vector<std::string> requiredCertifications;

public:
    TrainingCoordinator(std::shared_ptr<EmployeeManager> empManager);
    
    bool scheduleTrainingSession(const std::string& trainingType, const std::vector<std::string>& participantIds, 
                                const std::string& date, int duration);
    double calculateTrainingCost(const std::string& trainingType, int participantCount) const;
    bool validateEmployeeCertification(const std::string& employeeId, const std::string& equipmentType) const;
    std::vector<std::string> getRequiredTraining(const std::string& employeeId) const;
    double calculateTrainingROI(const std::string& sessionId) const;
    bool isCertificationRequired(const std::string& equipmentType) const;
    void addEmployeeCertification(const std::string& employeeId, const std::string& certification);
    std::vector<std::string> getCertifiedEmployees(const std::string& equipmentType) const;
    
    std::shared_ptr<EmployeeManager> getEmployeeManager() const;
    std::vector<std::string> getEmployeeCertifications(const std::string& employeeId) const;
    int getTrainingSessionCount() const;
    const std::unordered_map<std::string, TrainingSession>& getTrainingSessions() const {
    return trainingSessions;
}
    void setEmployeeManager(std::shared_ptr<EmployeeManager> manager);
    void addRequiredCertification(const std::string& certification);
    void setTrainingCost(const std::string& trainingType, double cost);
};

#endif