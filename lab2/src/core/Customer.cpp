#include "../../include/core/Customer.h"
#include "../../include/core/RentalHistory.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/utils/ValidationUtils.h"
#include <cmath>
#include <algorithm>

Customer::Customer(const std::string& id, const std::string& customerName, 
                   const std::string& customerEmail, int customerAge)
    : customerId(id), name(customerName), email(customerEmail), age(customerAge),
      status(CustomerStatus::NEW), tier(CustomerTier::BRONZE), creditScore(500.0), 
      rentalHistory(std::make_unique<RentalHistory>()),
      paymentProcessor(nullptr), loyaltyPoints(0), totalSpent(0.0),
      emailVerified(false), membershipStartDate("2024-01-01") {
    
    rentalLimits["VEHICLE"] = 1;
    rentalLimits["ELECTRONICS"] = 2;
    rentalLimits["CONSTRUCTION"] = 1;
    rentalLimits["TOOLS"] = 3;
    rentalLimits["SPORTS"] = 2;
}

Customer::Customer(const std::string& id, const std::string& customerName, 
                   const std::string& customerEmail, int customerAge,
                   std::shared_ptr<PaymentProcessor> paymentProc)
    : customerId(id), name(customerName), email(customerEmail), age(customerAge),
      status(CustomerStatus::NEW), tier(CustomerTier::BRONZE), creditScore(500.0), 
      rentalHistory(std::make_unique<RentalHistory>()),
      paymentProcessor(paymentProc), loyaltyPoints(0), totalSpent(0.0),
      emailVerified(false), membershipStartDate("2024-01-01") {
    
    rentalLimits["VEHICLE"] = 1;
    rentalLimits["ELECTRONICS"] = 2;
    rentalLimits["CONSTRUCTION"] = 1;
    rentalLimits["TOOLS"] = 3;
    rentalLimits["SPORTS"] = 2;
}

double Customer::calculateLoyaltyDiscount() const {
    double baseDiscount = 0.0;
    
    switch (tier) {
        case CustomerTier::PLATINUM: 
            baseDiscount = 0.20;
            break;
        case CustomerTier::GOLD:
            baseDiscount = 0.15;
            break;
        case CustomerTier::SILVER:
            baseDiscount = 0.10;
            break;
        case CustomerTier::BRONZE:
            baseDiscount = 0.05;
            break;
    }
    
    double pointsDiscount = std::min(0.1, loyaltyPoints * 0.00005);
    double statusBonus = (status == CustomerStatus::VIP) ? 0.05 : 0.0;
    
    return baseDiscount + pointsDiscount + statusBonus;
}

bool Customer::isEligibleForPremiumItems() const {
    if (status == CustomerStatus::BLACKLISTED) return false;
    
    if (age < 21) return false;
    
    if (creditScore < 650) return false;
    
    int successfulRentals = rentalHistory->getSuccessfulRentalCount();
    bool hasPremiumExperience = successfulRentals >= 5 || tier >= CustomerTier::GOLD;
    
    return hasPremiumExperience;
}

double Customer::getRiskAssessment() const {
    double riskScore = 0.0;
    
    riskScore += (850.0 - creditScore) / 850.0 * 0.4;
    
    if (age < 25) riskScore += 0.2;
    else if (age > 65) riskScore += 0.1;
    
    double lateReturnsRatio = rentalHistory->getLateReturnRatio();
    riskScore += lateReturnsRatio * 0.3;
    
    if (status == CustomerStatus::NEW) riskScore += 0.1;
    else if (status == CustomerStatus::BLACKLISTED) riskScore = 1.0;
    
    if (!emailVerified) riskScore += 0.05;
    
    if (paymentProcessor && !validatePaymentMethod()) {
        riskScore += 0.1;
    }
    
    return std::min(1.0, riskScore);
}

void Customer::updateCustomerStatusBasedOnActivity() {
    int rentalCount = rentalHistory->getSuccessfulRentalCount();
    double totalSpent = rentalHistory->getTotalAmountSpent();
    double onTimeRatio = rentalHistory->getOnTimePaymentRatio();
    
    if (rentalCount >= 20 && totalSpent > 5000.0 && onTimeRatio > 0.9) {
        status = CustomerStatus::VIP;
        tier = CustomerTier::PLATINUM;
    } else if (rentalCount >= 10 && totalSpent > 2000.0 && onTimeRatio > 0.8) {
        status = CustomerStatus::REGULAR;
        tier = CustomerTier::GOLD;
    } else if (rentalCount >= 5 && totalSpent > 500.0) {
        status = CustomerStatus::REGULAR;
        tier = CustomerTier::SILVER;
    } else if (rentalCount >= 3) {
        tier = CustomerTier::BRONZE;
    }
    
    double paymentReliability = rentalHistory->getOnTimePaymentRatio();
    creditScore = 300.0 + (paymentReliability * 550.0);
}

int Customer::calculateRentalLimit() const {
    int baseLimit = 1;
    
    switch (status) {
        case CustomerStatus::VIP: baseLimit = 5; break;
        case CustomerStatus::BUSINESS: baseLimit = 10; break;
        case CustomerStatus::REGULAR: baseLimit = 3; break;
        case CustomerStatus::NEW: baseLimit = 1; break;
        default: baseLimit = 0;
    }
    
    if (creditScore > 750) baseLimit += 2;
    else if (creditScore > 650) baseLimit += 1;
    
    if (tier == CustomerTier::PLATINUM) baseLimit += 3;
    else if (tier == CustomerTier::GOLD) baseLimit += 2;
    else if (tier == CustomerTier::SILVER) baseLimit += 1;
    
    return baseLimit;
}

bool Customer::validateRentalCompatibility(const std::string& itemCategory) const {
    if (itemCategory == "VEHICLE" && age < 18) return false;
    if (itemCategory == "CONSTRUCTION" && age < 21) return false;
    if (itemCategory == "LUXURY" && creditScore < 700) return false;
    
    auto limitIt = rentalLimits.find(itemCategory);
    if (limitIt != rentalLimits.end()) {
        int categoryRentalCount = rentalHistory->getRentalCountForCategory(itemCategory);
        if (categoryRentalCount >= limitIt->second && tier < CustomerTier::GOLD) {
            return false;
        }
    }
    
    if (itemCategory == "SPECIALIZED_EQUIPMENT") {
        return rentalHistory->hasRentedCategory("CONSTRUCTION") || 
               tier >= CustomerTier::GOLD;
    }
    
    double avgItemPrice = 0.0;
    if (itemCategory == "LUXURY") avgItemPrice = 500.0;
    else if (itemCategory == "ELECTRONICS") avgItemPrice = 200.0;
    
    if (avgItemPrice > 0 && creditScore < 650) {
        return rentalHistory->getTotalAmountSpent() > avgItemPrice * 2;
    }
    
    return true;
}

double Customer::calculateCustomerValueScore() const {
    double valueScore = 0.0;
    
    double spendingFactor = totalSpent / 10000.0;
    valueScore += std::min(0.4, spendingFactor);
    
    double loyaltyFactor = loyaltyPoints / 1000.0;
    valueScore += std::min(0.3, loyaltyFactor);
    
    double activityFactor = rentalHistory->getSuccessfulRentalCount() / 50.0;
    valueScore += std::min(0.2, activityFactor);
    
    double reliabilityFactor = rentalHistory->getOnTimePaymentRatio();
    valueScore += reliabilityFactor * 0.1;
    
    return std::min(1.0, valueScore);
}

bool Customer::canUpgradeTier() const {
    if (tier == CustomerTier::PLATINUM) return false;
    
    int rentalCount = rentalHistory->getSuccessfulRentalCount();
    double totalSpent = rentalHistory->getTotalAmountSpent();
    double onTimeRatio = rentalHistory->getOnTimePaymentRatio();
    
    switch (tier) {
        case CustomerTier::BRONZE:
            return rentalCount >= 5 && totalSpent >= 500.0 && onTimeRatio >= 0.8;
        case CustomerTier::SILVER:
            return rentalCount >= 10 && totalSpent >= 2000.0 && onTimeRatio >= 0.85;
        case CustomerTier::GOLD:
            return rentalCount >= 20 && totalSpent >= 5000.0 && onTimeRatio >= 0.9;
        default:
            return false;
    }
}

void Customer::applyTierUpgrade() {
    if (!canUpgradeTier()) return;
    
    switch (tier) {
        case CustomerTier::BRONZE:
            tier = CustomerTier::SILVER;
            loyaltyPoints += 500;
            break;
        case CustomerTier::SILVER:
            tier = CustomerTier::GOLD;
            loyaltyPoints += 1000;
            break;
        case CustomerTier::GOLD:
            tier = CustomerTier::PLATINUM;
            loyaltyPoints += 2000;
            status = CustomerStatus::VIP;
            break;
        default:
            break;
    }
}

std::vector<std::string> Customer::getRecommendedCategories() const {
    std::vector<std::string> recommendations;
    
    for (const auto& category : preferredCategories) {
        recommendations.push_back(category);
    }
    
    std::unordered_map<std::string, int> categoryCounts;
    for (const auto& category : preferredCategories) {
        categoryCounts[category] = rentalHistory->getRentalCountForCategory(category);
    }
    
    if (categoryCounts.empty()) {
        recommendations = {"TOOLS", "ELECTRONICS", "SPORTS"};
    }
    
    if (tier >= CustomerTier::GOLD && creditScore > 700) {
        recommendations.push_back("LUXURY");
        recommendations.push_back("SPECIALIZED");
    }
    
    if (age > 40) {
        recommendations.push_back("CONSTRUCTION");
        recommendations.push_back("VEHICLE");
    }
    
    return recommendations;
}

double Customer::predictSpendingAmount() const {
    double basePrediction = 200.0;
    
    double tierMultiplier = 1.0;
    switch (tier) {
        case CustomerTier::PLATINUM: tierMultiplier = 2.5; break;
        case CustomerTier::GOLD: tierMultiplier = 2.0; break;
        case CustomerTier::SILVER: tierMultiplier = 1.5; break;
        case CustomerTier::BRONZE: tierMultiplier = 1.0; break;
    }
    
    double creditMultiplier = creditScore / 850.0;
    double activityMultiplier = 1.0 + (rentalHistory->getSuccessfulRentalCount() * 0.1);
    
    return basePrediction * tierMultiplier * creditMultiplier * activityMultiplier;
}

bool Customer::validatePaymentMethod() const {
    if (!paymentProcessor) {
        return false;
    }
    
    return paymentProcessor->validatePaymentMethod(customerId);
}

void Customer::addPreferredCategory(const std::string& category) {
    if (std::find(preferredCategories.begin(), preferredCategories.end(), category) == preferredCategories.end()) {
        preferredCategories.push_back(category);
    }
}

std::shared_ptr<PaymentProcessor> Customer::getPaymentProcessor() const {
    return paymentProcessor;
}

void Customer::setPaymentProcessor(std::shared_ptr<PaymentProcessor> processor) {
    paymentProcessor = processor;
}

std::vector<std::string> Customer::getPreferredCategories() const {
    return preferredCategories;
}

void Customer::addRentalToHistory(double amount) {
    rentalHistory->addRental(amount);
    totalSpent += amount;
    
    loyaltyPoints += static_cast<int>(amount / 10.0);
    
    updateCustomerStatusBasedOnActivity();
    
    if (canUpgradeTier()) {
        applyTierUpgrade();
    }
}

void Customer::updateRentalLimit(const std::string& category, int limit) {
    rentalLimits[category] = std::max(0, limit);
}