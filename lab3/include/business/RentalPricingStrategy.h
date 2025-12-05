#ifndef RENTALPRICINGSTRATEGY_H
#define RENTALPRICINGSTRATEGY_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"

class RentalPricingStrategy {
private:
    std::string strategyId;
    std::string strategyName;
    std::string description;
    double baseMultiplier;
    double locationFactor;
    double amenityFactor;
    double marketAdjustment;
    int minimumLeaseTerm;
    bool dynamicPricingEnabled;

public:
    RentalPricingStrategy(const std::string& id, const std::string& name,
                         const std::string& description, double baseMultiplier,
                         double locationFactor, double amenityFactor,
                         double marketAdjustment, int minLeaseTerm, bool dynamicPricing);
    
    RentalPricingStrategy(const RentalPricingStrategy& other);
    RentalPricingStrategy& operator=(const RentalPricingStrategy& other);
    RentalPricingStrategy(RentalPricingStrategy&& other) noexcept;
    RentalPricingStrategy& operator=(RentalPricingStrategy&& other) noexcept;
    ~RentalPricingStrategy();

    std::string getStrategyId() const;
    std::string getStrategyName() const;
    std::string getDescription() const;
    double getBaseMultiplier() const;
    double getLocationFactor() const;
    double getAmenityFactor() const;
    double getMarketAdjustment() const;
    int getMinimumLeaseTerm() const;
    bool getDynamicPricingEnabled() const;

    void setBaseMultiplier(double newMultiplier);
    void setLocationFactor(double newFactor);
    void setMarketAdjustment(double newAdjustment);

    double calculateRentalPrice(std::shared_ptr<Property> property) const;
    double calculateLocationPremium(const std::string& locationTier) const;
    double calculateAmenityValue(std::shared_ptr<Property> property) const;
    double applyMarketConditions(double basePrice, const std::string& marketTrend) const;
    bool isPriceCompetitive(double proposedPrice, double marketAverage) const;
    double calculateOptimalPriceRange(std::shared_ptr<Property> property) const;
    std::string getPricingTier(double calculatedPrice) const;
    bool shouldAdjustPrice(double currentPrice, double marketAverage) const;
    double calculatePriceSensitivity(double priceChange) const;
};

#endif