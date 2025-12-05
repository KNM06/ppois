#ifndef SECURITYSYSTEM_H
#define SECURITYSYSTEM_H

#include "../../include/core/RentalSystem.h"
#include "../../include/business/CustomerManager.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class SecuritySystem {
private:
    std::shared_ptr<RentalSystem> rentalSystem;
    std::shared_ptr<CustomerManager> customerManager;
    std::unordered_map<std::string, int> suspiciousActivityCount;
    std::unordered_map<std::string, std::vector<std::string>> securityLogs;
    std::vector<std::string> blockedIPs;
    int maxFailedAttempts;

public:
    SecuritySystem(std::shared_ptr<RentalSystem> rentalSys, 
                   std::shared_ptr<CustomerManager> custMgr);
    
    bool detectFraudulentActivity(const std::string& customerId, double transactionAmount);
    void logSecurityEvent(const std::string& eventType, const std::string& description);
    bool verifyCustomerIdentity(const std::string& customerId, const std::string& verificationData);
    double calculateRiskScore(const std::string& customerId, const std::string& itemCategory);
    bool shouldRequireAdditionalVerification(const std::string& customerId, double itemValue);
    std::vector<std::string> getSecurityAlerts() const;
    void blockSuspiciousCustomer(const std::string& customerId);
    bool isIPBlocked(const std::string& ipAddress) const;
    void addToBlockList(const std::string& identifier);
    
    std::shared_ptr<RentalSystem> getRentalSystem() const;
    std::shared_ptr<CustomerManager> getCustomerManager() const;
    int getSuspiciousActivityCount(const std::string& customerId) const;
    
    void setMaxFailedAttempts(int attempts);
    void setRentalSystem(std::shared_ptr<RentalSystem> system);
    void setCustomerManager(std::shared_ptr<CustomerManager> manager);
};

#endif