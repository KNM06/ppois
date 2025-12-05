#include "../../include/business/TrainingCoordinator.h"
#include <algorithm>

TrainingCoordinator::TrainingCoordinator(std::shared_ptr<EmployeeManager> empManager)
    : employeeManager(empManager) {
    
    trainingCosts["SAFETY"] = 100.0;
    trainingCosts["EQUIPMENT"] = 150.0;
    trainingCosts["CUSTOMER_SERVICE"] = 80.0;
    trainingCosts["TECHNICAL"] = 200.0;
    
    requiredCertifications = {"HEAVY_EQUIPMENT", "ELECTRICAL", "CONSTRUCTION_SAFETY"};
}

bool TrainingCoordinator::scheduleTrainingSession(const std::string& trainingType, 
                                                const std::vector<std::string>& participantIds,
                                                const std::string& date, int duration) {
    TrainingSession session;
    session.sessionId = "TRAIN" + std::to_string(trainingSessions.size() + 1000);
    session.trainingType = trainingType;
    session.participantIds = participantIds;
    session.scheduledDate = date;
    session.durationHours = duration;
    session.cost = calculateTrainingCost(trainingType, participantIds.size());
    session.completed = false;
    
    trainingSessions[session.sessionId] = session;
    return true;
}

double TrainingCoordinator::calculateTrainingCost(const std::string& trainingType, int participantCount) const {
    auto costIt = trainingCosts.find(trainingType);
    double baseCost = costIt != trainingCosts.end() ? costIt->second : 50.0;
    
    double participantCost = participantCount * baseCost * 0.1;
    double materialsCost = baseCost * 0.3;
    
    return baseCost + participantCost + materialsCost;
}

bool TrainingCoordinator::validateEmployeeCertification(const std::string& employeeId, const std::string& equipmentType) const {
    auto certIt = employeeCertifications.find(employeeId);
    if (certIt == employeeCertifications.end()) {
        return false;
    }
    
    const auto& certifications = certIt->second;
    return std::find(certifications.begin(), certifications.end(), equipmentType) != certifications.end();
}

std::vector<std::string> TrainingCoordinator::getRequiredTraining(const std::string& employeeId) const {
    std::vector<std::string> requiredTraining;
    
    auto certIt = employeeCertifications.find(employeeId);
    if (certIt == employeeCertifications.end()) {
        return requiredCertifications;
    }
    
    for (const auto& requiredCert : requiredCertifications) {
        bool hasCertification = std::find(certIt->second.begin(), certIt->second.end(), requiredCert) != certIt->second.end();
        if (!hasCertification) {
            requiredTraining.push_back(requiredCert);
        }
    }
    
    return requiredTraining;
}

double TrainingCoordinator::calculateTrainingROI(const std::string& sessionId) const {
    auto sessionIt = trainingSessions.find(sessionId);
    if (sessionIt == trainingSessions.end() || !sessionIt->second.completed) {
        return 0.0;
    }
    
    const TrainingSession& session = sessionIt->second;
    double trainingCost = session.cost;
    double estimatedBenefit = session.participantIds.size() * 500.0;
    
    if (session.trainingType == "TECHNICAL") {
        estimatedBenefit *= 1.5;
    } else if (session.trainingType == "SAFETY") {
        estimatedBenefit *= 2.0;
    }
    
    return (estimatedBenefit - trainingCost) / trainingCost;
}

bool TrainingCoordinator::isCertificationRequired(const std::string& equipmentType) const {
    return std::find(requiredCertifications.begin(), requiredCertifications.end(), equipmentType) != requiredCertifications.end();
}

void TrainingCoordinator::addEmployeeCertification(const std::string& employeeId, const std::string& certification) {
    employeeCertifications[employeeId].push_back(certification);
}

std::vector<std::string> TrainingCoordinator::getCertifiedEmployees(const std::string& equipmentType) const {
    std::vector<std::string> certifiedEmployees;
    
    for (const auto& [employeeId, certifications] : employeeCertifications) {
        if (std::find(certifications.begin(), certifications.end(), equipmentType) != certifications.end()) {
            certifiedEmployees.push_back(employeeId);
        }
    }
    
    return certifiedEmployees;
}

std::shared_ptr<EmployeeManager> TrainingCoordinator::getEmployeeManager() const {
    return employeeManager;
}

std::vector<std::string> TrainingCoordinator::getEmployeeCertifications(const std::string& employeeId) const {
    auto it = employeeCertifications.find(employeeId);
    return it != employeeCertifications.end() ? it->second : std::vector<std::string>();
}

int TrainingCoordinator::getTrainingSessionCount() const {
    return trainingSessions.size();
}

void TrainingCoordinator::setEmployeeManager(std::shared_ptr<EmployeeManager> manager) {
    employeeManager = manager;
}

void TrainingCoordinator::addRequiredCertification(const std::string& certification) {
    if (std::find(requiredCertifications.begin(), requiredCertifications.end(), certification) == requiredCertifications.end()) {
        requiredCertifications.push_back(certification);
    }
}

void TrainingCoordinator::setTrainingCost(const std::string& trainingType, double cost) {
    trainingCosts[trainingType] = std::max(0.0, cost);
}