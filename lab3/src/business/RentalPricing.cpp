#include "../../include/business/RentalPricing.h"
#include <algorithm>
#include <cmath>

RentalPricing::RentalPricing(double basePrice, double locationMultiplier, double amenityMultiplier,
                             double marketDemand, double seasonality, int minLeaseTerm,
                             const std::string& strategy)
    : basePricePerSqFt(basePrice), locationMultiplier(locationMultiplier),
      amenityMultiplier(amenityMultiplier), marketDemandFactor(marketDemand),
      seasonalityAdjustment(seasonality), minimumLeaseTerm(minLeaseTerm),
      pricingStrategy(strategy) {}

RentalPricing::RentalPricing(const RentalPricing& other)
    : basePricePerSqFt(other.basePricePerSqFt), locationMultiplier(other.locationMultiplier),
      amenityMultiplier(other.amenityMultiplier), marketDemandFactor(other.marketDemandFactor),
      seasonalityAdjustment(other.seasonalityAdjustment), minimumLeaseTerm(other.minimumLeaseTerm),
      pricingStrategy(other.pricingStrategy) {}

RentalPricing& RentalPricing::operator=(const RentalPricing& other) {
    if (this != &other) {
        basePricePerSqFt = other.basePricePerSqFt;
        locationMultiplier = other.locationMultiplier;
        amenityMultiplier = other.amenityMultiplier;
        marketDemandFactor = other.marketDemandFactor;
        seasonalityAdjustment = other.seasonalityAdjustment;
        minimumLeaseTerm = other.minimumLeaseTerm;
        pricingStrategy = other.pricingStrategy;
    }
    return *this;
}

RentalPricing::RentalPricing(RentalPricing&& other) noexcept
    : basePricePerSqFt(other.basePricePerSqFt), locationMultiplier(other.locationMultiplier),
      amenityMultiplier(other.amenityMultiplier), marketDemandFactor(other.marketDemandFactor),
      seasonalityAdjustment(other.seasonalityAdjustment), minimumLeaseTerm(other.minimumLeaseTerm),
      pricingStrategy(std::move(other.pricingStrategy)) {}

RentalPricing& RentalPricing::operator=(RentalPricing&& other) noexcept {
    if (this != &other) {
        basePricePerSqFt = other.basePricePerSqFt;
        locationMultiplier = other.locationMultiplier;
        amenityMultiplier = other.amenityMultiplier;
        marketDemandFactor = other.marketDemandFactor;
        seasonalityAdjustment = other.seasonalityAdjustment;
        minimumLeaseTerm = other.minimumLeaseTerm;
        pricingStrategy = std::move(other.pricingStrategy);
    }
    return *this;
}

RentalPricing::~RentalPricing() {}

double RentalPricing::getBasePricePerSqFt() const { return basePricePerSqFt; }
double RentalPricing::getLocationMultiplier() const { return locationMultiplier; }
double RentalPricing::getAmenityMultiplier() const { return amenityMultiplier; }
double RentalPricing::getMarketDemandFactor() const { return marketDemandFactor; }
double RentalPricing::getSeasonalityAdjustment() const { return seasonalityAdjustment; }
int RentalPricing::getMinimumLeaseTerm() const { return minimumLeaseTerm; }
std::string RentalPricing::getPricingStrategy() const { return pricingStrategy; }

void RentalPricing::setBasePricePerSqFt(double newPrice) {
    basePricePerSqFt = std::max(0.0, newPrice);
}

void RentalPricing::setLocationMultiplier(double newMultiplier) {
    locationMultiplier = std::max(0.5, std::min(3.0, newMultiplier));
}

void RentalPricing::setMarketDemandFactor(double newFactor) {
    marketDemandFactor = std::max(0.5, std::min(2.0, newFactor));
}

void RentalPricing::setSeasonalityAdjustment(double newAdjustment) {
    seasonalityAdjustment = std::max(0.7, std::min(1.3, newAdjustment));
}

double RentalPricing::calculateBaseRent(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    return property->getArea() * basePricePerSqFt;
}

double RentalPricing::calculateMarketAdjustedRent(double baseRent, const std::string& neighborhood) const {
    double multiplier = locationMultiplier;
    
    if (neighborhood == "premium" || neighborhood == "downtown") multiplier *= 1.5;
    else if (neighborhood == "suburban") multiplier *= 1.0;
    else if (neighborhood == "rural") multiplier *= 0.7;
    
    return baseRent * multiplier * marketDemandFactor;
}

double RentalPricing::calculateAmenityPremium(std::shared_ptr<Property> property) const {
    if (!property) return 0.0;
    return calculateBaseRent(property) * amenityMultiplier * 0.1;
}

double RentalPricing::calculateLocationPremium(const std::string& locationTier) const {
    if (locationTier == "premium") return 0.25;
    if (locationTier == "standard") return 0.0;
    if (locationTier == "economy") return -0.15;
    return 0.0;
}

double RentalPricing::calculateSeasonalAdjustment(int month) const {
    if (month >= 5 && month <= 8) return seasonalityAdjustment;
    if (month >= 11 || month <= 2) return 0.9;
    return 1.0;
}

double RentalPricing::calculateLongTermDiscount(int leaseMonths) const {
    if (leaseMonths >= 24) return 0.9;
    if (leaseMonths >= 12) return 0.95;
    return 1.0;
}

bool RentalPricing::isCompetitivelyPriced(double proposedRent, double marketAverage) const {
    double lowerBound = marketAverage * 0.9;
    double upperBound = marketAverage * 1.1;
    return proposedRent >= lowerBound && proposedRent <= upperBound;
}

double RentalPricing::calculateOptimalRent(std::shared_ptr<Property> property, double marketAverage) const {
    if (!property) return 0.0;
    
    double baseRent = calculateBaseRent(property);
    double adjustedRent = calculateMarketAdjustedRent(baseRent, "standard");
    double amenityPremium = calculateAmenityPremium(property);
    
    double optimalRent = adjustedRent + amenityPremium;
    
    if (!isCompetitivelyPriced(optimalRent, marketAverage)) {
        optimalRent = marketAverage * 1.05;
    }
    
    return optimalRent;
}

double RentalPricing::calculatePriceSensitivity(double currentRent, double proposedRent) const {
    if (currentRent <= 0) return 0.0;
    double percentageChange = ((proposedRent - currentRent) / currentRent) * 100.0;
    return std::abs(percentageChange);
}

std::string RentalPricing::getPricingRecommendation(std::shared_ptr<Property> property) const {
    if (!property) return "insufficient_data";
    
    double baseRent = calculateBaseRent(property);
    if (baseRent < 1000.0) return "economy_pricing";
    if (baseRent < 3000.0) return "mid_range_pricing";
    if (baseRent < 7000.0) return "premium_pricing";
    return "luxury_pricing";
}