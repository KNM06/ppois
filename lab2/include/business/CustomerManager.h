#ifndef CUSTOMERMANAGER_H
#define CUSTOMERMANAGER_H

#include "../../include/core/Customer.h"
#include "../../include/core/RentalHistory.h"
#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/LateFeeCalculator.h"
#include <memory>
#include <unordered_map>
#include <vector>

class RecommendationEngine;
class LateFeeCalculator;

class CustomerManager {
private:
    std::unordered_map<std::string, std::shared_ptr<Customer>> customers;
    std::vector<std::string> blacklistedCustomers;
    std::unordered_map<std::string, int> rentalLimits;
    std::shared_ptr<RecommendationEngine> recommendationEngine;
    std::shared_ptr<LateFeeCalculator> lateFeeCalculator;
    std::unordered_map<std::string, double> customerRiskScores;

public:
    CustomerManager();
    CustomerManager(std::shared_ptr<RecommendationEngine> recommender, 
                   std::shared_ptr<LateFeeCalculator> lateCalculator);
    
    bool isCustomerEligible(const std::string& customerId) const;
    void updateCustomerStatus(const std::string& customerId, CustomerStatus newStatus);
    double calculateCustomerLifetimeValue(const std::string& customerId) const;
    bool validateRentalLimit(const std::string& customerId) const;
    void addToBlacklist(const std::string& customerId);
    void removeFromBlacklist(const std::string& customerId);
    std::vector<std::string> getVIPCustomers() const;
    double predictCustomerRetention(const std::string& customerId) const;
    double calculateCustomerRiskScore(const std::string& customerId) const;
    std::vector<std::string> getCustomersForPromotion() const;
    void applyLoyaltyBonus(const std::string& customerId, double purchaseAmount);
    
    std::shared_ptr<Customer> getCustomer(const std::string& customerId) const;
    std::shared_ptr<RecommendationEngine> getRecommendationEngine() const;
    std::shared_ptr<LateFeeCalculator> getLateFeeCalculator() const;
    
    void setRecommendationEngine(std::shared_ptr<RecommendationEngine> engine);
    void setLateFeeCalculator(std::shared_ptr<LateFeeCalculator> calculator);
    
    void addCustomer(std::shared_ptr<Customer> customer);
    void updateCustomerRiskScore(const std::string& customerId, double score);
};

#endif