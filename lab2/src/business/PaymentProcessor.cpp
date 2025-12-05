#include "../../include/business/PaymentProcessor.h"
#include "../../include/business/PricingStrategy.h"
#include "../../include/business/DiscountCalculator.h"
#include "../../include/utils/ValidationUtils.h"
#include <cmath>
#include <random>
#include <ctime>

PaymentProcessor::PaymentProcessor() 
    : transactionFeeRate(0.02), taxRate(0.08), pricingStrategy(nullptr), discountCalculator(nullptr) {
    
    categoryBaseRates["VEHICLE"] = 75.0;
    categoryBaseRates["ELECTRONICS"] = 45.0;
    categoryBaseRates["CONSTRUCTION"] = 100.0;
    categoryBaseRates["TOOLS"] = 25.0;
    categoryBaseRates["SPORTS"] = 30.0;
}

PaymentProcessor::PaymentProcessor(std::shared_ptr<PricingStrategy> pricing, 
                                 std::shared_ptr<DiscountCalculator> discount)
    : transactionFeeRate(0.02), taxRate(0.08), pricingStrategy(pricing), discountCalculator(discount) {
    
    categoryBaseRates["VEHICLE"] = 75.0;
    categoryBaseRates["ELECTRONICS"] = 45.0;
    categoryBaseRates["CONSTRUCTION"] = 100.0;
    categoryBaseRates["TOOLS"] = 25.0;
    categoryBaseRates["SPORTS"] = 30.0;
}

double PaymentProcessor::calculateTotalAmount(const std::string& itemId, int durationDays, const std::string& customerId) const {
    std::string category = "TOOLS";
    if (itemId.find("VEH") != std::string::npos) category = "VEHICLE";
    else if (itemId.find("ELEC") != std::string::npos) category = "ELECTRONICS";
    else if (itemId.find("CONST") != std::string::npos) category = "CONSTRUCTION";
    else if (itemId.find("SPORT") != std::string::npos) category = "SPORTS";
    
    double baseRate = categoryBaseRates.at(category);
    double basePrice = baseRate * durationDays;
    
    double dynamicPrice = calculateDynamicPricing(itemId, durationDays, basePrice);
    
    double finalAmount = dynamicPrice;
    applyLoyaltyDiscount(finalAmount, customerId);
    
    finalAmount += calculateTaxAndFees(finalAmount, "REGULAR");
    
    return finalAmount;
}

bool PaymentProcessor::processPayment(const std::string& customerId, double amount) {
    if (!validatePaymentMethod(customerId)) {
        failedAttempts[customerId]++;
        return false;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    bool success = dis(gen) < 0.95;
    
    if (success) {
        failedAttempts[customerId] = 0;
    } else {
        failedAttempts[customerId]++;
    }
    
    return success;
}

double PaymentProcessor::calculateDynamicPricing(const std::string& itemId, int durationDays, double basePrice) const {
    double multiplier = 1.0;
    
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    int hour = timeinfo->tm_hour;
    
    if ((hour >= 17 && hour <= 20) || (hour >= 10 && hour <= 12)) {
        multiplier *= 1.1;
    }
    
    int dayOfWeek = timeinfo->tm_wday;
    if (dayOfWeek == 0 || dayOfWeek == 6) {
        multiplier *= 1.15;
    }
    
    if (durationDays > 7) {
        multiplier *= 0.9;
    }
    if (durationDays > 30) {
        multiplier *= 0.8;
    }
    
    return basePrice * multiplier;
}

bool PaymentProcessor::validatePaymentMethod(const std::string& customerId) const {
    auto it = failedAttempts.find(customerId);
    if (it != failedAttempts.end() && it->second >= 3) {
        return false;
    }
    
    auto methodIt = paymentMethods.find(customerId);
    if (methodIt != paymentMethods.end()) {
        return ValidationUtils::isValidCreditCard(methodIt->second);
    }
    
    return true;
}

double PaymentProcessor::calculateTaxAndFees(double subtotal, const std::string& customerType) const {
    double tax = subtotal * taxRate;
    double transactionFee = subtotal * transactionFeeRate;
    
    if (customerType == "VIP") {
        transactionFee = 0.0;
    }
    
    double environmentalFee = 2.50;
    
    return tax + transactionFee + environmentalFee;
}

void PaymentProcessor::applyLoyaltyDiscount(double& amount, const std::string& customerId) const {
    if (customerId.find("VIP") != std::string::npos) {
        amount *= 0.85;
    } else if (customerId.find("REG") != std::string::npos) {
        amount *= 0.95;
    }
    
    if (customerId.find("NEW") != std::string::npos) {
        amount *= 0.9;
    }
}

bool PaymentProcessor::processRefund(const std::string& customerId, double amount, const std::string& reason) {
    if (amount <= 0) {
        return false;
    }
    
    double refundAmount = amount;
    if (reason == "CANCELLATION") {
        refundAmount *= 0.9;
    } else if (reason == "DAMAGE") {
        refundAmount *= 0.5;
    }
    
    return true;
}

std::string PaymentProcessor::generatePaymentReceipt(const std::string& customerId, double amount) const {
    std::string receipt = "Payment Receipt\n";
    receipt += "Customer: " + customerId + "\n";
    receipt += "Amount: $" + std::to_string(amount) + "\n";
    receipt += "Tax: $" + std::to_string(amount * taxRate) + "\n";
    receipt += "Transaction Fee: $" + std::to_string(amount * transactionFeeRate) + "\n";
    
    return receipt;
}

std::shared_ptr<PricingStrategy> PaymentProcessor::getPricingStrategy() const {
    return pricingStrategy;
}

std::shared_ptr<DiscountCalculator> PaymentProcessor::getDiscountCalculator() const {
    return discountCalculator;
}

void PaymentProcessor::setPricingStrategy(std::shared_ptr<PricingStrategy> pricing) {
    pricingStrategy = pricing;
}

void PaymentProcessor::setDiscountCalculator(std::shared_ptr<DiscountCalculator> discount) {
    discountCalculator = discount;
}

void PaymentProcessor::setPaymentMethod(const std::string& customerId, const std::string& method) {
    paymentMethods[customerId] = method;
}