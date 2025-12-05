#ifndef RENTALPRICING_H
#define RENTALPRICING_H

#include <string>
#include <unordered_map>

class RentalPricing {
private:
    std::unordered_map<std::string, double> categoryBaseRates;
    std::unordered_map<std::string, double> seasonalMultipliers;
    std::unordered_map<std::string, double> customerTypeDiscounts;

public:
    RentalPricing();
    
    double calculateDynamicPrice(const std::string& category, int durationDays, 
                                const std::string& season, const std::string& customerType) const;
    double applyLoyaltyDiscount(double basePrice, int loyaltyPoints) const;
    double calculateBulkDiscount(int itemCount, double totalPrice) const;
    double getSeasonalMultiplier(const std::string& season) const;
    double getCustomerDiscount(const std::string& customerType) const;
    double calculatePenaltyRate(int lateDays) const;
    
    double getBaseRate(const std::string& category) const;
    
    void updateBaseRate(const std::string& category, double newRate);
};

#endif