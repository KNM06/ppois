#ifndef PRICINGSTRATEGY_H
#define PRICINGSTRATEGY_H

#include "../../include/core/RentalPricing.h"
#include "../../include/business/DiscountCalculator.h"
#include "../../include/utils/DateUtils.h"
#include <memory>
#include <string>
#include <unordered_map>

class RentalPricing;
class DiscountCalculator;

class PricingStrategy {
private:
    std::unique_ptr<RentalPricing> rentalPricing;
    std::shared_ptr<DiscountCalculator> discountCalculator;
    std::unordered_map<std::string, double> demandMultipliers;
    std::unordered_map<std::string, double> competitorPrices;
    std::unordered_map<std::string, double> locationMultipliers;
    double profitMargin;

public:
    PricingStrategy();
    PricingStrategy(std::shared_ptr<DiscountCalculator> discountCalc);
    
    double calculateOptimalPrice(const std::string& category, int durationDays, 
                                const std::string& customerType, double demandLevel) const;
    bool shouldAdjustPriceForCompetition(const std::string& category) const;
    double calculatePriceSensitivity(const std::string& category, double proposedPrice) const;
    void updateCompetitorPrice(const std::string& category, double price);
    double calculateDynamicDiscount(int rentalCount, double totalValue) const;
    bool isPeakPricingApplicable(const std::string& category, const std::string& timePeriod) const;
    double calculateLocationBasedPrice(const std::string& category, const std::string& location) const;
    std::unordered_map<std::string, double> getPriceRecommendations() const;
    
    double getProfitMargin() const { return profitMargin; }
    std::shared_ptr<DiscountCalculator> getDiscountCalculator() const;
    
    void setProfitMargin(double margin);
    void setDiscountCalculator(std::shared_ptr<DiscountCalculator> calculator);
    void setLocationMultiplier(const std::string& location, double multiplier);
};

#endif