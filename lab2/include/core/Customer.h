#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "../../include/core/RentalHistory.h"
#include "../../include/business/PaymentProcessor.h"
#include "../../include/utils/ValidationUtils.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class RentalHistory;
class PaymentProcessor;

enum class CustomerStatus {
    NEW,
    REGULAR,
    VIP,
    BUSINESS,
    BLACKLISTED
};

enum class CustomerTier {
    BRONZE,
    SILVER,
    GOLD,
    PLATINUM
};

class Customer {
private:
    std::string customerId;
    std::string name;
    std::string email;
    std::string phoneNumber;
    std::string address;
    CustomerStatus status;
    CustomerTier tier;
    int age;
    double creditScore;
    std::unique_ptr<RentalHistory> rentalHistory;
    std::shared_ptr<PaymentProcessor> paymentProcessor;
    int loyaltyPoints;
    double totalSpent;
    std::vector<std::string> preferredCategories;
    std::unordered_map<std::string, int> rentalLimits;
    bool emailVerified;
    std::string membershipStartDate;

public:
    Customer(const std::string& id, const std::string& customerName, 
             const std::string& customerEmail, int customerAge);
    Customer(const std::string& id, const std::string& customerName, 
             const std::string& customerEmail, int customerAge,
             std::shared_ptr<PaymentProcessor> paymentProc);
    ~Customer() = default;
    
    // Уникальные методы бизнес-логики
    double calculateLoyaltyDiscount() const;
    bool isEligibleForPremiumItems() const;
    double getRiskAssessment() const;
    void updateCustomerStatusBasedOnActivity();
    int calculateRentalLimit() const;
    bool validateRentalCompatibility(const std::string& itemCategory) const;
    double calculateCustomerValueScore() const;
    bool canUpgradeTier() const;
    void applyTierUpgrade();
    std::vector<std::string> getRecommendedCategories() const;
    double predictSpendingAmount() const;
    bool validatePaymentMethod() const;
    void addPreferredCategory(const std::string& category);
    
    // Getters
    std::string getCustomerId() const { return customerId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    CustomerStatus getStatus() const { return status; }
    CustomerTier getTier() const { return tier; }
    double getCreditScore() const { return creditScore; }
    int getLoyaltyPoints() const { return loyaltyPoints; }
    double getTotalSpent() const { return totalSpent; }
    std::shared_ptr<PaymentProcessor> getPaymentProcessor() const;
    std::vector<std::string> getPreferredCategories() const;
    bool getEmailVerified() const { return emailVerified; }
    
    // Setters
    void setStatus(CustomerStatus newStatus) { status = newStatus; }
    void setTier(CustomerTier newTier) { tier = newTier; }
    void setCreditScore(double score) { creditScore = std::max(300.0, std::min(850.0, score)); }
    void setPaymentProcessor(std::shared_ptr<PaymentProcessor> processor);
    void setEmailVerified(bool verified) { emailVerified = verified; }
    void setPhoneNumber(const std::string& phone) { 
        if (ValidationUtils::isValidPhoneNumber(phone)) {
            phoneNumber = phone;
        }
    }
    void setAddress(const std::string& addr) { address = addr; }
    
    void addLoyaltyPoints(int points) { loyaltyPoints += points; }
    void addRentalToHistory(double amount);
    void updateRentalLimit(const std::string& category, int limit);
};

#endif