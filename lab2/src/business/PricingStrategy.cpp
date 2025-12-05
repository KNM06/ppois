#include "../../include/business/PricingStrategy.h"
#include "../../include/core/RentalPricing.h"
#include "../../include/business/DiscountCalculator.h"
#include "../../include/utils/DateUtils.h"
#include <cmath>
#include <algorithm>

PricingStrategy::PricingStrategy() 
    : rentalPricing(std::make_unique<RentalPricing>()), profitMargin(0.25), discountCalculator(nullptr) {
    
    demandMultipliers["VEHICLE"] = 1.3;
    demandMultipliers["ELECTRONICS"] = 1.1;
    demandMultipliers["CONSTRUCTION"] = 1.5;
    demandMultipliers["TOOLS"] = 1.0;
    demandMultipliers["SPORTS"] = 1.2;
    
    competitorPrices["VEHICLE"] = 70.0;
    competitorPrices["ELECTRONICS"] = 40.0;
    competitorPrices["CONSTRUCTION"] = 110.0;
    competitorPrices["TOOLS"] = 22.0;
    competitorPrices["SPORTS"] = 28.0;
    
    locationMultipliers["DOWNTOWN"] = 1.2;
    locationMultipliers["SUBURB"] = 0.9;
    locationMultipliers["AIRPORT"] = 1.5;
}

PricingStrategy::PricingStrategy(std::shared_ptr<DiscountCalculator> discountCalc)
    : rentalPricing(std::make_unique<RentalPricing>()), profitMargin(0.25), discountCalculator(discountCalc) {
    
    demandMultipliers["VEHICLE"] = 1.3;
    demandMultipliers["ELECTRONICS"] = 1.1;
    demandMultipliers["CONSTRUCTION"] = 1.5;
    demandMultipliers["TOOLS"] = 1.0;
    demandMultipliers["SPORTS"] = 1.2;
    
    competitorPrices["VEHICLE"] = 70.0;
    competitorPrices["ELECTRONICS"] = 40.0;
    competitorPrices["CONSTRUCTION"] = 110.0;
    competitorPrices["TOOLS"] = 22.0;
    competitorPrices["SPORTS"] = 28.0;
}

double PricingStrategy::calculateOptimalPrice(const std::string& category, int durationDays, 
                                            const std::string& customerType, double demandLevel) const {
    double basePrice = rentalPricing->calculateDynamicPrice(category, durationDays, "CURRENT", customerType);
    
    auto demandIt = demandMultipliers.find(category);
    double demandMultiplier = (demandIt != demandMultipliers.end()) ? demandIt->second : 1.0;
    double demandAdjustedPrice = basePrice * (1.0 + (demandLevel * demandMultiplier * 0.1));
    
    double competitionAdjustedPrice = demandAdjustedPrice;
    if (shouldAdjustPriceForCompetition(category)) {
        auto compIt = competitorPrices.find(category);
        if (compIt != competitorPrices.end()) {
            double competitorPrice = compIt->second * durationDays;
            if (competitionAdjustedPrice > competitorPrice * 1.05) {
                competitionAdjustedPrice = competitorPrice * 0.95;
            }
        }
    }
    
    double finalPrice = competitionAdjustedPrice * (1.0 + profitMargin);
    
    double sensitivityFactor = calculatePriceSensitivity(category, finalPrice);
    finalPrice *= sensitivityFactor;
    
    return std::max(0.0, finalPrice);
}

bool PricingStrategy::shouldAdjustPriceForCompetition(const std::string& category) const {
    if (category == "VEHICLE" || category == "ELECTRONICS") {
        return true;
    }
    
    if (category == "SPORTS" || category == "CONSTRUCTION") {
        return true;
    }
    
    auto compIt = competitorPrices.find(category);
    if (compIt != competitorPrices.end()) {
        double ourBasePrice = rentalPricing->getBaseRate(category);
        double competitorPrice = compIt->second;
        double priceDifference = std::abs(ourBasePrice - competitorPrice) / ourBasePrice;
        
        return priceDifference > 0.15;
    }
    
    return false;
}

double PricingStrategy::calculatePriceSensitivity(const std::string& category, double proposedPrice) const {
    double sensitivityFactor = 1.0;
    
    if (category == "LUXURY" || category == "BUSINESS") {
        sensitivityFactor = 0.9;
    } else if (category == "BASIC" || category == "TOOLS") {
        sensitivityFactor = 1.1;
    }
    
    if (proposedPrice > 500) {
        sensitivityFactor *= 0.8;
    } else if (proposedPrice < 50) {
        sensitivityFactor *= 1.2;
    }
    
    return sensitivityFactor;
}

void PricingStrategy::updateCompetitorPrice(const std::string& category, double price) {
    competitorPrices[category] = std::max(0.0, price);
}

double PricingStrategy::calculateDynamicDiscount(int rentalCount, double totalValue) const {
    double baseDiscount = 0.0;
    
    if (rentalCount >= 10) {
        baseDiscount = 0.15;
    } else if (rentalCount >= 5) {
        baseDiscount = 0.10;
    } else if (rentalCount >= 3) {
        baseDiscount = 0.05;
    }
    
    double valueDiscount = 0.0;
    if (totalValue > 1000) {
        valueDiscount = 0.08;
    } else if (totalValue > 500) {
        valueDiscount = 0.05;
    } else if (totalValue > 200) {
        valueDiscount = 0.02;
    }
    
    return baseDiscount + valueDiscount;
}

bool PricingStrategy::isPeakPricingApplicable(const std::string& category, const std::string& timePeriod) const {
    if (timePeriod == "WEEKEND" || timePeriod == "HOLIDAY") {
        return category == "VEHICLE" || category == "SPORTS" || category == "PARTY";
    }
    
    if (timePeriod == "EVENING" || timePeriod == "MORNING") {
        return category == "ELECTRONICS" || category == "TOOLS";
    }
    
    if (timePeriod == "SUMMER" && category == "SPORTS") {
        return true;
    } else if (timePeriod == "SPRING" && category == "GARDEN") {
        return true;
    } else if (timePeriod == "WINTER" && category == "WINTER_SPORTS") {
        return true;
    }
    
    return false;
}

double PricingStrategy::calculateLocationBasedPrice(const std::string& category, const std::string& location) const {
    double basePrice = rentalPricing->getBaseRate(category);
    auto locationIt = locationMultipliers.find(location);
    double locationMultiplier = (locationIt != locationMultipliers.end()) ? locationIt->second : 1.0;
    
    return basePrice * locationMultiplier;
}

std::unordered_map<std::string, double> PricingStrategy::getPriceRecommendations() const {
    std::unordered_map<std::string, double> recommendations;
    
    for (const auto& [category, baseRate] : competitorPrices) {
        double optimalPrice = calculateOptimalPrice(category, 1, "REGULAR", 0.5);
        recommendations[category] = optimalPrice;
    }
    
    return recommendations;
}

std::shared_ptr<DiscountCalculator> PricingStrategy::getDiscountCalculator() const {
    return discountCalculator;
}

void PricingStrategy::setProfitMargin(double margin) {
    profitMargin = std::max(0.0, std::min(1.0, margin));
}

void PricingStrategy::setDiscountCalculator(std::shared_ptr<DiscountCalculator> calculator) {
    discountCalculator = calculator;
}

void PricingStrategy::setLocationMultiplier(const std::string& location, double multiplier) {
    locationMultipliers[location] = std::max(0.5, std::min(2.0, multiplier));
}