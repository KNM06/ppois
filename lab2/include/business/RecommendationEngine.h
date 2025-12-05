#ifndef RECOMMENDATIONENGINE_H
#define RECOMMENDATIONENGINE_H

#include "../../include/business/CustomerManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/utils/DateUtils.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class CustomerManager;
class Inventory;

struct Recommendation {
    std::string itemId;
    std::string category;
    double relevanceScore;
    std::string reason;
};

class RecommendationEngine {
private:
    std::unordered_map<std::string, std::vector<std::string>> categoryAssociations;
    std::unordered_map<std::string, std::vector<std::string>> customerPreferences;
    std::unordered_map<std::string, double> seasonalTrends;
    std::shared_ptr<CustomerManager> customerManager;
    std::shared_ptr<Inventory> inventory;
    std::unordered_map<std::string, double> itemPopularity;

public:
    RecommendationEngine();
    RecommendationEngine(std::shared_ptr<CustomerManager> custManager, std::shared_ptr<Inventory> inv);
    
    std::vector<Recommendation> generateRecommendations(const std::string& customerId, 
                                                      const std::vector<std::string>& rentalHistory) const;
    double calculateRecommendationScore(const std::string& itemId, const std::string& customerId, 
                                      const std::string& season) const;
    std::vector<std::string> findComplementaryItems(const std::string& mainItemId) const;
    bool isTrendingItem(const std::string& itemId, const std::string& season) const;
    std::vector<std::string> getPersonalizedSuggestions(const std::string& customerId, 
                                                       int maxSuggestions) const;
    double predictCustomerSatisfaction(const std::string& itemId, const std::string& customerId) const;
    std::vector<Recommendation> getCrossSellRecommendations(const std::string& customerId) const;
    std::vector<Recommendation> getSeasonalRecommendations(const std::string& season) const;
    
    void updateCustomerPreferences(const std::string& customerId, const std::vector<std::string>& preferences);
    std::shared_ptr<CustomerManager> getCustomerManager() const;
    std::shared_ptr<Inventory> getInventory() const;
    void setCustomerManager(std::shared_ptr<CustomerManager> manager);
    void setInventory(std::shared_ptr<Inventory> inv);
    void updateItemPopularity(const std::string& itemId, double popularity);
};

#endif