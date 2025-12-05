#ifndef OWNERMANAGER_H
#define OWNERMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../include/core/Owner.h"
#include "../../include/core/Property.h"
#include "../../include/finance/Payment.h"

class OwnerManager {
private:
    std::vector<std::shared_ptr<Owner>> owners;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Property>>> ownerProperties;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Payment>>> ownerPayments;
    double managementFeePercentage;
    int paymentFrequencyDays;

public:
    OwnerManager(double managementFee, int paymentFrequency);
    
    OwnerManager(const OwnerManager& other);
    OwnerManager& operator=(const OwnerManager& other);
    OwnerManager(OwnerManager&& other) noexcept;
    OwnerManager& operator=(OwnerManager&& other) noexcept;
    ~OwnerManager();

    double getManagementFeePercentage() const;
    int getPaymentFrequencyDays() const;

    void setManagementFeePercentage(double newFee);
    void setPaymentFrequencyDays(int newFrequency);

    bool registerOwner(std::shared_ptr<Owner> owner);
    bool assignPropertyToOwner(std::shared_ptr<Owner> owner, std::shared_ptr<Property> property);
    double calculateOwnerPayment(std::shared_ptr<Owner> owner, double grossRevenue) const;
    std::vector<std::shared_ptr<Property>> getPropertiesByOwner(const std::string& ownerId) const;
    std::vector<std::shared_ptr<Payment>> getPaymentHistory(const std::string& ownerId) const;
    double calculateTotalRevenueForOwner(const std::string& ownerId) const;
    bool isOwnerActive(const std::string& ownerId) const;
    double calculateManagementFee(double grossRevenue) const;
    std::vector<std::shared_ptr<Owner>> getOwnersWithMultipleProperties() const;
    bool canOwnerAddMoreProperties(const std::string& ownerId) const;
    double calculateOwnerPortfolioValue(const std::string& ownerId) const;
};

#endif