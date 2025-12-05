#ifndef DISCOUNTCALCULATOR_H
#define DISCOUNTCALCULATOR_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class PricingStrategy;

class DiscountCalculator {
private:
    std::unordered_map<std::string, double> seasonalDiscounts;
    std::unordered_map<std::string, double> membershipDiscounts;
    std::unordered_map<std::string, double> bulkDiscountRates;
    std::shared_ptr<PricingStrategy> pricingStrategy;
    std::unordered_map<std::string, double> promotionalDiscounts;

    bool areCategoriesCompatible(const std::string& cat1, const std::string& cat2) const;

public:
    DiscountCalculator();
    DiscountCalculator(std::shared_ptr<PricingStrategy> pricing);
    
    double calculateSeasonalDiscount(const std::string& season, const std::string& category) const;
    double calculateMembershipDiscount(const std::string& membershipType, int loyaltyPoints) const;
    double calculateBulkDiscount(int itemCount, double totalAmount) const;
    double calculateEarlyBookingDiscount(int daysInAdvance) const;
    double calculateLongTermDiscount(int rentalDays) const;
    double calculateBundleDiscount(const std::vector<std::string>& categories) const;
    bool isPromotionApplicable(const std::string& promotionCode, double totalAmount) const;
    double calculateReferralDiscount(int referralCount) const;
    std::unordered_map<std::string, double> getAvailableDiscounts(const std::string& customerType) const;
    
    void updateSeasonalDiscount(const std::string& season, double discount);
    std::shared_ptr<PricingStrategy> getPricingStrategy() const;
    void setPricingStrategy(std::shared_ptr<PricingStrategy> pricing);
    void addPromotionalDiscount(const std::string& promotionCode, double discount);
};

#endif