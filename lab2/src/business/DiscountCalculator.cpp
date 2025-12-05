#include "../../include/business/DiscountCalculator.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/utils/DateUtils.h"
#include <algorithm>
#include <cmath>

DiscountCalculator::DiscountCalculator() 
    : pricingStrategy(nullptr) {
    
    seasonalDiscounts["WINTER"] = 0.15;
    seasonalDiscounts["SPRING"] = 0.10;
    seasonalDiscounts["SUMMER"] = 0.05;
    seasonalDiscounts["AUTUMN"] = 0.12;
    
    membershipDiscounts["BASIC"] = 0.05;
    membershipDiscounts["PREMIUM"] = 0.15;
    membershipDiscounts["BUSINESS"] = 0.20;
    membershipDiscounts["STUDENT"] = 0.10;
    
    bulkDiscountRates["3"] = 0.05;
    bulkDiscountRates["5"] = 0.10;
    bulkDiscountRates["10"] = 0.15;
    bulkDiscountRates["20"] = 0.25;
    
    promotionalDiscounts["WELCOME10"] = 0.10;
    promotionalDiscounts["SUMMER25"] = 0.25;
    promotionalDiscounts["FIRSTORDER"] = 0.15;
}

DiscountCalculator::DiscountCalculator(std::shared_ptr<PricingStrategy> pricing)
    : pricingStrategy(pricing) {
    
    seasonalDiscounts["WINTER"] = 0.15;
    seasonalDiscounts["SPRING"] = 0.10;
    seasonalDiscounts["SUMMER"] = 0.05;
    seasonalDiscounts["AUTUMN"] = 0.12;
    
    membershipDiscounts["BASIC"] = 0.05;
    membershipDiscounts["PREMIUM"] = 0.15;
    membershipDiscounts["BUSINESS"] = 0.20;
    membershipDiscounts["STUDENT"] = 0.10;
    
    bulkDiscountRates["3"] = 0.05;
    bulkDiscountRates["5"] = 0.10;
    bulkDiscountRates["10"] = 0.15;
    bulkDiscountRates["20"] = 0.25;
}

bool DiscountCalculator::areCategoriesCompatible(const std::string& cat1, const std::string& cat2) const {
    if ((cat1 == "CAMERA" && cat2 == "TRIPOD") ||
        (cat1 == "TRIPOD" && cat2 == "CAMERA")) {
        return true;
    }
    
    if ((cat1 == "CONSTRUCTION" && cat2 == "TOOLS") ||
        (cat1 == "TOOLS" && cat2 == "CONSTRUCTION")) {
        return true;
    }
    
    if ((cat1 == "SPORTS" && cat2 == "OUTDOOR") ||
        (cat1 == "OUTDOOR" && cat2 == "SPORTS")) {
        return true;
    }
    
    if ((cat1 == "ELECTRONICS" && cat2 == "ACCESSORIES") ||
        (cat1 == "ACCESSORIES" && cat2 == "ELECTRONICS")) {
        return true;
    }
    
    return false;
}

double DiscountCalculator::calculateSeasonalDiscount(const std::string& season, const std::string& category) const {
    double baseDiscount = 0.0;
    
    auto seasonIt = seasonalDiscounts.find(season);
    if (seasonIt != seasonalDiscounts.end()) {
        baseDiscount = seasonIt->second;
    }
    
    double categoryMultiplier = 1.0;
    if (category == "WINTER_SPORTS" && season == "WINTER") {
        categoryMultiplier = 0.5;
    } else if (category == "GARDEN" && season == "SPRING") {
        categoryMultiplier = 0.7;
    } else if (category == "OFF_SEASON") {
        categoryMultiplier = 1.5;
    }
    
    return baseDiscount * categoryMultiplier;
}

double DiscountCalculator::calculateMembershipDiscount(const std::string& membershipType, int loyaltyPoints) const {
    double baseDiscount = 0.0;
    
    auto membershipIt = membershipDiscounts.find(membershipType);
    if (membershipIt != membershipDiscounts.end()) {
        baseDiscount = membershipIt->second;
    }
    
    double pointsDiscount = std::min(0.1, loyaltyPoints * 0.0001);
    
    double totalDiscount = baseDiscount + pointsDiscount;
    return std::min(0.3, totalDiscount);
}

double DiscountCalculator::calculateBulkDiscount(int itemCount, double totalAmount) const {
    double discountRate = 0.0;
    
    if (itemCount >= 20) {
        discountRate = bulkDiscountRates.at("20");
    } else if (itemCount >= 10) {
        discountRate = bulkDiscountRates.at("10");
    } else if (itemCount >= 5) {
        discountRate = bulkDiscountRates.at("5");
    } else if (itemCount >= 3) {
        discountRate = bulkDiscountRates.at("3");
    }
    
    if (totalAmount > 1000) {
        discountRate += 0.05;
    } else if (totalAmount > 500) {
        discountRate += 0.03;
    } else if (totalAmount > 200) {
        discountRate += 0.02;
    }
    
    return std::min(0.4, discountRate);
}

double DiscountCalculator::calculateEarlyBookingDiscount(int daysInAdvance) const {
    if (daysInAdvance <= 0) return 0.0;
    
    double discount = 0.0;
    
    if (daysInAdvance >= 30) {
        discount = 0.20;
    } else if (daysInAdvance >= 14) {
        discount = 0.15;
    } else if (daysInAdvance >= 7) {
        discount = 0.10;
    } else if (daysInAdvance >= 3) {
        discount = 0.05;
    }
    
    return discount;
}

double DiscountCalculator::calculateLongTermDiscount(int rentalDays) const {
    if (rentalDays <= 0) return 0.0;
    
    double discount = 0.0;
    
    if (rentalDays >= 90) {
        discount = 0.35;
    } else if (rentalDays >= 60) {
        discount = 0.25;
    } else if (rentalDays >= 30) {
        discount = 0.15;
    } else if (rentalDays >= 14) {
        discount = 0.10;
    } else if (rentalDays >= 7) {
        discount = 0.05;
    }
    
    return discount;
}

double DiscountCalculator::calculateBundleDiscount(const std::vector<std::string>& categories) const {
    if (categories.empty()) return 0.0;
    
    double baseDiscount = 0.05;
    
    int compatiblePairs = 0;
    for (size_t i = 0; i < categories.size(); ++i) {
        for (size_t j = i + 1; j < categories.size(); ++j) {
            if (areCategoriesCompatible(categories[i], categories[j])) {
                compatiblePairs++;
            }
        }
    }
    
    baseDiscount += compatiblePairs * 0.02;
    
    if (categories.size() >= 5) {
        baseDiscount += 0.08;
    } else if (categories.size() >= 3) {
        baseDiscount += 0.05;
    }
    
    return std::min(0.25, baseDiscount);
}

bool DiscountCalculator::isPromotionApplicable(const std::string& promotionCode, double totalAmount) const {
    if (promotionCode.empty()) return false;
    
    if (totalAmount < 50.0) {
        return false;
    }
    
    if (promotionCode.find("SUMMER") != std::string::npos) {
        return true;
    } else if (promotionCode.find("FIRST") != std::string::npos) {
        return true;
    } else if (promotionCode.find("VIP") != std::string::npos) {
        return totalAmount > 100.0;
    }
    
    auto promoIt = promotionalDiscounts.find(promotionCode);
    if (promoIt != promotionalDiscounts.end()) {
        return totalAmount >= 50.0;
    }
    
    return promotionCode.length() >= 6;
}

double DiscountCalculator::calculateReferralDiscount(int referralCount) const {
    double baseDiscount = 0.05;
    
    if (referralCount >= 10) {
        baseDiscount = 0.20;
    } else if (referralCount >= 5) {
        baseDiscount = 0.15;
    } else if (referralCount >= 3) {
        baseDiscount = 0.10;
    }
    
    return baseDiscount;
}

std::unordered_map<std::string, double> DiscountCalculator::getAvailableDiscounts(const std::string& customerType) const {
    std::unordered_map<std::string, double> availableDiscounts;
    
    availableDiscounts["Seasonal"] = calculateSeasonalDiscount("CURRENT", "GENERAL");
    availableDiscounts["Membership"] = calculateMembershipDiscount(customerType, 0);
    availableDiscounts["Bulk"] = calculateBulkDiscount(3, 200.0);
    availableDiscounts["LongTerm"] = calculateLongTermDiscount(7);
    
    for (const auto& [code, discount] : promotionalDiscounts) {
        availableDiscounts[code] = discount;
    }
    
    return availableDiscounts;
}

void DiscountCalculator::updateSeasonalDiscount(const std::string& season, double discount) {
    seasonalDiscounts[season] = std::max(0.0, std::min(0.5, discount));
}

std::shared_ptr<PricingStrategy> DiscountCalculator::getPricingStrategy() const {
    return pricingStrategy;
}

void DiscountCalculator::setPricingStrategy(std::shared_ptr<PricingStrategy> pricing) {
    pricingStrategy = pricing;
}

void DiscountCalculator::addPromotionalDiscount(const std::string& promotionCode, double discount) {
    promotionalDiscounts[promotionCode] = std::max(0.0, std::min(0.5, discount));
}