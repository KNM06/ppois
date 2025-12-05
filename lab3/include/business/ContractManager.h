#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "../../include/finance/RentalContract.h"
#include "../../include/core/Property.h"
#include "../../include/core/Tenant.h"
#include "../../include/core/Owner.h"

class ContractManager {
private:
    std::vector<std::shared_ptr<RentalContract>> contracts;
    std::unordered_map<std::string, std::shared_ptr<RentalContract>> propertyContractMap;
    std::unordered_map<std::string, std::vector<std::shared_ptr<RentalContract>>> tenantContractHistory;
    int autoRenewalNoticeDays;
    double securityDepositMultiplier;
    int maxLeaseTermMonths;
    std::vector<std::string> standardClauses;

public:
    ContractManager(int renewalNotice, double depositMultiplier, int maxLeaseTerm,
                   const std::vector<std::string>& clauses);
    
    ContractManager(const ContractManager& other);
    ContractManager& operator=(const ContractManager& other);
    ContractManager(ContractManager&& other) noexcept;
    ContractManager& operator=(ContractManager&& other) noexcept;
    ~ContractManager();

    int getAutoRenewalNoticeDays() const;
    double getSecurityDepositMultiplier() const;
    int getMaxLeaseTermMonths() const;
    std::vector<std::string> getStandardClauses() const;

    void setAutoRenewalNoticeDays(int newDays);
    void setSecurityDepositMultiplier(double newMultiplier);
    void setMaxLeaseTermMonths(int newMax);
    void addStandardClause(const std::string& clause);

    bool createRentalContract(std::shared_ptr<Property> property,
                             std::shared_ptr<Tenant> tenant,
                             std::shared_ptr<Owner> owner,
                             const std::chrono::system_clock::time_point& startDate,
                             int leaseTermMonths,
                             double monthlyRent,
                             const std::string& paymentTerms);
    bool terminateContract(const std::string& contractId, const std::string& terminationReason);
    bool renewContract(const std::string& contractId, int renewalTermMonths, double newRent);
    std::shared_ptr<RentalContract> findContractByProperty(const std::string& propertyId) const;
    std::vector<std::shared_ptr<RentalContract>> getExpiringContracts(int daysThreshold) const;
    std::vector<std::shared_ptr<RentalContract>> getActiveContracts() const;
    double calculateTotalSecurityDeposits() const;
    bool canTenantRentAnotherProperty(const std::string& tenantId) const;
    double calculateRequiredSecurityDeposit(double monthlyRent) const;
    bool isLeaseTermValid(int leaseTermMonths) const;
    std::vector<std::shared_ptr<RentalContract>> getContractsByTenant(const std::string& tenantId) const;
    bool hasContractViolations(const std::string& tenantId) const;
    double calculateTotalMonthlyRent() const;
};

#endif