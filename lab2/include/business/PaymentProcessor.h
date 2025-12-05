#ifndef PAYMENTPROCESSOR_H
#define PAYMENTPROCESSOR_H

#include "../../include/business/PricingStrategy.h"
#include "../../include/business/DiscountCalculator.h"
#include "../../include/utils/ValidationUtils.h"
#include <string>
#include <unordered_map>
#include <memory>

class PricingStrategy;
class DiscountCalculator;

class PaymentProcessor {
private:
    double transactionFeeRate;
    double taxRate;
    std::unordered_map<std::string, double> categoryBaseRates;
    std::unordered_map<std::string, int> failedAttempts;
    std::shared_ptr<PricingStrategy> pricingStrategy;
    std::shared_ptr<DiscountCalculator> discountCalculator;
    std::unordered_map<std::string, std::string> paymentMethods;

public:
    PaymentProcessor();
    PaymentProcessor(std::shared_ptr<PricingStrategy> pricing, 
                    std::shared_ptr<DiscountCalculator> discount);
    
    double calculateTotalAmount(const std::string& itemId, int durationDays, const std::string& customerId) const;
    bool processPayment(const std::string& customerId, double amount);
    double calculateDynamicPricing(const std::string& itemId, int durationDays, double basePrice) const;
    bool validatePaymentMethod(const std::string& customerId) const;
    double calculateTaxAndFees(double subtotal, const std::string& customerType) const;
    void applyLoyaltyDiscount(double& amount, const std::string& customerId) const;
    bool processRefund(const std::string& customerId, double amount, const std::string& reason);
    std::string generatePaymentReceipt(const std::string& customerId, double amount) const;
    
    double getTransactionFeeRate() const { return transactionFeeRate; }
    double getTaxRate() const { return taxRate; }
    std::shared_ptr<PricingStrategy> getPricingStrategy() const;
    std::shared_ptr<DiscountCalculator> getDiscountCalculator() const;
    
    void setPricingStrategy(std::shared_ptr<PricingStrategy> pricing);
    void setDiscountCalculator(std::shared_ptr<DiscountCalculator> discount);
    void setPaymentMethod(const std::string& customerId, const std::string& method);
};

#endif