#include "../../include/business/CustomerManager.h"
#include "../../include/core/Customer.h"
#include "../../include/core/RentalHistory.h"
#include <algorithm>
#include <cmath>

CustomerManager::CustomerManager() 
    : recommendationEngine(nullptr), lateFeeCalculator(nullptr) {
    rentalLimits["NEW"] = 1;
    rentalLimits["REGULAR"] = 3;
    rentalLimits["VIP"] = 5;
    rentalLimits["BUSINESS"] = 10;
}

CustomerManager::CustomerManager(std::shared_ptr<RecommendationEngine> recommender, 
                               std::shared_ptr<LateFeeCalculator> lateCalculator)
    : recommendationEngine(recommender), lateFeeCalculator(lateCalculator) {
    rentalLimits["NEW"] = 1;
    rentalLimits["REGULAR"] = 3;
    rentalLimits["VIP"] = 5;
    rentalLimits["BUSINESS"] = 10;
}

bool CustomerManager::isCustomerEligible(const std::string& customerId) const {
    if (std::find(blacklistedCustomers.begin(), blacklistedCustomers.end(), customerId) != blacklistedCustomers.end()) {
        return false;
    }
    
    auto customerIt = customers.find(customerId);
    if (customerIt == customers.end()) {
        return false;
    }
    
    CustomerStatus status = customerIt->second->getStatus();
    if (status == CustomerStatus::BLACKLISTED) {
        return false;
    }
    
    double creditScore = customerIt->second->getCreditScore();
    if (creditScore < 400) {
        return false;
    }
    
    return true;
}

void CustomerManager::updateCustomerStatus(const std::string& customerId, CustomerStatus newStatus) {
    auto customerIt = customers.find(customerId);
    if (customerIt != customers.end()) {
        customerIt->second->setStatus(newStatus);
        
        if (newStatus == CustomerStatus::BLACKLISTED) {
            addToBlacklist(customerId);
        } else {
            removeFromBlacklist(customerId);
        }
    }
}

double CustomerManager::calculateCustomerLifetimeValue(const std::string& customerId) const {
    auto customerIt = customers.find(customerId);
    if (customerIt == customers.end()) {
        return 0.0;
    }
    
    std::shared_ptr<Customer> customer = customerIt->second;
    double avgRentalValue = 100.0;
    double rentalFrequency = 4.0;
    double customerLifespan = 3.0;
    
    double statusMultiplier = 1.0;
    switch (customer->getStatus()) {
        case CustomerStatus::VIP: statusMultiplier = 1.5; break;
        case CustomerStatus::REGULAR: statusMultiplier = 1.2; break;
        case CustomerStatus::NEW: statusMultiplier = 0.8; break;
        default: statusMultiplier = 0.5; break;
    }
    
    double creditMultiplier = customer->getCreditScore() / 850.0;
    double baseLTV = avgRentalValue * rentalFrequency * customerLifespan;
    double adjustedLTV = baseLTV * statusMultiplier * creditMultiplier;
    
    return adjustedLTV;
}

bool CustomerManager::validateRentalLimit(const std::string& customerId) const {
    auto customerIt = customers.find(customerId);
    if (customerIt == customers.end()) {
        return false;
    }
    
    CustomerStatus status = customerIt->second->getStatus();
    std::string statusStr;
    switch (status) {
        case CustomerStatus::NEW: statusStr = "NEW"; break;
        case CustomerStatus::REGULAR: statusStr = "REGULAR"; break;
        case CustomerStatus::VIP: statusStr = "VIP"; break;
        case CustomerStatus::BLACKLISTED: statusStr = "BLACKLISTED"; break;
    }
    
    auto limitIt = rentalLimits.find(statusStr);
    if (limitIt == rentalLimits.end()) {
        return false;
    }
    
    int maxRentals = limitIt->second;
    int currentRentals = 0;
    
    return currentRentals < maxRentals;
}

void CustomerManager::addToBlacklist(const std::string& customerId) {
    if (std::find(blacklistedCustomers.begin(), blacklistedCustomers.end(), customerId) == blacklistedCustomers.end()) {
        blacklistedCustomers.push_back(customerId);
    }
}

void CustomerManager::removeFromBlacklist(const std::string& customerId) {
    auto it = std::find(blacklistedCustomers.begin(), blacklistedCustomers.end(), customerId);
    if (it != blacklistedCustomers.end()) {
        blacklistedCustomers.erase(it);
    }
}

std::vector<std::string> CustomerManager::getVIPCustomers() const {
    std::vector<std::string> vipCustomers;
    
    for (const auto& [customerId, customer] : customers) {
        if (customer->getStatus() == CustomerStatus::VIP) {
            vipCustomers.push_back(customerId);
        }
    }
    
    return vipCustomers;
}

double CustomerManager::predictCustomerRetention(const std::string& customerId) const {
    auto customerIt = customers.find(customerId);
    if (customerIt == customers.end()) {
        return 0.0;
    }
    
    std::shared_ptr<Customer> customer = customerIt->second;
    double baseRetentionRate = 0.7;
    
    double statusBonus = 0.0;
    switch (customer->getStatus()) {
        case CustomerStatus::VIP: statusBonus = 0.3; break;
        case CustomerStatus::REGULAR: statusBonus = 0.15; break;
        case CustomerStatus::NEW: statusBonus = -0.1; break;
        default: statusBonus = -0.5; break;
    }
    
    double creditBonus = (customer->getCreditScore() - 500) / 1000.0;
    double loyaltyBonus = std::min(0.2, customer->getLoyaltyPoints() * 0.0001);
    double predictedRetention = baseRetentionRate + statusBonus + creditBonus + loyaltyBonus;
    
    return std::max(0.0, std::min(1.0, predictedRetention));
}

double CustomerManager::calculateCustomerRiskScore(const std::string& customerId) const {
    auto riskIt = customerRiskScores.find(customerId);
    if (riskIt != customerRiskScores.end()) {
        return riskIt->second;
    }
    
    auto customerIt = customers.find(customerId);
    if (customerIt == customers.end()) {
        return 1.0;
    }
    
    std::shared_ptr<Customer> customer = customerIt->second;
    double riskScore = 0.0;
    
    if (customer->getStatus() == CustomerStatus::NEW) {
        riskScore += 0.3;
    }
    
    if (customer->getCreditScore() < 600) {
        riskScore += 0.4;
    }
    
    if (customer->getLoyaltyPoints() < 100) {
        riskScore += 0.2;
    }
    
    return std::min(1.0, riskScore);
}

std::vector<std::string> CustomerManager::getCustomersForPromotion() const {
    std::vector<std::string> promotionCustomers;
    
    for (const auto& [customerId, customer] : customers) {
        if (customer->getStatus() == CustomerStatus::REGULAR && 
            customer->getLoyaltyPoints() > 500 &&
            calculateCustomerRiskScore(customerId) < 0.5) {
            promotionCustomers.push_back(customerId);
        }
    }
    
    return promotionCustomers;
}

void CustomerManager::applyLoyaltyBonus(const std::string& customerId, double purchaseAmount) {
    auto customerIt = customers.find(customerId);
    if (customerIt != customers.end()) {
        int bonusPoints = static_cast<int>(purchaseAmount / 10.0);
        customerIt->second->addLoyaltyPoints(bonusPoints);
    }
}

std::shared_ptr<Customer> CustomerManager::getCustomer(const std::string& customerId) const {
    auto it = customers.find(customerId);
    if (it != customers.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<RecommendationEngine> CustomerManager::getRecommendationEngine() const {
    return recommendationEngine;
}

std::shared_ptr<LateFeeCalculator> CustomerManager::getLateFeeCalculator() const {
    return lateFeeCalculator;
}

void CustomerManager::setRecommendationEngine(std::shared_ptr<RecommendationEngine> engine) {
    recommendationEngine = engine;
}

void CustomerManager::setLateFeeCalculator(std::shared_ptr<LateFeeCalculator> calculator) {
    lateFeeCalculator = calculator;
}

void CustomerManager::addCustomer(std::shared_ptr<Customer> customer) {
    customers[customer->getCustomerId()] = customer;
}

void CustomerManager::updateCustomerRiskScore(const std::string& customerId, double score) {
    customerRiskScores[customerId] = std::max(0.0, std::min(1.0, score));
}