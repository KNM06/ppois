#ifndef RENTALPRICING_H
#define RENTALPRICING_H

#include <string>
#include <memory>
#include "../../include/core/Property.h"
#include "../../include/core/PropertyFeature.h"

class RentalPricing {
private:
    double basePricePerSqFt;
    double locationMultiplier;
    double amenityMultiplier;
    double marketDemandFactor;
    double seasonalityAdjustment;
    int minimumLeaseTerm;
    std::string pricingStrategy;

public:
    RentalPricing(double basePrice, double locationMultiplier, double amenityMultiplier,
                  double marketDemand, double seasonality, int minLeaseTerm,
                  const std::string& strategy);
    
    RentalPricing(const RentalPricing& other);
    RentalPricing& operator=(const RentalPricing& other);
    RentalPricing(RentalPricing&& other) noexcept;
    RentalPricing& operator=(RentalPricing&& other) noexcept;
    ~RentalPricing();

    double getBasePricePerSqFt() const;
    double getLocationMultiplier() const;
    double getAmenityMultiplier() const;
    double getMarketDemandFactor() const;
    double getSeasonalityAdjustment() const;
    int getMinimumLeaseTerm() const;
    std::string getPricingStrategy() const;

    void setBasePricePerSqFt(double newPrice);
    void setLocationMultiplier(double newMultiplier);
    void setMarketDemandFactor(double newFactor);
    void setSeasonalityAdjustment(double newAdjustment);

    double calculateBaseRent(std::shared_ptr<Property> property) const;
    double calculateMarketAdjustedRent(double baseRent, const std::string& neighborhood) const;
    double calculateAmenityPremium(std::shared_ptr<Property> property) const;
    double calculateLocationPremium(const std::string& locationTier) const;
    double calculateSeasonalAdjustment(int month) const;
    double calculateLongTermDiscount(int leaseMonths) const;
    bool isCompetitivelyPriced(double proposedRent, double marketAverage) const;
    double calculateOptimalRent(std::shared_ptr<Property> property, double marketAverage) const;
    double calculatePriceSensitivity(double currentRent, double proposedRent) const;
    std::string getPricingRecommendation(std::shared_ptr<Property> property) const;
};

#endif