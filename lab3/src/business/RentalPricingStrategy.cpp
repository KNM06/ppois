#include "../../include/business/RentalPricingStrategy.h"
#include <algorithm>
#include <cmath>

RentalPricingStrategy::RentalPricingStrategy(const std::string& id, const std::string& name,
                                             const std::string& description, double baseMultiplier,
                                             double locationFactor, double amenityFactor,
                                             double marketAdjustment, int minLeaseTerm, bool dynamicPricing)
    : strategyId(id), strategyName(name), description(description),
      baseMultiplier(baseMultiplier), locationFactor(locationFactor),
      amenityFactor(amenityFactor), marketAdjustment(marketAdjustment),
      minimumLeaseTerm(minLeaseTerm), dynamicPricingEnabled(dynamicPricing) {}

RentalPricingStrategy::RentalPricingStrategy(const RentalPricingStrategy& other)
    : strategyId(other.strategyId), strategyName(other.strategyName),
      description(other.description), baseMultiplier(other.baseMultiplier),
      locationFactor(other.locationFactor), amenityFactor(other.amenityFactor),
      marketAdjustment(other.marketAdjustment), minimumLeaseTerm(other.minimumLeaseTerm),
      dynamicPricingEnabled(other.dynamicPricingEnabled) {}

RentalPricingStrategy& RentalPricingStrategy::operator=(const RentalPricingStrategy& other) {
    if (this != &other) {
        strategyId = other.strategyId;
        strategyName = other.strategyName;
        description = other.description;
        baseMultiplier = other.baseMultiplier;
        locationFactor = other.locationFactor;
        amenityFactor = other.amenityFactor;
        marketAdjustment = other.marketAdjustment;
        minimumLeaseTerm = other.minimumLeaseTerm;
        dynamicPricingEnabled = other.dynamicPricingEnabled;
    }
    return *this;
}

RentalPricingStrategy::RentalPricingStrategy(RentalPricingStrategy&& other) noexcept
    : strategyId(std::move(other.strategyId)), strategyName(std::move(other.strategyName)),
      description(std::move(other.description)), baseMultiplier(other.baseMultiplier),
      locationFactor(other.locationFactor), amenityFactor(other.amenityFactor),
      marketAdjustment(other.marketAdjustment), minimumLeaseTerm(other.minimumLeaseTerm),
      dynamicPricingEnabled(other.dynamicPricingEnabled) {}

RentalPricingStrategy& RentalPricingStrategy::operator=(RentalPricingStrategy&& other) noexcept {
    if (this != &other) {
        strategyId = std::move(other.strategyId);
        strategyName = std::move(other.strategyName);
        description = std::move(other.description);
        baseMultiplier = other.baseMultiplier;
        locationFactor = other.locationFactor;
        amenityFactor = other.amenityFactor;
        marketAdjustment = other.marketAdjustment;
        minimumLeaseTerm = other.minimumLeaseTerm;
        dynamicPricingEnabled = other.dynamicPricingEnabled;
    }
    return *this;
}

RentalPricingStrategy::~RentalPricingStrategy() {}

std::string RentalPricingStrategy::getStrategyId() const { return strategyId; }
std::string RentalPricingStrategy::getStrategyName() const { return strategyName; }
std::string RentalPricingStrategy::getDescription() const { return description; }
double RentalPricingStrategy::getBaseMultiplier() const { return baseMultiplier; }
double RentalPricingStrategy::getLocationFactor() const { return locationFactor; }
double RentalPricingStrategy::getAmenityFactor() const { return amenityFactor; }
double RentalPricingStrategy::getMarketAdjustment() const { return marketAdjustment; }
int RentalPricingStrategy::getMinimumLeaseTerm() const { return minimumLeaseTerm; }
bool RentalPricingStrategy::getDynamicPricingEnabled() const { return dynamicPricingEnabled; }

void RentalPricingStrategy::setBaseMultiplier(double newMultiplier) {
    baseMultiplier = std::max(0.5, newMultiplier);
}

void RentalPricingStrategy::setLocationFactor(double newFactor) {
    locationFactor = std::max(0.5, std::min(3.0, newFactor));
}

void RentalPricingStrategy::setMarketAdjustment(double newAdjustment) {
    marketAdjustment = std::max(0.7, std::min(1.5, newAdjustment));
}

double RentalPricingStrategy::calculateRentalPrice(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    
    double basePrice = property->getArea() * baseMultiplier;
    double locationPremium = calculateLocationPremium("standard");
    double amenityValue = calculateAmenityValue(property);
    
    return basePrice * locationFactor * marketAdjustment + amenityValue;
}

double RentalPricingStrategy::calculateLocationPremium(const std::string& locationTier) const {
    if (locationTier == "premium") return 1.5;
    if (locationTier == "standard") return 1.0;
    if (locationTier == "economy") return 0.7;
    return 1.0;
}

double RentalPricingStrategy::calculateAmenityValue(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    
    double baseValue = property->getRentalPrice() * 0.1;
    return baseValue * amenityFactor;
}

double RentalPricingStrategy::applyMarketConditions(double basePrice, const std::string& marketTrend) const {
    if (marketTrend == "hot") return basePrice * 1.2;
    if (marketTrend == "stable") return basePrice;
    if (marketTrend == "slow") return basePrice * 0.9;
    return basePrice * marketAdjustment;
}

bool RentalPricingStrategy::isPriceCompetitive(double proposedPrice, double marketAverage) const {
    double lowerBound = marketAverage * 0.9;
    double upperBound = marketAverage * 1.1;
    return proposedPrice >= lowerBound && proposedPrice <= upperBound;
}

double RentalPricingStrategy::calculateOptimalPriceRange(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    
    double basePrice = calculateRentalPrice(property);
    return basePrice * 0.1;
}

std::string RentalPricingStrategy::getPricingTier(double calculatedPrice) const {
    if (calculatedPrice < 1000.0) return "economy";
    if (calculatedPrice < 3000.0) return "mid_range";
    if (calculatedPrice < 7000.0) return "premium";
    return "luxury";
}

bool RentalPricingStrategy::shouldAdjustPrice(double currentPrice, double marketAverage) const {
    if (!dynamicPricingEnabled) return false;
    
    double variance = std::abs(currentPrice - marketAverage) / marketAverage;
    return variance > 0.15;
}

double RentalPricingStrategy::calculatePriceSensitivity(double priceChange) const {
    return std::abs(priceChange) * 100.0;
}