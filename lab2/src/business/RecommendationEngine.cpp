#include "../../include/business/RecommendationEngine.h"
#include "../../include/business/CustomerManager.h"
#include "../../include/core/Inventory.h"
#include "../../include/utils/DateUtils.h"
#include <algorithm>
#include <cmath>
#include <random>

RecommendationEngine::RecommendationEngine() 
    : customerManager(nullptr), inventory(nullptr) {
    
    categoryAssociations["CAMERA"] = {"TRIPOD", "LENS", "LIGHTING", "MEMORY_CARD"};
    categoryAssociations["CONSTRUCTION"] = {"TOOLS", "SAFETY_GEAR", "MEASURING_TOOLS"};
    categoryAssociations["SPORTS"] = {"OUTDOOR_GEAR", "PROTECTIVE_EQUIPMENT", "TRAINING_EQUIPMENT"};
    categoryAssociations["ELECTRONICS"] = {"ACCESSORIES", "POWER_BANKS", "CASES"};
    categoryAssociations["VEHICLE"] = {"NAVIGATION", "SAFETY_KIT", "CLEANING_SUPPLIES"};
    
    seasonalTrends["SUMMER_SPORTS"] = 0.9;
    seasonalTrends["WINTER_SPORTS"] = 0.8;
    seasonalTrends["GARDEN_EQUIPMENT"] = 0.7;
    seasonalTrends["HOME_IMPROVEMENT"] = 0.6;
    seasonalTrends["ELECTRONICS"] = 0.5;
    
    itemPopularity["CAM001"] = 0.8;
    itemPopularity["TOOL001"] = 0.6;
    itemPopularity["VEH001"] = 0.9;
}

RecommendationEngine::RecommendationEngine(std::shared_ptr<CustomerManager> custManager, std::shared_ptr<Inventory> inv)
    : customerManager(custManager), inventory(inv) {
    
    categoryAssociations["CAMERA"] = {"TRIPOD", "LENS", "LIGHTING", "MEMORY_CARD"};
    categoryAssociations["CONSTRUCTION"] = {"TOOLS", "SAFETY_GEAR", "MEASURING_TOOLS"};
    categoryAssociations["SPORTS"] = {"OUTDOOR_GEAR", "PROTECTIVE_EQUIPMENT", "TRAINING_EQUIPMENT"};
    categoryAssociations["ELECTRONICS"] = {"ACCESSORIES", "POWER_BANKS", "CASES"};
    categoryAssociations["VEHICLE"] = {"NAVIGATION", "SAFETY_KIT", "CLEANING_SUPPLIES"};
    
    seasonalTrends["SUMMER_SPORTS"] = 0.9;
    seasonalTrends["WINTER_SPORTS"] = 0.8;
    seasonalTrends["GARDEN_EQUIPMENT"] = 0.7;
    seasonalTrends["HOME_IMPROVEMENT"] = 0.6;
    seasonalTrends["ELECTRONICS"] = 0.5;
}

std::vector<Recommendation> RecommendationEngine::generateRecommendations(const std::string& customerId, 
                                                                        const std::vector<std::string>& rentalHistory) const {
    std::vector<Recommendation> recommendations;
    
    std::vector<std::string> preferredCategories;
    for (const auto& item : rentalHistory) {
        if (item.find("CAM") != std::string::npos) preferredCategories.push_back("PHOTOGRAPHY");
        else if (item.find("CONST") != std::string::npos) preferredCategories.push_back("CONSTRUCTION");
        else if (item.find("SPORT") != std::string::npos) preferredCategories.push_back("SPORTS");
        else if (item.find("ELEC") != std::string::npos) preferredCategories.push_back("ELECTRONICS");
        else if (item.find("VEH") != std::string::npos) preferredCategories.push_back("VEHICLE");
    }
    
    for (const auto& category : preferredCategories) {
        auto complementaryItems = findComplementaryItems(category);
        
        for (const auto& item : complementaryItems) {
            double score = calculateRecommendationScore(item, customerId, "CURRENT");
            
            if (score > 0.3) {
                Recommendation rec;
                rec.itemId = item;
                rec.category = category;
                rec.relevanceScore = score;
                rec.reason = "Based on your rental history";
                
                recommendations.push_back(rec);
            }
        }
    }
    
    std::sort(recommendations.begin(), recommendations.end(), 
              [](const Recommendation& a, const Recommendation& b) {
                  return a.relevanceScore > b.relevanceScore;
              });
    
    if (recommendations.size() > 5) {
        recommendations.resize(5);
    }
    
    return recommendations;
}

double RecommendationEngine::calculateRecommendationScore(const std::string& itemId, const std::string& customerId, 
                                                        const std::string& season) const {
    double score = 0.0;
    
    score += 0.3;
    
    if (isTrendingItem(itemId, season)) {
        score += 0.2;
    }
    
    auto prefIt = customerPreferences.find(customerId);
    if (prefIt != customerPreferences.end()) {
        const auto& preferences = prefIt->second;
        for (const auto& preference : preferences) {
            if (itemId.find(preference) != std::string::npos) {
                score += 0.3;
                break;
            }
        }
    }
    
    double satisfactionScore = predictCustomerSatisfaction(itemId, customerId);
    score += satisfactionScore * 0.2;
    
    auto popularityIt = itemPopularity.find(itemId);
    if (popularityIt != itemPopularity.end()) {
        score += popularityIt->second * 0.1;
    }
    
    return std::min(1.0, score);
}

std::vector<std::string> RecommendationEngine::findComplementaryItems(const std::string& mainItemId) const {
    std::vector<std::string> complementaryItems;
    
    std::string mainCategory = "GENERAL";
    if (mainItemId.find("CAM") != std::string::npos) mainCategory = "PHOTOGRAPHY";
    else if (mainItemId.find("CONST") != std::string::npos) mainCategory = "CONSTRUCTION";
    else if (mainItemId.find("SPORT") != std::string::npos) mainCategory = "SPORTS";
    else if (mainItemId.find("ELEC") != std::string::npos) mainCategory = "ELECTRONICS";
    else if (mainItemId.find("VEH") != std::string::npos) mainCategory = "VEHICLE";
    
    auto assocIt = categoryAssociations.find(mainCategory);
    if (assocIt != categoryAssociations.end()) {
        complementaryItems = assocIt->second;
    }
    
    complementaryItems.push_back("EXTENDED_WARRANTY");
    complementaryItems.push_back("DELIVERY_SERVICE");
    complementaryItems.push_back("SETUP_ASSISTANCE");
    
    return complementaryItems;
}

bool RecommendationEngine::isTrendingItem(const std::string& itemId, const std::string& season) const {
    std::string itemCategory = "GENERAL";
    if (itemId.find("SPORT") != std::string::npos) {
        if (season == "SUMMER" && itemId.find("WATER") != std::string::npos) {
            return true;
        } else if (season == "WINTER" && itemId.find("SKI") != std::string::npos) {
            return true;
        }
    } else if (itemId.find("GARDEN") != std::string::npos && season == "SPRING") {
        return true;
    } else if (itemId.find("HOME") != std::string::npos && season == "AUTUMN") {
        return true;
    }
    
    for (const auto& [trend, score] : seasonalTrends) {
        if (itemId.find(trend) != std::string::npos && score > 0.7) {
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> RecommendationEngine::getPersonalizedSuggestions(const std::string& customerId, 
                                                                         int maxSuggestions) const {
    std::vector<std::string> suggestions;
    
    auto prefIt = customerPreferences.find(customerId);
    if (prefIt == customerPreferences.end()) {
        return suggestions;
    }
    
    const auto& preferences = prefIt->second;
    
    for (const auto& preference : preferences) {
        auto complementary = findComplementaryItems(preference);
        suggestions.insert(suggestions.end(), complementary.begin(), complementary.end());
        
        if (suggestions.size() >= static_cast<size_t>(maxSuggestions)) {
            break;
        }
    }
    
    std::sort(suggestions.begin(), suggestions.end());
    suggestions.erase(std::unique(suggestions.begin(), suggestions.end()), suggestions.end());
    
    if (suggestions.size() > static_cast<size_t>(maxSuggestions)) {
        suggestions.resize(maxSuggestions);
    }
    
    return suggestions;
}

double RecommendationEngine::predictCustomerSatisfaction(const std::string& itemId, const std::string& customerId) const {
    double baseSatisfaction = 0.7;
    
    auto prefIt = customerPreferences.find(customerId);
    if (prefIt != customerPreferences.end()) {
        const auto& preferences = prefIt->second;
        for (const auto& preference : preferences) {
            if (itemId.find(preference) != std::string::npos) {
                baseSatisfaction += 0.2;
                break;
            }
        }
    }
    
    if (itemId.find("PROFESSIONAL") != std::string::npos || 
        itemId.find("ADVANCED") != std::string::npos) {
        baseSatisfaction -= 0.1;
    }
    
    double averageRating = 4.2;
    baseSatisfaction += (averageRating - 3.0) * 0.05;
    
    return std::max(0.0, std::min(1.0, baseSatisfaction));
}

std::vector<Recommendation> RecommendationEngine::getCrossSellRecommendations(const std::string& customerId) const {
    std::vector<Recommendation> crossSell;
    
    auto suggestions = getPersonalizedSuggestions(customerId, 10);
    for (const auto& item : suggestions) {
        double score = calculateRecommendationScore(item, customerId, "CURRENT");
        
        if (score > 0.4) {
            Recommendation rec;
            rec.itemId = item;
            rec.category = "CROSS_SELL";
            rec.relevanceScore = score;
            rec.reason = "Frequently rented together";
            
            crossSell.push_back(rec);
        }
    }
    
    return crossSell;
}

std::vector<Recommendation> RecommendationEngine::getSeasonalRecommendations(const std::string& season) const {
    std::vector<Recommendation> seasonalRecs;
    
    for (const auto& [trend, score] : seasonalTrends) {
        if (score > 0.7) {
            Recommendation rec;
            rec.itemId = trend + "_ITEM";
            rec.category = "SEASONAL";
            rec.relevanceScore = score;
            rec.reason = "Popular this season";
            
            seasonalRecs.push_back(rec);
        }
    }
    
    return seasonalRecs;
}

void RecommendationEngine::updateCustomerPreferences(const std::string& customerId, 
                                                   const std::vector<std::string>& preferences) {
    customerPreferences[customerId] = preferences;
}

std::shared_ptr<CustomerManager> RecommendationEngine::getCustomerManager() const {
    return customerManager;
}

std::shared_ptr<Inventory> RecommendationEngine::getInventory() const {
    return inventory;
}

void RecommendationEngine::setCustomerManager(std::shared_ptr<CustomerManager> manager) {
    customerManager = manager;
}

void RecommendationEngine::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void RecommendationEngine::updateItemPopularity(const std::string& itemId, double popularity) {
    itemPopularity[itemId] = std::max(0.0, std::min(1.0, popularity));
}