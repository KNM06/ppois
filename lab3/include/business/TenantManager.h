#ifndef TENANTMANAGER_H
#define TENANTMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../include/core/Tenant.h"
#include "../../include/finance/RentalContract.h"
#include "../../include/finance/Payment.h"

class TenantManager {
private:
    std::vector<std::shared_ptr<Tenant>> tenants;
    std::unordered_map<std::string, std::shared_ptr<RentalContract>> tenantContracts;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Payment>>> tenantPaymentHistory;
    double minimumCreditScore;
    double minimumIncomeToRentRatio;

public:
    TenantManager(double minCreditScore, double minIncomeRatio);
    
    TenantManager(const TenantManager& other);
    TenantManager& operator=(const TenantManager& other);
    TenantManager(TenantManager&& other) noexcept;
    TenantManager& operator=(TenantManager&& other) noexcept;
    ~TenantManager();

    double getMinimumCreditScore() const;
    double getMinimumIncomeToRentRatio() const;

    void setMinimumCreditScore(double newScore);
    void setMinimumIncomeToRentRatio(double newRatio);

    bool registerTenant(std::shared_ptr<Tenant> tenant);
    bool approveTenantApplication(std::shared_ptr<Tenant> tenant, double proposedRent);
    std::vector<std::shared_ptr<Payment>> getTenantPaymentHistory(const std::string& tenantId) const;
    double calculateTenantPaymentScore(const std::string& tenantId) const;
    bool hasGoodPaymentHistory(const std::string& tenantId) const;
    std::vector<std::shared_ptr<Tenant>> getTenantsWithLatePayments() const;
    bool canTenantRenewLease(const std::string& tenantId) const;
    double calculateAverageRentPayment(const std::string& tenantId) const;
    std::vector<std::shared_ptr<Tenant>> searchTenantsByCriteria(double minCreditScore, double minIncome) const;
    bool isTenantEligibleForUpgrade(const std::string& tenantId, double newRent) const;
};

#endif