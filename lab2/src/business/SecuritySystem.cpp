#include "../../include/business/SecuritySystem.h"
#include <algorithm>
#include <cmath>

SecuritySystem::SecuritySystem(std::shared_ptr<RentalSystem> rentalSys, 
                               std::shared_ptr<CustomerManager> custMgr)
    : rentalSystem(rentalSys), customerManager(custMgr), maxFailedAttempts(5) {}

bool SecuritySystem::detectFraudulentActivity(const std::string& customerId, double transactionAmount) {
    auto customer = customerManager->getCustomer(customerId);
    if (!customer) return false;
    
    double customerValue = customerManager->calculateCustomerLifetimeValue(customerId);
    double riskScore = calculateRiskScore(customerId, "GENERAL");
    
    bool unusualAmount = transactionAmount > customerValue * 2.0;
    bool highRiskCustomer = riskScore > 0.7;
    bool multipleSuspiciousActivities = suspiciousActivityCount[customerId] > 3;
    
    if (unusualAmount || highRiskCustomer || multipleSuspiciousActivities) {
        logSecurityEvent("FRAUD_DETECTED", "Suspicious activity detected for customer: " + customerId);
        suspiciousActivityCount[customerId]++;
        return true;
    }
    
    return false;
}

void SecuritySystem::logSecurityEvent(const std::string& eventType, const std::string& description) {
    std::string logEntry = "[" + eventType + "] " + description;
    securityLogs[eventType].push_back(logEntry);
}

bool SecuritySystem::verifyCustomerIdentity(const std::string& customerId, const std::string& verificationData) {
    auto customer = customerManager->getCustomer(customerId);
    if (!customer) return false;
    
    double riskScore = calculateRiskScore(customerId, "GENERAL");
    bool requiresVerification = riskScore > 0.5 || customer->getTotalSpent() > 1000.0;
    
    if (requiresVerification) {
        bool verificationValid = verificationData.length() > 5 && 
                                verificationData.find("VERIFIED") != std::string::npos;
        return verificationValid;
    }
    
    return true;
}

double SecuritySystem::calculateRiskScore(const std::string& customerId, const std::string& itemCategory) {
    double riskScore = 0.0;
    
    auto customer = customerManager->getCustomer(customerId);
    if (!customer) return 1.0;
    
    double creditRisk = (850.0 - customer->getCreditScore()) / 850.0 * 0.4;
    double activityRisk = suspiciousActivityCount[customerId] * 0.1;
    double valueRisk = customer->getTotalSpent() > 5000.0 ? 0.1 : 0.0;
    
    if (itemCategory == "LUXURY" || itemCategory == "VEHICLE") {
        riskScore += 0.2;
    }
    
    riskScore += creditRisk + activityRisk + valueRisk;
    return std::min(1.0, riskScore);
}

bool SecuritySystem::shouldRequireAdditionalVerification(const std::string& customerId, double itemValue) {
    double riskScore = calculateRiskScore(customerId, "GENERAL");
    bool highValueItem = itemValue > 1000.0;
    bool newCustomer = customerManager->getCustomer(customerId)->getTotalSpent() < 100.0;
    
    return riskScore > 0.6 || highValueItem || newCustomer;
}

std::vector<std::string> SecuritySystem::getSecurityAlerts() const {
    std::vector<std::string> alerts;
    
    for (const auto& [customerId, count] : suspiciousActivityCount) {
        if (count >= 3) {
            alerts.push_back("High suspicious activity for customer: " + customerId);
        }
    }
    
    if (alerts.empty()) {
        alerts.push_back("No security alerts");
    }
    
    return alerts;
}

void SecuritySystem::blockSuspiciousCustomer(const std::string& customerId) {
    customerManager->addToBlacklist(customerId);
    logSecurityEvent("CUSTOMER_BLOCKED", "Customer blocked due to suspicious activity: " + customerId);
}

bool SecuritySystem::isIPBlocked(const std::string& ipAddress) const {
    return std::find(blockedIPs.begin(), blockedIPs.end(), ipAddress) != blockedIPs.end();
}

void SecuritySystem::addToBlockList(const std::string& identifier) {
    if (std::find(blockedIPs.begin(), blockedIPs.end(), identifier) == blockedIPs.end()) {
        blockedIPs.push_back(identifier);
    }
}

std::shared_ptr<RentalSystem> SecuritySystem::getRentalSystem() const {
    return rentalSystem;
}

std::shared_ptr<CustomerManager> SecuritySystem::getCustomerManager() const {
    return customerManager;
}

int SecuritySystem::getSuspiciousActivityCount(const std::string& customerId) const {
    auto it = suspiciousActivityCount.find(customerId);
    return it != suspiciousActivityCount.end() ? it->second : 0;
}

void SecuritySystem::setMaxFailedAttempts(int attempts) {
    maxFailedAttempts = std::max(1, attempts);
}

void SecuritySystem::setRentalSystem(std::shared_ptr<RentalSystem> system) {
    rentalSystem = system;
}

void SecuritySystem::setCustomerManager(std::shared_ptr<CustomerManager> manager) {
    customerManager = manager;
}