#include "../../include/business/OwnerManager.h"
#include <algorithm>

OwnerManager::OwnerManager(double managementFee, int paymentFrequency)
    : managementFeePercentage(managementFee), paymentFrequencyDays(paymentFrequency) {}

OwnerManager::OwnerManager(const OwnerManager& other)
    : owners(other.owners), ownerProperties(other.ownerProperties),
      ownerPayments(other.ownerPayments), managementFeePercentage(other.managementFeePercentage),
      paymentFrequencyDays(other.paymentFrequencyDays) {}

OwnerManager& OwnerManager::operator=(const OwnerManager& other) {
    if (this != &other) {
        owners = other.owners;
        ownerProperties = other.ownerProperties;
        ownerPayments = other.ownerPayments;
        managementFeePercentage = other.managementFeePercentage;
        paymentFrequencyDays = other.paymentFrequencyDays;
    }
    return *this;
}

OwnerManager::OwnerManager(OwnerManager&& other) noexcept
    : owners(std::move(other.owners)), ownerProperties(std::move(other.ownerProperties)),
      ownerPayments(std::move(other.ownerPayments)), managementFeePercentage(other.managementFeePercentage),
      paymentFrequencyDays(other.paymentFrequencyDays) {}

OwnerManager& OwnerManager::operator=(OwnerManager&& other) noexcept {
    if (this != &other) {
        owners = std::move(other.owners);
        ownerProperties = std::move(other.ownerProperties);
        ownerPayments = std::move(other.ownerPayments);
        managementFeePercentage = other.managementFeePercentage;
        paymentFrequencyDays = other.paymentFrequencyDays;
    }
    return *this;
}

OwnerManager::~OwnerManager() {}

double OwnerManager::getManagementFeePercentage() const { return managementFeePercentage; }
int OwnerManager::getPaymentFrequencyDays() const { return paymentFrequencyDays; }

void OwnerManager::setManagementFeePercentage(double newFee) {
    managementFeePercentage = std::max(0.0, std::min(100.0, newFee));
}

void OwnerManager::setPaymentFrequencyDays(int newFrequency) {
    paymentFrequencyDays = std::max(7, newFrequency);
}

bool OwnerManager::registerOwner(std::shared_ptr<Owner> owner) {
    if (!owner) return false;
    
    owners.push_back(owner);
    return true;
}

bool OwnerManager::assignPropertyToOwner(std::shared_ptr<Owner> owner, std::shared_ptr<Property> property) {
    if (!owner || !property) return false;
    
    ownerProperties[owner->getOwnerId()].push_back(property);
    return true;
}

double OwnerManager::calculateOwnerPayment(std::shared_ptr<Owner> owner, double grossRevenue) const {
    if (!owner) return 0.0;
    
    double managementFee = calculateManagementFee(grossRevenue);
    double ownerShare = (owner->getOwnershipPercentage() / 100.0);
    
    return (grossRevenue - managementFee) * ownerShare;
}

std::vector<std::shared_ptr<Property>> OwnerManager::getPropertiesByOwner(const std::string& ownerId) const {
    auto it = ownerProperties.find(ownerId);
    return it != ownerProperties.end() ? it->second : std::vector<std::shared_ptr<Property>>();
}

std::vector<std::shared_ptr<Payment>> OwnerManager::getPaymentHistory(const std::string& ownerId) const {
    auto it = ownerPayments.find(ownerId);
    return it != ownerPayments.end() ? it->second : std::vector<std::shared_ptr<Payment>>();
}

double OwnerManager::calculateTotalRevenueForOwner(const std::string& ownerId) const {
    double totalRevenue = 0.0;
    auto payments = getPaymentHistory(ownerId);
    
    for (const auto& payment : payments) {
        if (payment->getStatus() == "completed") {
            totalRevenue += payment->getAmount();
        }
    }
    
    return totalRevenue;
}

bool OwnerManager::isOwnerActive(const std::string& ownerId) const {
    auto properties = getPropertiesByOwner(ownerId);
    return !properties.empty();
}

double OwnerManager::calculateManagementFee(double grossRevenue) const {
    return grossRevenue * (managementFeePercentage / 100.0);
}

std::vector<std::shared_ptr<Owner>> OwnerManager::getOwnersWithMultipleProperties() const {
    std::vector<std::shared_ptr<Owner>> multiPropertyOwners;
    
    for (const auto& owner : owners) {
        auto properties = getPropertiesByOwner(owner->getOwnerId());
        if (properties.size() > 1) {
            multiPropertyOwners.push_back(owner);
        }
    }
    
    return multiPropertyOwners;
}

bool OwnerManager::canOwnerAddMoreProperties(const std::string& ownerId) const {
    auto properties = getPropertiesByOwner(ownerId);
    return properties.size() < 10;
}

double OwnerManager::calculateOwnerPortfolioValue(const std::string& ownerId) const {
    auto properties = getPropertiesByOwner(ownerId);
    double totalValue = 0.0;
    
    for (const auto& property : properties) {
        totalValue += property->getRentalPrice() * 100;
    }
    
    return totalValue;
}