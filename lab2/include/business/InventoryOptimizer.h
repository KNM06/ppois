#ifndef INVENTORYOPTIMIZER_H
#define INVENTORYOPTIMIZER_H

#include "../../include/core/Inventory.h"
#include "../../include/business/AnalyticsEngine.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class InventoryOptimizer {
private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<AnalyticsEngine> analyticsEngine;
    std::unordered_map<std::string, double> categoryOptimalLevels;
    std::unordered_map<std::string, int> seasonalAdjustments;
    std::vector<std::string> highDemandCategories;

public:
    InventoryOptimizer(std::shared_ptr<Inventory> inv, std::shared_ptr<AnalyticsEngine> analytics);
    
    double calculateOptimalInventoryLevel(const std::string& category) const;
    std::vector<std::string> identifySlowMovingItems(double threshold) const;
    bool shouldReorderItem(const std::string& itemId, int currentStock) const;
    double calculateCarryingCost(const std::string& category, int quantity) const;
    std::vector<std::string> optimizeCategoryDistribution() const;
    double calculateStockoutRisk(const std::string& itemId) const;
    bool isOverstocked(const std::string& category, int currentQuantity) const;
    void updateOptimalLevels(const std::string& category, double level);
    
    std::shared_ptr<Inventory> getInventory() const;
    std::shared_ptr<AnalyticsEngine> getAnalyticsEngine() const;
    double getCategoryOptimalLevel(const std::string& category) const;
    
    void setInventory(std::shared_ptr<Inventory> inv);
    void setAnalyticsEngine(std::shared_ptr<AnalyticsEngine> analytics);
    void addHighDemandCategory(const std::string& category);
};

#endif