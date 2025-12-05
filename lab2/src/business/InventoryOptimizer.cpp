#include "../../include/business/InventoryOptimizer.h"
#include <algorithm>
#include <cmath>

InventoryOptimizer::InventoryOptimizer(std::shared_ptr<Inventory> inv, std::shared_ptr<AnalyticsEngine> analytics)
    : inventory(inv), analyticsEngine(analytics) {
    
    categoryOptimalLevels["VEHICLE"] = 0.75;
    categoryOptimalLevels["ELECTRONICS"] = 0.65;
    categoryOptimalLevels["CONSTRUCTION"] = 0.6;
    categoryOptimalLevels["TOOLS"] = 0.8;
    categoryOptimalLevels["SPORTS"] = 0.7;
    
    seasonalAdjustments["SUMMER"] = 20;
    seasonalAdjustments["WINTER"] = -10;
    seasonalAdjustments["SPRING"] = 5;
    seasonalAdjustments["AUTUMN"] = 0;
    
    highDemandCategories = {"ELECTRONICS", "SPORTS", "TOOLS"};
}

double InventoryOptimizer::calculateOptimalInventoryLevel(const std::string& category) const {
    auto levelIt = categoryOptimalLevels.find(category);
    double baseLevel = levelIt != categoryOptimalLevels.end() ? levelIt->second : 0.7;
    
    double demandFactor = 1.0;
    if (analyticsEngine) {
        double predictedDemand = analyticsEngine->predictDemandTrend(category, 30);
        demandFactor = predictedDemand / 50.0;
    }
    
    bool isHighDemand = std::find(highDemandCategories.begin(), highDemandCategories.end(), category) != highDemandCategories.end();
    if (isHighDemand) {
        baseLevel *= 1.2;
    }
    
    std::string currentSeason = "SUMMER";
    auto seasonIt = seasonalAdjustments.find(currentSeason);
    if (seasonIt != seasonalAdjustments.end()) {
        baseLevel *= (1.0 + seasonIt->second / 100.0);
    }
    
    return std::min(0.95, std::max(0.1, baseLevel));
}

std::vector<std::string> InventoryOptimizer::identifySlowMovingItems(double threshold) const {
    std::vector<std::string> slowMovingItems;
    
    auto utilization = inventory->getUtilizationByCategory();
    for (const auto& [category, util] : utilization) {
        if (util < threshold) {
            slowMovingItems.push_back(category);
        }
    }
    
    double overallUtilization = 0.0;
    if (!utilization.empty()) {
        for (const auto& [category, util] : utilization) {
            overallUtilization += util;
        }
        overallUtilization /= utilization.size();
    }
    
    if (overallUtilization < threshold * 0.8) {
        slowMovingItems.push_back("SYSTEM_WIDE: Low overall utilization");
    }
    
    return slowMovingItems;
}

bool InventoryOptimizer::shouldReorderItem(const std::string& itemId, int currentStock) const {
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    
    double optimalLevel = calculateOptimalInventoryLevel(category);
    int optimalStock = static_cast<int>(optimalLevel * 100);
    
    double stockoutRisk = calculateStockoutRisk(itemId);
    bool highRiskItem = stockoutRisk > 0.3;
    bool lowStock = currentStock < optimalStock * 0.3;
    
    return lowStock || highRiskItem;
}

double InventoryOptimizer::calculateCarryingCost(const std::string& category, int quantity) const {
    double baseCostPerUnit = 10.0;
    
    if (category == "ELECTRONICS") baseCostPerUnit = 15.0;
    else if (category == "VEHICLE") baseCostPerUnit = 25.0;
    else if (category == "CONSTRUCTION") baseCostPerUnit = 20.0;
    
    double storageCost = baseCostPerUnit * quantity * 0.1;
    double insuranceCost = baseCostPerUnit * quantity * 0.05;
    double maintenanceCost = baseCostPerUnit * quantity * 0.03;
    
    return storageCost + insuranceCost + maintenanceCost;
}

std::vector<std::string> InventoryOptimizer::optimizeCategoryDistribution() const {
    std::vector<std::string> recommendations;
    
    auto utilization = inventory->getUtilizationByCategory();
    auto slowMoving = identifySlowMovingItems(0.3);
    
    for (const auto& category : slowMoving) {
        recommendations.push_back("Reduce inventory for " + category + " - slow moving");
    }
    
    for (const auto& category : highDemandCategories) {
        auto utilIt = utilization.find(category);
        if (utilIt != utilization.end() && utilIt->second > 0.8) {
            recommendations.push_back("Increase inventory for " + category + " - high demand");
        }
    }
    
    double totalCarryingCost = 0.0;
    for (const auto& [category, util] : utilization) {
        totalCarryingCost += calculateCarryingCost(category, static_cast<int>(util * 100));
    }
    
    if (totalCarryingCost > 1000.0) {
        recommendations.push_back("High carrying costs detected - consider inventory reduction");
    }
    
    return recommendations;
}

double InventoryOptimizer::calculateStockoutRisk(const std::string& itemId) const {
    std::string category = "GENERAL";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    
    double demandFactor = 1.0;
    if (analyticsEngine) {
        double predictedDemand = analyticsEngine->predictDemandTrend(category, 7);
        demandFactor = predictedDemand / 50.0;
    }
    
    bool isHighDemandCategory = std::find(highDemandCategories.begin(), highDemandCategories.end(), category) != highDemandCategories.end();
    double baseRisk = isHighDemandCategory ? 0.3 : 0.1;
    
    double utilization = 0.0;
    auto utilMap = inventory->getUtilizationByCategory();
    auto utilIt = utilMap.find(category);
    if (utilIt != utilMap.end()) {
        utilization = utilIt->second;
    }
    
    double risk = baseRisk * demandFactor * (1.0 + utilization);
    return std::min(1.0, risk);
}

bool InventoryOptimizer::isOverstocked(const std::string& category, int currentQuantity) const {
    double optimalLevel = calculateOptimalInventoryLevel(category);
    int optimalQuantity = static_cast<int>(optimalLevel * 100);
    
    return currentQuantity > optimalQuantity * 1.5;
}

void InventoryOptimizer::updateOptimalLevels(const std::string& category, double level) {
    categoryOptimalLevels[category] = std::max(0.1, std::min(1.0, level));
}

std::shared_ptr<Inventory> InventoryOptimizer::getInventory() const {
    return inventory;
}

std::shared_ptr<AnalyticsEngine> InventoryOptimizer::getAnalyticsEngine() const {
    return analyticsEngine;
}

double InventoryOptimizer::getCategoryOptimalLevel(const std::string& category) const {
    auto it = categoryOptimalLevels.find(category);
    return it != categoryOptimalLevels.end() ? it->second : 0.7;
}

void InventoryOptimizer::setInventory(std::shared_ptr<Inventory> inv) {
    inventory = inv;
}

void InventoryOptimizer::setAnalyticsEngine(std::shared_ptr<AnalyticsEngine> analytics) {
    analyticsEngine = analytics;
}

void InventoryOptimizer::addHighDemandCategory(const std::string& category) {
    if (std::find(highDemandCategories.begin(), highDemandCategories.end(), category) == highDemandCategories.end()) {
        highDemandCategories.push_back(category);
    }
}